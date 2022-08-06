/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kweathercore_p.h"
#include "metnoparser_p.h"

#include <KHolidays/SunRiseSet>
#include <kholidays_version.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace KWeatherCore;

void MetNoParser::parseLocationForecast(const QByteArray &data)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data);

    if (jsonDocument.isObject()) {
        QJsonObject obj = jsonDocument.object();
        QJsonObject prop = obj[QStringLiteral("properties")].toObject();

        if (prop.contains(QStringLiteral("timeseries"))) {
            QJsonArray timeseries = prop[QStringLiteral("timeseries")].toArray();

            // loop over all forecast data
            for (const auto &ref : qAsConst(timeseries)) {
                parseOneElement(ref.toObject());
            }
        }
    }
}

void MetNoParser::parseOneElement(const QJsonObject &obj)
{
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
    hourForecast.setWindDirectionDegree(instant[QStringLiteral("wind_from_direction")].toDouble());
    hourForecast.setWindSpeed(instant[QStringLiteral("wind_speed")].toDouble());
    hourForecast.setHumidity(instant[QStringLiteral("relative_humidity")].toDouble());
    hourForecast.setFog(instant[QStringLiteral("fog_area_fraction")].toDouble());
    hourForecast.setUvIndex(instant[QStringLiteral("ultraviolet_index_clear_sky")].toDouble());
    hourForecast.setPrecipitationAmount(precipitationAmount);
    hourForecast.setSymbolCode(symbolCode);
    hourlyForecast.push_back(std::move(hourForecast));
}

bool MetNoParser::isDayTime(const QDateTime &dt) const
{
    const auto sunriseTime = KHolidays::SunRiseSet::utcSunrise(dt.date(), forecast.latitude(), forecast.longitude());
    const auto sunsetTime = KHolidays::SunRiseSet::utcSunset(dt.date(), forecast.latitude(), forecast.longitude());

#if KHOLIDAYS_VERSION >= QT_VERSION_CHECK(5, 97, 0)
    // polar day/night: there is no sunrise/sunset
    if (!sunriseTime.isValid() || !sunsetTime.isValid()) {
        return KHolidays::SunRiseSet::isPolarDay(dt.date(), forecast.latitude());
    }
#endif

    auto sunrise = QDateTime(dt.date(), sunriseTime, Qt::UTC);
    auto sunset = QDateTime(dt.date(), sunsetTime, Qt::UTC);

    // sunset before sunrise means the sunset actually happens the next day
    if (dt >= sunrise && sunset < sunrise) {
        sunset = sunset.addDays(1);
    } else if (dt < sunrise && sunset < sunrise) {
        sunrise = sunrise.addDays(-1);
    }

    // 30 min threshold
    return sunrise.addSecs(-1800) <= dt && sunset.addSecs(1800) >= dt;
}

void MetNoParser::applySunriseToForecast(const QTimeZone &timezone)
{
    // ************* Lambda *************** //
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
        hourForecast.setDate(hourForecast.date().toTimeZone(timezone));

        bool isDay;
        isDay = isDayTime(hourForecast.date());
        hourForecast.setWeatherIcon(getSymbolCodeIcon(isDay, hourForecast.symbolCode())); // set day/night icon
        hourForecast.setWeatherDescription(getSymbolCodeDescription(isDay, hourForecast.symbolCode()));
        forecast += std::move(hourForecast);
    }

    // save to cache
    QFile file(KWeatherCorePrivate::getCacheDirectory(forecast.latitude(), forecast.longitude()).path() + QStringLiteral("/cache.json"));

    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(forecast.toJson()).toJson(QJsonDocument::Compact));
    } else {
        qWarning() << "write to cache failed";
    }
}
