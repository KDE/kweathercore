/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "geotimezone.h"
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
namespace KWeatherCore
{
GeoTimezone::GeoTimezone(double lat, double lon, QObject *parent)
    : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);

    m_manager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
    m_manager->setStrictTransportSecurityEnabled(true);
    m_manager->enableStrictTransportSecurityStore(true);

    QUrl url(QStringLiteral("http://api.geonames.org/timezoneJSON"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), QString::number(lat));
    query.addQueryItem(QStringLiteral("lng"), QString::number(lon));
    query.addQueryItem(QStringLiteral("username"), QStringLiteral("kweatherdev"));
    url.setQuery(query);
    QNetworkRequest req(url);

    connect(m_manager, &QNetworkAccessManager::finished, this, &GeoTimezone::downloadFinished);
    m_manager->get(req);
}

void GeoTimezone::downloadFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        Q_EMIT networkError();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    // if our api calls reached daily limit
    if (doc[QStringLiteral("status")][QStringLiteral("value")].toInt() == 18) {
        qWarning() << "api calls reached daily limit";
        return;
    }
    Q_EMIT finished(doc[QStringLiteral("timezoneId")].toString());
}
}
