/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "weatherforecastsource.h"
#include "geotimezone.h"
#include "kweathercore_p.h"
#include "pendingweatherforecast_p.h"
#include "weatherforecast.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QUrlQuery>
#include <algorithm>
namespace KWeatherCore
{
class WeatherForecastSourcePrivate : public QObject
{
    Q_OBJECT
public:
    WeatherForecastSourcePrivate(QObject *parent = nullptr);
    PendingWeatherForecast *requestData(double latitude, double longitude);

private:
};
WeatherForecastSourcePrivate::WeatherForecastSourcePrivate(QObject *parent)
    : QObject(parent)
{
}
PendingWeatherForecast *WeatherForecastSourcePrivate::requestData(double latitude, double longitude)
{
    QFile cache(self()->getCacheDirectory(latitude, longitude).path() + QStringLiteral("/cache.json"));
    std::vector<Sunrise> sunriseCache;
    QString timezone;
    // valid cache
    if (cache.exists() && cache.open(QIODevice::ReadOnly)) {
        auto weatherforecast = WeatherForecast::fromJson(QJsonDocument::fromJson(cache.readAll()).object());
        timezone = weatherforecast.timezone();
        if (weatherforecast.createdTime().secsTo(QDateTime::currentDateTime()) <= 3600)
            return new PendingWeatherForecast(weatherforecast);
        else {
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

    // query weather api
    QUrl url(QStringLiteral("https://api.met.no/weatherapi/locationforecast/2.0/complete"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), self()->toFixedString(latitude));
    query.addQueryItem(QStringLiteral("lon"), self()->toFixedString(longitude));

    url.setQuery(query);

    return new PendingWeatherForecast(latitude, longitude, url, timezone, sunriseCache);
}
WeatherForecastSource::WeatherForecastSource(QObject *parent)
    : QObject(parent)
    , d(new WeatherForecastSourcePrivate(this))
{
}
PendingWeatherForecast *WeatherForecastSource::requestData(double latitude, double longitude)
{
    return d->requestData(latitude, longitude);
}
PendingWeatherForecast *WeatherForecastSource::requestData(const KWeatherCore::LocationQueryResult &result)
{
    return requestData(result.latitude(), result.longitude());
}
}
#include "weatherforecastsource.moc"
