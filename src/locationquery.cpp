/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "locationquery.h"
#include "kweathercore_p.h"
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
    void located(const KWeatherCore::LocationQueryResult &);
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
    auto lat = KWeatherCorePrivate::instance()->toFixedString(update.coordinate().latitude());
    auto lon = KWeatherCorePrivate::instance()->toFixedString(update.coordinate().longitude());
    QUrl url(QStringLiteral("http://api.geonames.org/findNearbyJSON"));
    QUrlQuery urlQuery;

    urlQuery.addQueryItem(QStringLiteral("lat"), lat);
    urlQuery.addQueryItem(QStringLiteral("lng"), lon);
    urlQuery.addQueryItem(QStringLiteral("username"),
                          QStringLiteral("kweatherdev"));
    url.setQuery(urlQuery);

    auto req = QNetworkRequest(url);

    qWarning() << "lat: " << lat << "lon: " << lon;
    auto reply = manager->get(req);

    connect(reply, &QNetworkReply::finished, [this, update, reply] {
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = document.object();
        auto array = root[QStringLiteral("geonames")].toArray();
        if (array.size()) {
            Q_EMIT this->located(LocationQueryResult(
                update.coordinate().latitude(),
                update.coordinate().longitude(),
                array.at(0)[QStringLiteral("toponymName")].toString(),
                array.at(0)[QStringLiteral("name")].toString(),
                array.at(0)[QStringLiteral("countryCode")].toString(),
                array.at(0)[QStringLiteral("countryName")].toString(),
                QString::number(root[QStringLiteral("geonameId")].toInt())));
        }
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
    connect(reply, &QNetworkReply::finished, [reply, this] {
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
    std::vector<LocationQueryResult> retVec;

    // if our api calls reached daily limit
    if (root[QStringLiteral("status")]
            .toObject()[QStringLiteral("value")]
            .toInt() == 18) {
        Q_EMIT queryError();
        qWarning("API calls reached daily limit");
        return;
    }
    auto geonames = root.value(QStringLiteral("geonames")).toArray();
    // add query results
    for (const auto &resRef : qAsConst(geonames)) {
        auto res = resRef.toObject();
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
