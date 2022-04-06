/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "weatherforecastsource.h"
#include "kweathercore_p.h"
#include "locationqueryresult.h"
#include "weatherforecast.h"

#include <QFile>
#include <QJsonDocument>
#include <QStandardPaths>

#include <algorithm>

namespace KWeatherCore
{
class WeatherForecastSourcePrivate
{
public:
};

WeatherForecastSource::WeatherForecastSource(QObject *parent)
    : QObject(parent)
    , d(new WeatherForecastSourcePrivate)
{
}

WeatherForecastSource::~WeatherForecastSource() = default;

PendingWeatherForecast *WeatherForecastSource::requestData(double latitude, double longitude)
{
    QFile cache(self()->getCacheDirectory(latitude, longitude).path() + QStringLiteral("/cache.json"));
    std::vector<Sunrise> sunriseCache;
    QString timezone;
    // valid cache
    if (cache.exists() && cache.open(QIODevice::ReadOnly)) {
        auto weatherforecast = WeatherForecast::fromJson(QJsonDocument::fromJson(cache.readAll()).object());
        timezone = weatherforecast.timezone();
        if (weatherforecast.createdTime().secsTo(QDateTime::currentDateTime()) <= 3600) {
            return new PendingWeatherForecast(weatherforecast);
        } else {
            const auto &days = weatherforecast.dailyWeatherForecast();
            auto it = std::lower_bound(days.begin(), days.end(), QDate::currentDate(), [](const DailyWeatherForecast &day, const QDate &date) {
                return day.date() < date;
            });

            auto size = std::distance(it, days.end());
            if (size) {
                sunriseCache.reserve(size);
                while (it != days.end()) {
                    sunriseCache.push_back(it->sunrise());
                    it++;
                }
            }
        }
    }

    return new PendingWeatherForecast(latitude, longitude, timezone, sunriseCache);
}

PendingWeatherForecast *WeatherForecastSource::requestData(const KWeatherCore::LocationQueryResult &result)
{
    return requestData(result.latitude(), result.longitude());
}
}
