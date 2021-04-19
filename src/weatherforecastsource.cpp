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
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QUrlQuery>
namespace KWeatherCore
{
class WeatherForecastSourcePrivate : public QObject
{
    Q_OBJECT
public:
    WeatherForecastSourcePrivate(QObject *parent = nullptr);
    std::pair<PendingWeatherForecast *, bool>
    requestData(double latitude,
                double longitude,
                QString timezone = QString(),
                const std::vector<Sunrise> &sunrise = std::vector<Sunrise>());

private:
    QNetworkAccessManager *manager = nullptr;
};
WeatherForecastSourcePrivate::WeatherForecastSourcePrivate(QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}
std::pair<PendingWeatherForecast *, bool>
WeatherForecastSourcePrivate::requestData(double latitude,
                                          double longitude,
                                          QString timezone,
                                          const std::vector<Sunrise> &sunrise)
{
    QFile cache(
        QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
        QStringLiteral("/cache/") + toFixedString(latitude) +
        QStringLiteral("/") + toFixedString(longitude) +
        QStringLiteral("/cache.json"));

    // valid cache
    if (cache.exists() && cache.open(QIODevice::ReadOnly)) {
        auto weatherforecast = WeatherForecast::fromJson(
            QJsonDocument::fromJson(cache.readAll()).object());

        if (weatherforecast->createdTime().secsTo(
                QDateTime::currentDateTime()) <= 3600)
            return {new PendingWeatherForecast(weatherforecast), true};
    }

    // query weather api
    QUrl url(QStringLiteral(
        "https://api.met.no/weatherapi/locationforecast/2.0/complete"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), toFixedString(latitude));
    query.addQueryItem(QStringLiteral("lon"), toFixedString(longitude));

    url.setQuery(query);

    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                     QNetworkRequest::NoLessSafeRedirectPolicy);

    // see §Identification on https://api.met.no/conditions_service.html
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  QString(QStringLiteral("KWeatherCore/") + VERSION_NUMBER +
                          QStringLiteral(" kde-frameworks-devel@kde.org")));

    auto reply = manager->get(req);
    return {new PendingWeatherForecast(
                latitude, longitude, reply, timezone, sunrise),
            false};
}
WeatherForecastSource::WeatherForecastSource(QObject *parent)
    : QObject(parent)
    , d(new WeatherForecastSourcePrivate(this))
{
    // create cache location if it does not exist, and load cache
    QDir().mkpath(
        QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
        QStringLiteral("/cache/"));
}
std::pair<PendingWeatherForecast *, bool>
WeatherForecastSource::requestData(double latitude,
                                   double longitude,
                                   QString timezone,
                                   const std::vector<Sunrise> &sunrise)
{
    return d->requestData(latitude, longitude, std::move(timezone), sunrise);
}
std::pair<PendingWeatherForecast *, bool> WeatherForecastSource::requestData(
    const KWeatherCore::LocationQueryResult &result)
{
    return requestData(result.latitude(), result.longitude());
}
}
#include "weatherforecastsource.moc"
