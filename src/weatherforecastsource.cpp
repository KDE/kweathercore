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
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
namespace KWeatherCore
{
class WeatherForecastSourcePrivate : public QObject
{
    Q_OBJECT
public:
    WeatherForecastSourcePrivate(QObject *parent = nullptr);
    PendingWeatherForecast *
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
PendingWeatherForecast *
WeatherForecastSourcePrivate::requestData(double latitude,
                                          double longitude,
                                          QString timezone,
                                          const std::vector<Sunrise> &sunrise)
{
    // query weather api
    QUrl url(QStringLiteral(
        "https://api.met.no/weatherapi/locationforecast/2.0/complete"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"),
                       QString::number(latitude, 'g', 2));
    query.addQueryItem(QStringLiteral("lon"),
                       QString::number(longitude, 'g', 2));

    url.setQuery(query);

    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                     QNetworkRequest::NoLessSafeRedirectPolicy);

    // see §Identification on https://api.met.no/conditions_service.html
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  QString(QStringLiteral("KWeatherCore/") +
                          VERSION_NUMBER +
                          QStringLiteral(" kde-frameworks-devel@kde.org")));

    auto reply = manager->get(req);
    return new PendingWeatherForecast(
        latitude, longitude, reply, timezone, sunrise);
}
WeatherForecastSource::WeatherForecastSource(QObject *parent)
    : QObject(parent)
    , d(new WeatherForecastSourcePrivate(this))
{
}
PendingWeatherForecast *
WeatherForecastSource::requestData(double latitude,
                                   double longitude,
                                   QString timezone,
                                   const std::vector<Sunrise> &sunrise)
{
    return d->requestData(latitude, longitude, std::move(timezone), sunrise);
}
PendingWeatherForecast *WeatherForecastSource::requestData(
    const KWeatherCore::LocationQueryResult &result)
{
    return requestData(result.latitude(), result.longitude());
}
}
#include "weatherforecastsource.moc"
