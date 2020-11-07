/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "locationquery.h"
#include <QGeoPositionInfoSource>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
namespace KWeatherCore
{
class LocationQueryPrivate
{
public:
    QGeoPositionInfoSource *locationSource = nullptr;
    QNetworkAccessManager *manager = nullptr;
};
LocationQuery::LocationQuery(QObject *parent)
    : QObject(parent)
    , d(new LocationQueryPrivate)
{
    d->locationSource = QGeoPositionInfoSource::createDefaultSource(this);
    d->locationSource->stopUpdates();
    d->manager = new QNetworkAccessManager(this);

    connect(d->locationSource, &QGeoPositionInfoSource::positionUpdated, this, &LocationQuery::positionUpdated);
}
LocationQuery::~LocationQuery()
{
    delete d;
}

void LocationQuery::query(QString name, int number)
{
    QUrl url(QStringLiteral("http://api.geonames.org/searchJSON"));
    QUrlQuery urlQuery;

    urlQuery.addQueryItem(QStringLiteral("q"), name);
    urlQuery.addQueryItem(QStringLiteral("maxRows"), QString::number(number));
    urlQuery.addQueryItem(QStringLiteral("username"), QStringLiteral("kweatherdev"));
    url.setQuery(urlQuery);
    d->manager->get(QNetworkRequest(url));
    connect(d->manager, &QNetworkAccessManager::finished, this, &LocationQuery::handleQueryResult);
}
void LocationQuery::locate()
{
    d->locationSource->requestUpdate();
}
void LocationQuery::handleQueryResult(QNetworkReply *reply)
{
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject root = document.object();
    reply->deleteLater();

    auto counts = root[QStringLiteral("totalResultsCount")].toInt();
    // if no result
    if (!counts) {
        Q_EMIT queryError();
        return;
    }
    QVector<LocationQueryResult> retVec;

    // if our api calls reached daily limit
    if (root[QStringLiteral("status")].toObject()[QStringLiteral("value")].toInt() == 18) {
        Q_EMIT queryError();
        qWarning("API calls reached daily limit");
        return;
    }
    QJsonArray geonames = root.value(QStringLiteral("geonames")).toArray();
    // add query results
    for (QJsonValueRef resRef : geonames) {
        QJsonObject res = resRef.toObject();
        auto result = LocationQueryResult(res.value(QStringLiteral("lat")).toString().toFloat(),
                                          res.value(QStringLiteral("lng")).toString().toFloat(),
                                          res.value(QStringLiteral("toponymName")).toString(),
                                          res.value(QStringLiteral("name")).toString(),
                                          res.value(QStringLiteral("countryCode")).toString(),
                                          res.value(QStringLiteral("countryName")).toString(),
                                          QString::number(res.value(QStringLiteral("geonameId")).toInt()));
        retVec.append(result);
    }

    Q_EMIT queryFinished(retVec);
}

void LocationQuery::positionUpdated(const QGeoPositionInfo &update)
{
    QUrl url(QStringLiteral("https://nominatim.openstreetmap.org/reverse"));
    QUrlQuery urlQuery;

    urlQuery.addQueryItem(QStringLiteral("format"), QStringLiteral("jsonv2"));
    urlQuery.addQueryItem(QStringLiteral("lat"), QString::number(update.coordinate().latitude()));
    urlQuery.addQueryItem(QStringLiteral("lon"), QString::number(update.coordinate().longitude()));
    urlQuery.addQueryItem(QStringLiteral("email"), QStringLiteral("hanyoung@protonmail.com"));
    url.setQuery(urlQuery);

    qWarning() << "lat: " << update.coordinate().latitude() << "lon: " << update.coordinate().longitude();
    auto reply = d->manager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, [this, update, reply] {
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = document.object();
        Q_EMIT this->located(LocationQueryResult(update.coordinate().latitude(),
                                                 update.coordinate().longitude(),
                                                 root[QStringLiteral("display_name")].toString(),
                                                 root[QStringLiteral("name")].toString(),
                                                 root[QStringLiteral("address")].toObject()[QStringLiteral("country_code")].toString(),
                                                 root[QStringLiteral("address")].toObject()[QStringLiteral("country")].toString(),
                                                 root[QStringLiteral("osm_id")].toString()));
        reply->deleteLater();
    });
}
}
