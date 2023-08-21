/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "geotimezone.h"
#include "reply_p.h"

#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

namespace KWeatherCore
{
class GeoTimezonePrivate : public ReplyPrivate
{
public:
    QString m_timezone;
};

GeoTimezone::GeoTimezone(QNetworkAccessManager *nam, double lat, double lon, QObject *parent)
    : Reply(new GeoTimezonePrivate, parent)
{
    Q_D(GeoTimezone);
    QUrl url(QStringLiteral("http://api.geonames.org/timezoneJSON"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), QString::number(lat));
    query.addQueryItem(QStringLiteral("lng"), QString::number(lon));
    query.addQueryItem(QStringLiteral("username"), QStringLiteral("kweatherdev"));
    url.setQuery(query);

    QNetworkRequest req(url);
    auto reply = nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        Q_D(GeoTimezone);
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            d->setError(Reply::NetworkError, reply->errorString());
        } else {
            const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            // if our api calls reached daily limit
            if (doc[QLatin1String("status")][QLatin1String("value")].toInt() == 18) {
                d->setError(Reply::RateLimitExceeded);
                qWarning() << "api calls reached daily limit";
            } else {
                d->m_timezone = doc[QLatin1String("timezoneId")].toString();
            }
        }
        Q_EMIT finished();
    });
}

QString GeoTimezone::timezone() const
{
    Q_D(const GeoTimezone);
    return d->m_timezone;
}
}
