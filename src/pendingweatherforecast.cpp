/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "pendingweatherforecast.h"
#include "geotimezone.h"
#include "kweathercore_p.h"
#include "pendingweatherforecast_p.h"
#include "sunrisesource.h"
#include <KLocalizedString>
#include <QExplicitlySharedDataPointer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QTimeZone>
namespace KWeatherCore
{
PendingWeatherForecastPrivate::PendingWeatherForecastPrivate(double latitude, double longitude, const QString &timezone, QVector<Sunrise> &&sunrise)
    : forecast(QExplicitlySharedDataPointer<WeatherForecast>(new WeatherForecast))
    , m_latitude(latitude)
    , m_longitude(longitude)
{
    forecast->setCoordinate(latitude, longitude);
    sunriseVec = std::move(sunrise);
    if (timezone.isEmpty()) {
        hasTimezone = false;
        getTimezone(latitude, longitude);
    } else {
        hasTimezone = true;
        forecast->setTimezone(timezone);
        getSunrise(m_latitude, m_longitude, QDateTime::currentDateTime().toTimeZone(QTimeZone(timezone.toUtf8())).offsetFromUtc());
    }
    if (sunriseVec.size() <= 10) {
        hasSunrise = false;
    }
}

PendingWeatherForecastPrivate::PendingWeatherForecastPrivate(double latitude, double longitude, const QString &timezone, const QVector<Sunrise> &sunrise)
    : forecast(QExplicitlySharedDataPointer<WeatherForecast>(new WeatherForecast))
    , m_latitude(latitude)
    , m_longitude(longitude)
{
    sunriseVec = sunrise;
    if (timezone.isEmpty()) {
        hasTimezone = false;
        getTimezone(latitude, longitude);
    } else {
        hasTimezone = true;
        forecast->setTimezone(timezone);
        getSunrise(m_latitude, m_longitude, QDateTime::currentDateTime().toTimeZone(QTimeZone(timezone.toUtf8())).offsetFromUtc());
    }
    if (sunriseVec.size() <= 10) {
        hasSunrise = false;
    }
}
void PendingWeatherForecastPrivate::getTimezone(double latitude, double longitude)
{
    auto timezoneSource = new GeoTimezone(latitude, longitude, this);
    connect(timezoneSource, &GeoTimezone::finished, this, &PendingWeatherForecastPrivate::parseTimezoneResult);
}
void PendingWeatherForecastPrivate::parseTimezoneResult(QString result)
{
    hasTimezone = true;
    forecast->setTimezone(result);
    if (!hasSunrise)
        getSunrise(m_latitude, m_longitude, QDateTime::currentDateTime().toTimeZone(QTimeZone(result.toUtf8())).offsetFromUtc());
}

void PendingWeatherForecastPrivate::getSunrise(double latitude, double longitude, int offset)
{
    m_sunriseSource = new SunriseSource(latitude, longitude, offset, sunriseVec, this);
    m_sunriseSource->requestData();
    connect(m_sunriseSource, &SunriseSource::finished, this, &PendingWeatherForecastPrivate::parseSunriseResults);
}
void PendingWeatherForecastPrivate::parseSunriseResults()
{
    if (m_sunriseSource) {
        sunriseVec = m_sunriseSource->value();
        hasSunrise = true;
    }

    // if this arrived later than forecast
    if (hourlyForecast.size() > 0)
        applySunriseToForecast();
}
void PendingWeatherForecastPrivate::parseWeatherForecastResults(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        qWarning() << "network error when fetching forecast:" << reply->errorString();
        Q_EMIT networkError();
        return;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    if (jsonDocument.isObject()) {
        QJsonObject obj = jsonDocument.object();
        QJsonObject prop = obj[QStringLiteral("properties")].toObject();

        if (prop.contains(QStringLiteral("timeseries"))) {
            QJsonArray timeseries = prop[QStringLiteral("timeseries")].toArray();

            // loop over all forecast data
            for (QJsonValueRef ref : timeseries) {
                QJsonObject refObj = ref.toObject();
                parseOneElement(refObj, hourlyForecast);
            }
        }
    }

    if (hasTimezone && hasSunrise)
        applySunriseToForecast();
    // Q_EMIT finished();
}

void PendingWeatherForecastPrivate::parseOneElement(const QJsonObject &obj, QVector<HourlyWeatherForecast> &hourlyForecast)
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

    QJsonObject data = obj[QStringLiteral("data")].toObject(), instant = data[QStringLiteral("instant")].toObject()[QStringLiteral("details")].toObject();
    // ignore last forecast, which does not have enough data
    if (!data.contains(QStringLiteral("next_6_hours")) && !data.contains(QStringLiteral("next_1_hours")))
        return;

    // correct date to corresponding timezone of location if possible
    QDateTime date = QDateTime::fromString(obj.value(QStringLiteral("time")).toString(), Qt::ISODate);

    // get symbolCode and precipitation amount
    QString symbolCode;
    double precipitationAmount = 0;
    // some fields contain only "next_1_hours", and others may contain only
    // "next_6_hours"
    if (data.contains(QStringLiteral("next_1_hours"))) {
        QJsonObject nextOneHours = data[QStringLiteral("next_1_hours")].toObject();
        symbolCode = nextOneHours[QStringLiteral("summary")].toObject()[QStringLiteral("symbol_code")].toString(QStringLiteral("unknown"));
        precipitationAmount = nextOneHours[QStringLiteral("details")].toObject()[QStringLiteral("precipitation_amount")].toDouble();
    } else {
        QJsonObject nextSixHours = data[QStringLiteral("next_6_hours")].toObject();
        symbolCode = nextSixHours[QStringLiteral("summary")].toObject()[QStringLiteral("symbol_code")].toString(QStringLiteral("unknown"));
        precipitationAmount = nextSixHours[QStringLiteral("details")].toObject()[QStringLiteral("precipitation_amount")].toDouble();
    }

    symbolCode = symbolCode.split(QLatin1Char('_'))[0]; // trim _[day/night] from end -
                                                        // https://api.met.no/weatherapi/weathericon/2.0/legends
    auto hourForecast = HourlyWeatherForecast(QDateTime::fromString(obj.value(QStringLiteral("time")).toString(), Qt::ISODate),
                                              QString(),
                                              QString(),
                                              apiDescMap[symbolCode + QStringLiteral("_neutral")].icon,
                                              instant[QStringLiteral("air_temperature")].toDouble(),
                                              instant[QStringLiteral("air_pressure_at_sea_level")].toDouble(),
                                              getWindDeg(instant[QStringLiteral("wind_from_direction")].toDouble()),
                                              instant[QStringLiteral("wind_speed")].toDouble(),
                                              instant[QStringLiteral("relative_humidity")].toDouble(),
                                              instant[QStringLiteral("fog_area_fraction")].toDouble(),
                                              instant[QStringLiteral("ultraviolet_index_clear_sky")].toDouble(),
                                              precipitationAmount);
    hourForecast.setSymbolCode(symbolCode);
    hourlyForecast.append(std::move(hourForecast));
}

void PendingWeatherForecastPrivate::applySunriseToForecast()
{
    // ************* Lambda *************** //
    auto isDayTime = [](QDateTime date, const QVector<Sunrise> &sunrise) {
        for (auto sr : sunrise) {
            // if on the same day
            if (sr.sunRise().date().daysTo(date.date()) == 0 && sr.sunRise().date().day() == date.date().day()) {
                // 30 min threshold
                return sr.sunRise().addSecs(-1800) <= date && sr.sunSet().addSecs(1800) >= date;
            }
        }

        // not found
        return date.time().hour() >= 6 && date.time().hour() <= 18;
    };

    auto getSymbolCodeDescription = [](bool isDay, const QString &symbolCode) { return isDay ? apiDescMap[symbolCode + QStringLiteral("_day")].desc : apiDescMap[symbolCode + QStringLiteral("_night")].desc; };

    auto getSymbolCodeIcon = [](bool isDay, const QString &symbolCode) { return isDay ? apiDescMap[symbolCode + QStringLiteral("_day")].icon : apiDescMap[symbolCode + QStringLiteral("_night")].icon; };

    // ******* code ******** //
    for (auto &hourForecast : hourlyForecast) {
        hourForecast.setDate(hourForecast.date().toTimeZone(QTimeZone(m_timezone.toUtf8())));

        bool isDay;
        isDay = isDayTime(hourForecast.date(), sunriseVec);
        hourForecast.setWeatherIcon(getSymbolCodeIcon(isDay, hourForecast.symbolCode())); // set day/night icon
        hourForecast.setWeatherDescription(getSymbolCodeDescription(isDay, hourForecast.symbolCode()));
        *forecast += std::move(hourForecast);
    }
    forecast->setSunriseForecast(std::move(sunriseVec));
    Q_EMIT finished();
}

PendingWeatherForecast::PendingWeatherForecast(double latitude, double longitude, QNetworkReply *reply, const QString &timezone, const QVector<Sunrise> &sunrise)
    : d(new PendingWeatherForecastPrivate(latitude, longitude, timezone, sunrise))
{
    connect(d, &PendingWeatherForecastPrivate::finished, [this] { this->m_finished = true; });
    connect(d, &PendingWeatherForecastPrivate::finished, this, &PendingWeatherForecast::finished);
    connect(d, &PendingWeatherForecastPrivate::networkError, this, &PendingWeatherForecast::networkError);
    if (reply) {
        auto d = this->d; // explicitly capture?
        connect(reply, &QNetworkReply::finished, [d, reply] { d->parseWeatherForecastResults(reply); });
    }
}

QExplicitlySharedDataPointer<WeatherForecast> PendingWeatherForecast::value() const
{
    return d->forecast;
}
PendingWeatherForecast::~PendingWeatherForecast()
{
    if (d)
        delete d;
}
}
