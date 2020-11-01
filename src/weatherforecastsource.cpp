/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "weatherforecastsource.h"
#include "geotimezone.h"
#include "pendingweatherforecast_p.h"
#include "weatherforecast.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
namespace KWeatherCore
{
class WeatherForecastSourcePrivate
{
public:
    QNetworkAccessManager *manager = nullptr;
};

WeatherForecastSource::WeatherForecastSource(QObject *parent)
    : QObject(parent)
    , d(new WeatherForecastSourcePrivate)
{
    d->manager = new QNetworkAccessManager(this);
}
WeatherForecastSource::~WeatherForecastSource()
{
    delete d;
}
PendingWeatherForecast *WeatherForecastSource::requestData(double latitude, double longitude, QString timezone, const QVector<Sunrise> &sunrise)
{
    auto pf = new PendingWeatherForecastPrivate(latitude, longitude, timezone, sunrise);
    // query weather api
    QUrl url(QStringLiteral("https://api.met.no/weatherapi/locationforecast/2.0/complete"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), QString::number(latitude));
    query.addQueryItem(QStringLiteral("lon"), QString::number(longitude));

    url.setQuery(query);

    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    // see §Identification on https://api.met.no/conditions_service.html
    req.setHeader(QNetworkRequest::UserAgentHeader, QString(QCoreApplication::applicationName() + QLatin1Char(' ') + QCoreApplication::applicationVersion() + QStringLiteral(" (kde-pim@kde.org)")));

    auto reply = d->manager->get(req);
    connect(reply, &QNetworkReply::finished, [pf, reply] { pf->parseWeatherForecastResults(reply); });

    return new PendingWeatherForecast(pf);
}
PendingWeatherForecast *WeatherForecastSource::requestData(double latitude, double longitude, QString timezone, QVector<Sunrise> &&sunrise)
{
    auto pf = new PendingWeatherForecastPrivate(latitude, longitude, timezone, std::move(sunrise));
    // query weather api
    QUrl url(QStringLiteral("https://api.met.no/weatherapi/locationforecast/2.0/complete"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), QString::number(latitude));
    query.addQueryItem(QStringLiteral("lon"), QString::number(longitude));

    url.setQuery(query);

    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    // see §Identification on https://api.met.no/conditions_service.html
    req.setHeader(QNetworkRequest::UserAgentHeader, QString(QCoreApplication::applicationName() + QLatin1Char(' ') + QCoreApplication::applicationVersion() + QStringLiteral(" (kde-pim@kde.org)")));

    auto reply = d->manager->get(req);
    connect(reply, &QNetworkReply::finished, pf, [pf, reply] { pf->parseWeatherForecastResults(reply); });

    return new PendingWeatherForecast(pf);
}
}
