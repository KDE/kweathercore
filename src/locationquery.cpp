/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
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
class LocationQueryPrivate : public QObject
{
    Q_OBJECT
public:
    LocationQueryPrivate(LocationQuery *parent);
    void requestUpdate();
    void query(QString name, int number);
Q_SIGNALS:
    void located(const LocationQueryResult &);
    void queryFinished(std::vector<LocationQueryResult> result);
    void queryError();
private Q_SLOTS:
    void positionUpdated(const QGeoPositionInfo &update);
    void handleQueryResult(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager = nullptr;
    QGeoPositionInfoSource *locationSource = nullptr;
};

LocationQueryPrivate::LocationQueryPrivate(LocationQuery *parent)
    : QObject(parent)
    , manager(new QNetworkAccessManager(this))
    , locationSource(QGeoPositionInfoSource::createDefaultSource(this))
{
    locationSource->stopUpdates();

    connect(locationSource,
            &QGeoPositionInfoSource::positionUpdated,
            this,
            &LocationQueryPrivate::positionUpdated);
    connect(this,
            &LocationQueryPrivate::queryFinished,
            parent,
            &LocationQuery::queryFinished);
    connect(this,
            &LocationQueryPrivate::queryError,
            parent,
            &LocationQuery::queryError);
    connect(
        this, &LocationQueryPrivate::located, parent, &LocationQuery::located);
}

void LocationQueryPrivate::requestUpdate()
{
    locationSource->requestUpdate();
}
void LocationQueryPrivate::positionUpdated(const QGeoPositionInfo &update)
{
    auto roundCoordinate = [](QString coordinate)-> QString {
        auto pointPos = coordinate.indexOf(QLatin1Char('.'));
        coordinate.truncate(pointPos + 3);
        return coordinate;
    };
    auto lat = roundCoordinate(QString::number(update.coordinate().latitude()));
    auto lon = roundCoordinate(QString::number(update.coordinate().longitude()));
    QUrl url(QStringLiteral("https://nominatim.openstreetmap.org/reverse"));
    QUrlQuery urlQuery;

    urlQuery.addQueryItem(QStringLiteral("format"), QStringLiteral("jsonv2"));
    urlQuery.addQueryItem(
        QStringLiteral("lat"),
        lat);
    urlQuery.addQueryItem(
        QStringLiteral("lon"),
        lon);
    urlQuery.addQueryItem(QStringLiteral("email"),
                          QStringLiteral("hanyoung@protonmail.com"));
    url.setQuery(urlQuery);

    qWarning() << "lat: " << lat
               << "lon: " << lon;
    auto reply = manager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, [this, update, reply] {
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = document.object();
        Q_EMIT this->located(
            LocationQueryResult(update.coordinate().latitude(),
                                update.coordinate().longitude(),
                                root[QStringLiteral("display_name")].toString(),
                                root[QStringLiteral("name")].toString(),
                                root[QStringLiteral("address")]
                                    .toObject()[QStringLiteral("country_code")]
                                    .toString(),
                                root[QStringLiteral("address")]
                                    .toObject()[QStringLiteral("country")]
                                    .toString(),
                                root[QStringLiteral("osm_id")].toString()));
        qWarning() << root;
        reply->deleteLater();
    });
}
LocationQuery::LocationQuery(QObject *parent)
    : QObject(parent)
    , d(new LocationQueryPrivate(this))
{
}
void LocationQuery::query(QString name, int number)
{
    d->query(std::move(name), number);
}

void LocationQueryPrivate::query(QString name, int number)
{
    QUrl url(QStringLiteral("http://api.geonames.org/searchJSON"));
    QUrlQuery urlQuery;

    urlQuery.addQueryItem(QStringLiteral("q"), name);
    urlQuery.addQueryItem(QStringLiteral("maxRows"), QString::number(number));
    urlQuery.addQueryItem(QStringLiteral("username"),
                          QStringLiteral("kweatherdev"));
    url.setQuery(urlQuery);

    auto reply = manager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, [reply, this]{
        this->handleQueryResult(reply);
    });
}
void LocationQuery::locate()
{
    d->requestUpdate();
}
void LocationQueryPrivate::handleQueryResult(QNetworkReply *reply)
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
    std::vector<LocationQueryResult> retVec(counts);

    // if our api calls reached daily limit
    if (root[QStringLiteral("status")]
            .toObject()[QStringLiteral("value")]
            .toInt() == 18) {
        Q_EMIT queryError();
        qWarning("API calls reached daily limit");
        return;
    }
    QJsonArray geonames = root.value(QStringLiteral("geonames")).toArray();
    // add query results
    for (QJsonValueRef resRef : geonames) {
        QJsonObject res = resRef.toObject();
        auto result = LocationQueryResult(
            res.value(QStringLiteral("lat")).toString().toFloat(),
            res.value(QStringLiteral("lng")).toString().toFloat(),
            res.value(QStringLiteral("toponymName")).toString(),
            res.value(QStringLiteral("name")).toString(),
            res.value(QStringLiteral("countryCode")).toString(),
            res.value(QStringLiteral("countryName")).toString(),
            QString::number(res.value(QStringLiteral("geonameId")).toInt()));
        retVec.push_back(result);
    }

    Q_EMIT queryFinished(retVec);
}
}

#include "locationquery.moc"
