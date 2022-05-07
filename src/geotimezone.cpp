/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "geotimezone.h"
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
namespace KWeatherCore
{
GeoTimezone::GeoTimezone(QNetworkAccessManager *nam, double lat, double lon, QObject *parent)
    : QObject(parent)
{
    QUrl url(QStringLiteral("http://api.geonames.org/timezoneJSON"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), QString::number(lat));
    query.addQueryItem(QStringLiteral("lng"), QString::number(lon));
    query.addQueryItem(QStringLiteral("username"), QStringLiteral("kweatherdev"));
    url.setQuery(query);

    QNetworkRequest req(url);
    auto reply = nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        reply->deleteLater();
        if (reply->error()) {
            Q_EMIT networkErrorOccured();
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        // if our api calls reached daily limit
        if (doc[QStringLiteral("status")][QStringLiteral("value")].toInt() == 18) {
            qWarning() << "api calls reached daily limit";
            return;
        }
        Q_EMIT finished(doc[QStringLiteral("timezoneId")].toString());
    });
}
}
