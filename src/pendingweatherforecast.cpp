/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "pendingweatherforecast.h"
#include "geotimezone.h"
#include "kweathercore_p.h"
#include "pendingweatherforecast_p.h"
#include "sunrisesource.h"
#include <KLocalizedString>
#include <QDir>
#include <QExplicitlySharedDataPointer>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QTimeZone>
namespace KWeatherCore
{
PendingWeatherForecastPrivate::PendingWeatherForecastPrivate(
    double latitude,
    double longitude,
    const QString &timezone,
    QNetworkReply *reply,
    const std::vector<Sunrise> &sunrise,
    PendingWeatherForecast *parent)
    : QObject(parent)
    , forecast(
          QExplicitlySharedDataPointer<WeatherForecast>(new WeatherForecast))
    , m_latitude(latitude)
    , m_longitude(longitude)
    , m_timezone(timezone)
{
    connect(this, &PendingWeatherForecastPrivate::finished, [this] {
        this->isFinished = true;
    });
    connect(this,
            &PendingWeatherForecastPrivate::finished,
            parent,
            &PendingWeatherForecast::finished);
    connect(this,
            &PendingWeatherForecastPrivate::networkError,
            parent,
            &PendingWeatherForecast::networkError);
    if (reply) {
        connect(reply, &QNetworkReply::finished, [this, reply] {
            this->parseWeatherForecastResults(reply);
        });
    }

    m_sunriseSource =
        new SunriseSource(latitude, longitude, m_timezone, sunrise, this);
    if (timezone.isEmpty()) {
        hasTimezone = false;
        getTimezone(latitude, longitude);
    } else {
        hasTimezone = true;
        forecast->setTimezone(timezone);
        m_timezone = timezone;
        getSunrise();
    }
}
PendingWeatherForecastPrivate::PendingWeatherForecastPrivate(
    QExplicitlySharedDataPointer<WeatherForecast> data)
    : forecast(data)
    , isFinished(true)
{
}
void PendingWeatherForecastPrivate::getTimezone(double latitude,
                                                double longitude)
{
    auto timezoneSource = new GeoTimezone(latitude, longitude, this);
    connect(timezoneSource,
            &GeoTimezone::finished,
            this,
            &PendingWeatherForecastPrivate::parseTimezoneResult);
}
void PendingWeatherForecastPrivate::parseTimezoneResult(const QString &result)
{
    hasTimezone = true;
    forecast->setTimezone(result);
    m_timezone = result;
    getSunrise();
}

void PendingWeatherForecastPrivate::getSunrise()
{
    connect(m_sunriseSource,
            &SunriseSource::finished,
            this,
            &PendingWeatherForecastPrivate::parseSunriseResults);
    m_sunriseSource->setTimezone(m_timezone);
    m_sunriseSource->requestData();
}
void PendingWeatherForecastPrivate::parseSunriseResults()
{
    hasSunrise = true;

    // if this arrived later than forecast
    if (!hourlyForecast.empty())
        applySunriseToForecast();
}
void PendingWeatherForecastPrivate::parseWeatherForecastResults(
    QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        qWarning() << "network error when fetching forecast:"
                   << reply->errorString();
        Q_EMIT networkError();
        return;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    if (jsonDocument.isObject()) {
        QJsonObject obj = jsonDocument.object();
        QJsonObject prop = obj[QStringLiteral("properties")].toObject();

        if (prop.contains(QStringLiteral("timeseries"))) {
            QJsonArray timeseries =
                prop[QStringLiteral("timeseries")].toArray();

            // loop over all forecast data
            for (const auto &ref : qAsConst(timeseries)) {
                parseOneElement(ref.toObject(), hourlyForecast);
            }
        }
    }

    if (hasTimezone && hasSunrise)
        applySunriseToForecast();
    // Q_EMIT finished();
}

void PendingWeatherForecastPrivate::parseOneElement(
    const QJsonObject &obj,
    std::vector<HourlyWeatherForecast> &hourlyForecast)
{
    /*~~~~~~~~~~ lambda ~~~~~~~~~~~*/

    auto getWindDeg = [](double deg) -> WindDirection {
        if (deg < 22.5 || deg >= 337.5) {
            return WindDirection::S; // from N
        } else if (deg > 22.5 || deg <= 67.5) {
            return WindDirection::SW; // from NE
        } else if (deg > 67.5 || deg <= 112.5) {
            return WindDirection::W; // from E
        } else if (deg > 112.5 || deg <= 157.5) {
            return WindDirection::NW; // from SE
        } else if (deg > 157.5 || deg <= 202.5) {
            return WindDirection::N; // from S
        } else if (deg > 202.5 || deg <= 247.5) {
            return WindDirection::NE; // from SW
        } else if (deg > 247.5 || deg <= 292.5) {
            return WindDirection::E; // from W
        } else if (deg > 292.5 || deg <= 337.5) {
            return WindDirection::SE; // from NW
        }
        return WindDirection::N;
    };

    /*================== actual code ======================*/

    QJsonObject data = obj[QStringLiteral("data")].toObject(),
                instant = data[QStringLiteral("instant")]
                              .toObject()[QStringLiteral("details")]
                              .toObject();
    // ignore last forecast, which does not have enough data
    if (!data.contains(QStringLiteral("next_6_hours")) &&
        !data.contains(QStringLiteral("next_1_hours")))
        return;

    // get symbolCode and precipitation amount
    QString symbolCode;
    double precipitationAmount = 0;
    // some fields contain only "next_1_hours", and others may contain only
    // "next_6_hours"
    if (data.contains(QStringLiteral("next_1_hours"))) {
        QJsonObject nextOneHours =
            data[QStringLiteral("next_1_hours")].toObject();
        symbolCode = nextOneHours[QStringLiteral("summary")]
                         .toObject()[QStringLiteral("symbol_code")]
                         .toString(QStringLiteral("unknown"));
        precipitationAmount =
            nextOneHours[QStringLiteral("details")]
                .toObject()[QStringLiteral("precipitation_amount")]
                .toDouble();
    } else {
        QJsonObject nextSixHours =
            data[QStringLiteral("next_6_hours")].toObject();
        symbolCode = nextSixHours[QStringLiteral("summary")]
                         .toObject()[QStringLiteral("symbol_code")]
                         .toString(QStringLiteral("unknown"));
        precipitationAmount =
            nextSixHours[QStringLiteral("details")]
                .toObject()[QStringLiteral("precipitation_amount")]
                .toDouble();
    }

    symbolCode = symbolCode.split(QLatin1Char(
        '_'))[0]; // trim _[day/night] from end -
                  // https://api.met.no/weatherapi/weathericon/2.0/legends
    HourlyWeatherForecast hourForecast(QDateTime::fromString(
        obj.value(QStringLiteral("time")).toString(), Qt::ISODate));
    hourForecast.setNeutralWeatherIcon(
        apiDescMap[symbolCode + QStringLiteral("_neutral")].icon);
    hourForecast.setTemperature(
        instant[QStringLiteral("air_temperature")].toDouble());
    hourForecast.setPressure(
        instant[QStringLiteral("air_pressure_at_sea_level")].toDouble());
    hourForecast.setWindDirection(
        getWindDeg(instant[QStringLiteral("wind_from_direction")].toDouble()));
    hourForecast.setWindSpeed(instant[QStringLiteral("wind_speed")].toDouble());
    hourForecast.setHumidity(
        instant[QStringLiteral("relative_humidity")].toDouble());
    hourForecast.setFog(
        instant[QStringLiteral("fog_area_fraction")].toDouble());
    hourForecast.setUvIndex(
        instant[QStringLiteral("ultraviolet_index_clear_sky")].toDouble());
    hourForecast.setPrecipitationAmount(precipitationAmount);
    hourForecast.setSymbolCode(symbolCode);
    hourlyForecast.push_back(std::move(hourForecast));
}

void PendingWeatherForecastPrivate::applySunriseToForecast()
{
    // ************* Lambda *************** //
    auto isDayTime = [](const QDateTime &date,
                        const std::vector<Sunrise> &sunrise) {
        for (auto &sr : sunrise) {
            // if on the same day
            if (sr.sunRise().date().daysTo(date.date()) == 0 &&
                sr.sunRise().date().day() == date.date().day()) {
                // 30 min threshold
                return sr.sunRise().addSecs(-1800) <= date &&
                    sr.sunSet().addSecs(1800) >= date;
            }
        }

        // not found
        return date.time().hour() >= 6 && date.time().hour() <= 18;
    };

    auto getSymbolCodeDescription = [](bool isDay, const QString &symbolCode) {
        return isDay ? apiDescMap[symbolCode + QStringLiteral("_day")].desc
                     : apiDescMap[symbolCode + QStringLiteral("_night")].desc;
    };

    auto getSymbolCodeIcon = [](bool isDay, const QString &symbolCode) {
        return isDay ? apiDescMap[symbolCode + QStringLiteral("_day")].icon
                     : apiDescMap[symbolCode + QStringLiteral("_night")].icon;
    };

    // ******* code ******** //
    for (auto &hourForecast : hourlyForecast) {
        hourForecast.setDate(
            hourForecast.date().toTimeZone(QTimeZone(m_timezone.toUtf8())));

        bool isDay;
        isDay = isDayTime(hourForecast.date(), m_sunriseSource->value());
        hourForecast.setWeatherIcon(getSymbolCodeIcon(
            isDay, hourForecast.symbolCode())); // set day/night icon
        hourForecast.setWeatherDescription(
            getSymbolCodeDescription(isDay, hourForecast.symbolCode()));
        *forecast += std::move(hourForecast);
    }
    forecast->setSunriseForecast(m_sunriseSource->value());
    Q_EMIT finished();

    // save to cache
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
             QStringLiteral("/cache/") + toFixedString(m_latitude) +
             QStringLiteral("/") + toFixedString(m_longitude));
    dir.mkpath(QStringLiteral("."));

    QFile file(dir.path() + QStringLiteral("/cache.json"));

    if (file.open(QIODevice::WriteOnly)) {
        file.write(
            QJsonDocument(forecast->toJson()).toJson(QJsonDocument::Compact));
    } else
        qWarning() << "write to cache failed";
}

PendingWeatherForecast::PendingWeatherForecast(
    double latitude,
    double longitude,
    QNetworkReply *reply,
    const QString &timezone,
    const std::vector<Sunrise> &sunrise)
    : d(new PendingWeatherForecastPrivate(latitude,
                                          longitude,
                                          timezone,
                                          reply,
                                          sunrise,
                                          this))
{
}
PendingWeatherForecast::PendingWeatherForecast(
    QExplicitlySharedDataPointer<WeatherForecast> data)
    : d(new PendingWeatherForecastPrivate(data))
{
}
bool PendingWeatherForecast::isFinished() const
{
    return d->isFinished;
}

QExplicitlySharedDataPointer<WeatherForecast>
PendingWeatherForecast::value() const
{
    return d->forecast;
}
}
