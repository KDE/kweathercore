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
#include <optional>
namespace KWeatherCore
{
class LocationQueryPrivate
{
public:
    LocationQueryPrivate(LocationQuery *parent);
    void positionUpdated(const QGeoPositionInfo &update);
    void handleQueryResult(QNetworkReply *reply);

    LocationQuery *q = nullptr;
    QNetworkAccessManager *manager = nullptr;
    QGeoPositionInfoSource *locationSource = nullptr;
};

LocationQueryPrivate::LocationQueryPrivate(LocationQuery *parent)
    : q(parent)
    , manager(new QNetworkAccessManager(q))
    , locationSource(QGeoPositionInfoSource::createDefaultSource(q))
{
    if (locationSource) {
        locationSource->stopUpdates();
        QObject::connect(locationSource, &QGeoPositionInfoSource::positionUpdated, q, [this](const auto &pos) {
            positionUpdated(pos);
        });
    }
}

void LocationQueryPrivate::positionUpdated(const QGeoPositionInfo &update)
{
    auto lat = KWeatherCorePrivate::toFixedString(update.coordinate().latitude());
    auto lon = KWeatherCorePrivate::toFixedString(update.coordinate().longitude());
    QUrl url(QStringLiteral("http://api.geonames.org/findNearbyJSON"));
    QUrlQuery urlQuery;

    urlQuery.addQueryItem(QStringLiteral("lat"), lat);
    urlQuery.addQueryItem(QStringLiteral("lng"), lon);
    urlQuery.addQueryItem(QStringLiteral("username"), QStringLiteral("kweatherdev"));
    url.setQuery(urlQuery);

    auto req = QNetworkRequest(url);

    qWarning() << "lat: " << lat << "lon: " << lon;
    auto reply = manager->get(req);

    QObject::connect(reply, &QNetworkReply::finished, [this, update, reply] {
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = document.object();
        auto array = root[QStringLiteral("geonames")].toArray();
        if (array.size()) {
            Q_EMIT q->located(LocationQueryResult(update.coordinate().latitude(),
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

LocationQuery::~LocationQuery() = default;

void LocationQuery::query(QString name, int number)
{
    QUrl url(QStringLiteral("http://api.geonames.org/searchJSON"));
    QUrlQuery urlQuery;

    urlQuery.addQueryItem(QStringLiteral("q"), name);
    urlQuery.addQueryItem(QStringLiteral("maxRows"), QString::number(number));
    urlQuery.addQueryItem(QStringLiteral("username"), QStringLiteral("kweatherdev"));
    url.setQuery(urlQuery);

    auto reply = d->manager->get(QNetworkRequest(url));
    QObject::connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        d->handleQueryResult(reply);
    });
}
void LocationQuery::locate()
{
    if (d->locationSource) {
        d->locationSource->requestUpdate();
    }
}
static std::optional<QString> findSubdivision(const QJsonObject &json)
{
    const auto adminCodeIter = json.constFind(QStringLiteral("adminCodes1"));
    if (adminCodeIter == json.constEnd()) {
        return std::nullopt;
    } else {
        return (*adminCodeIter).toObject().value(QStringLiteral("ISO3166_2")).toString();
    }
}
void LocationQueryPrivate::handleQueryResult(QNetworkReply *reply)
{
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject root = document.object();
    reply->deleteLater();

    auto counts = root[QStringLiteral("totalResultsCount")].toInt();
    // if no result
    if (!counts) {
        Q_EMIT q->queryError();
        return;
    }
    std::vector<LocationQueryResult> retVec;

    // if our api calls reached daily limit
    if (root[QStringLiteral("status")].toObject()[QStringLiteral("value")].toInt() == 18) {
        Q_EMIT q->queryError();
        qWarning("API calls reached daily limit");
        return;
    }
    auto geonames = root.value(QStringLiteral("geonames")).toArray();
    // add query results
    for (const auto &resRef : qAsConst(geonames)) {
        auto res = resRef.toObject();
        auto result = LocationQueryResult(res.value(QStringLiteral("lat")).toString().toFloat(),
                                          res.value(QStringLiteral("lng")).toString().toFloat(),
                                          res.value(QStringLiteral("toponymName")).toString(),
                                          res.value(QStringLiteral("name")).toString(),
                                          res.value(QStringLiteral("countryCode")).toString(),
                                          res.value(QStringLiteral("countryName")).toString(),
                                          QString::number(res.value(QStringLiteral("geonameId")).toInt()),
                                          findSubdivision(res));
        retVec.push_back(result);
    }

    Q_EMIT q->queryFinished(retVec);
}
}
