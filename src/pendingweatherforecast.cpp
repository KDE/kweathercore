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

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QTimeZone>
#include <QUrlQuery>

namespace KWeatherCore
{

PendingWeatherForecastPrivate::PendingWeatherForecastPrivate(PendingWeatherForecast *qq)
    : q(qq)
{
}

void PendingWeatherForecastPrivate::getTimezone(double latitude, double longitude)
{
    auto timezoneSource = new GeoTimezone(latitude, longitude, q);
    QObject::connect(timezoneSource, &GeoTimezone::finished, q, [this](const QString &tz) {
        parseTimezoneResult(tz);
    });
}
void PendingWeatherForecastPrivate::parseTimezoneResult(const QString &result)
{
    hasTimezone = true;
    forecast.setTimezone(result);
    m_timezone = result;
    getSunrise();
}

void PendingWeatherForecastPrivate::getSunrise()
{
    QObject::connect(m_sunriseSource, &SunriseSource::finished, q, [this]() {
        parseSunriseResults();
    });
    m_sunriseSource->setTimezone(m_timezone);
    m_sunriseSource->requestData();
}
void PendingWeatherForecastPrivate::parseSunriseResults()
{
    hasSunrise = true;

    // if this arrived later than forecast
    if (!hourlyForecast.empty()) {
        applySunriseToForecast();
    }
}
void PendingWeatherForecastPrivate::parseWeatherForecastResults(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        qWarning() << "network error when fetching forecast:" << reply->errorString();
        Q_EMIT q->networkError();
        return;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    if (jsonDocument.isObject()) {
        QJsonObject obj = jsonDocument.object();
        QJsonObject prop = obj[QStringLiteral("properties")].toObject();

        if (prop.contains(QStringLiteral("timeseries"))) {
            QJsonArray timeseries = prop[QStringLiteral("timeseries")].toArray();

            // loop over all forecast data
            for (const auto &ref : qAsConst(timeseries)) {
                parseOneElement(ref.toObject(), hourlyForecast);
            }
        }
    }

    if (hasTimezone && hasSunrise) {
        applySunriseToForecast();
    }
    // Q_EMIT finished();
}

void PendingWeatherForecastPrivate::parseOneElement(const QJsonObject &obj, std::vector<HourlyWeatherForecast> &hourlyForecast)
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

    QJsonObject data = obj[QStringLiteral("data")].toObject();
    QJsonObject instant = data[QStringLiteral("instant")].toObject()[QStringLiteral("details")].toObject();
    // ignore last forecast, which does not have enough data
    if (!data.contains(QStringLiteral("next_6_hours")) && !data.contains(QStringLiteral("next_1_hours"))) {
        return;
    }

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
    HourlyWeatherForecast hourForecast(QDateTime::fromString(obj.value(QStringLiteral("time")).toString(), Qt::ISODate));
    hourForecast.setNeutralWeatherIcon(KWeatherCorePrivate::resolveAPIWeatherDesc(symbolCode + QStringLiteral("_neutral")).icon);
    hourForecast.setTemperature(instant[QStringLiteral("air_temperature")].toDouble());
    hourForecast.setPressure(instant[QStringLiteral("air_pressure_at_sea_level")].toDouble());
    hourForecast.setWindDirection(getWindDeg(instant[QStringLiteral("wind_from_direction")].toDouble()));
    hourForecast.setWindSpeed(instant[QStringLiteral("wind_speed")].toDouble());
    hourForecast.setHumidity(instant[QStringLiteral("relative_humidity")].toDouble());
    hourForecast.setFog(instant[QStringLiteral("fog_area_fraction")].toDouble());
    hourForecast.setUvIndex(instant[QStringLiteral("ultraviolet_index_clear_sky")].toDouble());
    hourForecast.setPrecipitationAmount(precipitationAmount);
    hourForecast.setSymbolCode(symbolCode);
    hourlyForecast.push_back(std::move(hourForecast));
}

void PendingWeatherForecastPrivate::applySunriseToForecast()
{
    // ************* Lambda *************** //
    auto isDayTime = [](const QDateTime &date, const std::vector<Sunrise> &sunrise) {
        for (auto &sr : sunrise) {
            // if on the same day
            if (sr.sunRise().date().daysTo(date.date()) == 0 && sr.sunRise().date().day() == date.date().day()) {
                // 30 min threshold
                return sr.sunRise().addSecs(-1800) <= date && sr.sunSet().addSecs(1800) >= date;
            }
        }

        // not found
        return date.time().hour() >= 6 && date.time().hour() <= 18;
    };

    auto getSymbolCodeDescription = [](bool isDay, const QString &symbolCode) {
        return isDay ? KWeatherCorePrivate::resolveAPIWeatherDesc(symbolCode + QStringLiteral("_day")).desc
                     : KWeatherCorePrivate::resolveAPIWeatherDesc(symbolCode + QStringLiteral("_night")).desc;
    };

    auto getSymbolCodeIcon = [](bool isDay, const QString &symbolCode) {
        return isDay ? KWeatherCorePrivate::resolveAPIWeatherDesc(symbolCode + QStringLiteral("_day")).icon
                     : KWeatherCorePrivate::resolveAPIWeatherDesc(symbolCode + QStringLiteral("_night")).icon;
    };

    // ******* code ******** //
    for (auto &hourForecast : hourlyForecast) {
        hourForecast.setDate(hourForecast.date().toTimeZone(QTimeZone(m_timezone.toUtf8())));

        bool isDay;
        isDay = isDayTime(hourForecast.date(), m_sunriseSource->value());
        hourForecast.setWeatherIcon(getSymbolCodeIcon(isDay, hourForecast.symbolCode())); // set day/night icon
        hourForecast.setWeatherDescription(getSymbolCodeDescription(isDay, hourForecast.symbolCode()));
        forecast += std::move(hourForecast);
    }
    forecast.setSunriseForecast(m_sunriseSource->value());
    Q_EMIT q->finished();

    // save to cache

    QFile file(KWeatherCorePrivate::getCacheDirectory(m_latitude, m_longitude).path() + QStringLiteral("/cache.json"));

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(forecast.toJson()).toJson(QJsonDocument::Compact));
    } else {
        qWarning() << "write to cache failed";
    }
}

PendingWeatherForecast::PendingWeatherForecast(double latitude,
                                               double longitude,
                                               const QString &timezone,
                                               const std::vector<Sunrise> &sunrise,
                                               QNetworkAccessManager *nam)
    : d(new PendingWeatherForecastPrivate(this))
{
    d->m_manager = nam;
    connect(this, &PendingWeatherForecast::finished, this, [this] {
        d->isFinished = true;
    });

    // query weather api
    QUrl url(QStringLiteral("https://api.met.no/weatherapi/locationforecast/2.0/complete"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), KWeatherCorePrivate::toFixedString(latitude));
    query.addQueryItem(QStringLiteral("lon"), KWeatherCorePrivate::toFixedString(longitude));
    url.setQuery(query);
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    // see §Identification on https://api.met.no/conditions_service.html
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  QString(QStringLiteral("KWeatherCore/") + VERSION_NUMBER + QStringLiteral(" kde-frameworks-devel@kde.org")));
    auto reply = d->m_manager->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        d->parseWeatherForecastResults(reply);
    });

    d->forecast.setCoordinate(latitude, longitude);

    d->m_sunriseSource = new SunriseSource(latitude, longitude, d->m_timezone, sunrise, this);
    if (timezone.isEmpty()) {
        d->hasTimezone = false;
        d->getTimezone(latitude, longitude);
    } else {
        d->hasTimezone = true;
        d->forecast.setTimezone(timezone);
        d->m_timezone = timezone;
        d->getSunrise();
    }
}
PendingWeatherForecast::PendingWeatherForecast(WeatherForecast data)
    : d(new PendingWeatherForecastPrivate(this))
{
    d->forecast = data;
    d->isFinished = true;
}

PendingWeatherForecast::~PendingWeatherForecast() = default;

bool PendingWeatherForecast::isFinished() const
{
    return d->isFinished;
}

WeatherForecast PendingWeatherForecast::value() const
{
    return d->forecast;
}
}
