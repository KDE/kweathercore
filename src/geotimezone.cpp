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
class GeoTimezonePrivate : public QObject
{
    Q_OBJECT
public:
    GeoTimezonePrivate(double lat, double lon, GeoTimezone *parent);
Q_SIGNALS:
    void finished(const QString &timezone);
    void networkError();
private Q_SLOTS:
    void downloadFinished(QNetworkReply *reply);
    QNetworkAccessManager *m_manager = nullptr;
};
GeoTimezonePrivate::GeoTimezonePrivate(double lat,
                                       double lon,
                                       GeoTimezone *parent)
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
    query.addQueryItem(QStringLiteral("username"),
                       QStringLiteral("kweatherdev"));
    url.setQuery(query);
    QNetworkRequest req(url);

    connect(m_manager,
            &QNetworkAccessManager::finished,
            this,
            &GeoTimezonePrivate::downloadFinished);
    connect(
        this, &GeoTimezonePrivate::finished, parent, &GeoTimezone::finished);
    connect(this,
            &GeoTimezonePrivate::networkError,
            parent,
            &GeoTimezone::networkError);
    m_manager->get(req);
}
GeoTimezone::GeoTimezone(double lat, double lon, QObject *parent)
    : QObject(parent)
    , d(new GeoTimezonePrivate(lat, lon, this))
{
}
void GeoTimezonePrivate::downloadFinished(QNetworkReply *reply)
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
#include "geotimezone.moc"
