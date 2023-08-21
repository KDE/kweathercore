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
    const QJsonDocument jsonDocument = QJsonDocument::fromJson(data);

    if (jsonDocument.isObject()) {
        const QJsonObject obj = jsonDocument.object();
        const QJsonObject prop = obj[QLatin1String("properties")].toObject();

        if (prop.contains(QLatin1String("timeseries"))) {
            const QJsonArray timeseries = prop[QLatin1String("timeseries")].toArray();

            // loop over all forecast data
            for (const auto &ref : qAsConst(timeseries)) {
                parseOneElement(ref.toObject());
            }
        }
    }
}

void MetNoParser::parseOneElement(const QJsonObject &obj)
{
    const QJsonObject data = obj[QLatin1String("data")].toObject();
    const QJsonObject instant = data[QLatin1String("instant")].toObject()[QLatin1String("details")].toObject();
    // ignore last forecast, which does not have enough data
    if (!data.contains(QLatin1String("next_6_hours")) && !data.contains(QLatin1String("next_1_hours"))) {
        return;
    }

    // get symbolCode and precipitation amount
    QString symbolCode;
    double precipitationAmount = 0;
    // some fields contain only "next_1_hours", and others may contain only
    // "next_6_hours"
    if (data.contains(QLatin1String("next_1_hours"))) {
        const QJsonObject nextOneHours = data[QLatin1String("next_1_hours")].toObject();
        symbolCode = nextOneHours[QLatin1String("summary")].toObject()[QLatin1String("symbol_code")].toString(QLatin1String("unknown"));
        precipitationAmount = nextOneHours[QLatin1String("details")].toObject()[QLatin1String("precipitation_amount")].toDouble();
    } else {
        const QJsonObject nextSixHours = data[QLatin1String("next_6_hours")].toObject();
        symbolCode = nextSixHours[QLatin1String("summary")].toObject()[QLatin1String("symbol_code")].toString(QLatin1String("unknown"));
        precipitationAmount = nextSixHours[QLatin1String("details")].toObject()[QLatin1String("precipitation_amount")].toDouble();
    }

    symbolCode = symbolCode.split(QLatin1Char('_'))[0]; // trim _[day/night] from end -
                                                        // https://api.met.no/weatherapi/weathericon/2.0/legends
    HourlyWeatherForecast hourForecast(QDateTime::fromString(obj.value(QLatin1String("time")).toString(), Qt::ISODate));
    hourForecast.setNeutralWeatherIcon(KWeatherCorePrivate::resolveAPIWeatherDesc(symbolCode + QLatin1String("_neutral")).icon);
    hourForecast.setTemperature(instant[QLatin1String("air_temperature")].toDouble());
    hourForecast.setPressure(instant[QLatin1String("air_pressure_at_sea_level")].toDouble());
    hourForecast.setWindDirectionDegree(instant[QLatin1String("wind_from_direction")].toDouble());
    hourForecast.setWindSpeed(instant[QLatin1String("wind_speed")].toDouble());
    hourForecast.setHumidity(instant[QLatin1String("relative_humidity")].toDouble());
    hourForecast.setFog(instant[QLatin1String("fog_area_fraction")].toDouble());
    hourForecast.setUvIndex(instant[QLatin1String("ultraviolet_index_clear_sky")].toDouble());
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
