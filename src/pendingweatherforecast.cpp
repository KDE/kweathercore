/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "pendingweatherforecast.h"
#include "geotimezone.h"
#include "kweathercore_p.h"
#include "kweathercore_version.h"
#include "pendingweatherforecast_p.h"

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
    auto timezoneSource = new GeoTimezone(m_manager, latitude, longitude, q);
    QObject::connect(timezoneSource, &GeoTimezone::finished, q, [this, timezoneSource]() {
        timezoneSource->deleteLater();
        parseTimezoneResult(timezoneSource->timezone());
    });
}
void PendingWeatherForecastPrivate::parseTimezoneResult(const QString &result)
{
    hasTimezone = true;
    parser.forecast.setTimezone(result);
    m_timezone = result;
    if (parser.hasData()) {
        parser.applySunriseToForecast(QTimeZone(m_timezone.toUtf8()));
        Q_EMIT q->finished();
    }
}

void PendingWeatherForecastPrivate::parseWeatherForecastResults(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "network error when fetching forecast:" << reply->errorString();
        setError(PendingWeatherForecast::NetworkError, reply->errorString());
        Q_EMIT q->finished();
        return;
    }

    parser.parseLocationForecast(reply->readAll());
    if (hasTimezone) {
        parser.applySunriseToForecast(QTimeZone(m_timezone.toUtf8()));
        Q_EMIT q->finished();
    }
}

PendingWeatherForecast::PendingWeatherForecast(double latitude, double longitude, const QString &timezone, QNetworkAccessManager *nam, QObject *parent)
    : Reply(new PendingWeatherForecastPrivate(this), parent)
{
    Q_D(PendingWeatherForecast);
    d->m_manager = nam;

    // query weather api
    QUrl url(QStringLiteral("https://api.met.no/weatherapi/locationforecast/2.0/complete"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), KWeatherCorePrivate::toFixedString(latitude));
    query.addQueryItem(QStringLiteral("lon"), KWeatherCorePrivate::toFixedString(longitude));
    url.setQuery(query);
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    // see §Identification on https://api.met.no/conditions_service.html
    req.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("KWeatherCore/" KWEATHERCORE_VERSION_STRING " kde-frameworks-devel@kde.org"));
    auto reply = d->m_manager->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        Q_D(PendingWeatherForecast);
        d->parseWeatherForecastResults(reply);
    });

    d->parser.forecast.setCoordinate(latitude, longitude);

    if (timezone.isEmpty()) {
        d->hasTimezone = false;
        d->getTimezone(latitude, longitude);
    } else {
        d->hasTimezone = true;
        d->parser.forecast.setTimezone(timezone);
        d->m_timezone = timezone;
    }
}
PendingWeatherForecast::PendingWeatherForecast(WeatherForecast data, QObject *parent)
    : Reply(new PendingWeatherForecastPrivate(this), parent)
{
    Q_D(PendingWeatherForecast);
    d->parser.forecast = data;
    QMetaObject::invokeMethod(this, &PendingWeatherForecast::finished, Qt::QueuedConnection);
}

PendingWeatherForecast::~PendingWeatherForecast() = default;

WeatherForecast PendingWeatherForecast::value() const
{
    Q_D(const PendingWeatherForecast);
    return d->parser.forecast;
}
}

#include "moc_pendingweatherforecast.cpp"
