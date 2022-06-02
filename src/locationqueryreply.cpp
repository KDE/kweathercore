/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "locationqueryreply.h"
#include "kweathercore_p.h"
#include "locationqueryresult.h"
#include "reply_p.h"

#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace KWeatherCore;

class KWeatherCore::LocationQueryReplyPrivate : public ReplyPrivate
{
public:
    std::vector<LocationQueryResult> m_result;
};

static std::optional<QString> findSubdivision(const QJsonObject &json)
{
    const auto adminCodeIter = json.constFind(QStringLiteral("adminCodes1"));
    if (adminCodeIter == json.constEnd()) {
        return std::nullopt;
    } else {
        return (*adminCodeIter).toObject().value(QStringLiteral("ISO3166_2")).toString();
    }
}

LocationQueryReply::LocationQueryReply(const QString &name, int number, QNetworkAccessManager *nam, QObject *parent)
    : Reply(new LocationQueryReplyPrivate, parent)
{
    QUrl url(QStringLiteral("http://api.geonames.org/searchJSON"));
    QUrlQuery urlQuery;

    urlQuery.addQueryItem(QStringLiteral("q"), name);
    urlQuery.addQueryItem(QStringLiteral("maxRows"), QString::number(number));
    urlQuery.addQueryItem(QStringLiteral("username"), QStringLiteral("kweatherdev"));
    url.setQuery(urlQuery);

    auto reply = nam->get(QNetworkRequest(url));
    QObject::connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        Q_D(LocationQueryReply);
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            d->setError(Reply::NetworkError, reply->errorString());
            Q_EMIT finished();
            return;
        }

        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = document.object();

        auto counts = root[QStringLiteral("totalResultsCount")].toInt();
        // if no result
        if (!counts) {
            d->setError(Reply::NotFound);
            Q_EMIT finished();
            return;
        }

        // if our api calls reached daily limit
        if (root[QStringLiteral("status")].toObject()[QStringLiteral("value")].toInt() == 18) {
            d->setError(Reply::RateLimitExceeded);
            qWarning("API calls reached daily limit");
            Q_EMIT finished();
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
            d->m_result.push_back(result);
        }

        Q_EMIT finished();
    });
}

LocationQueryReply::LocationQueryReply(QGeoPositionInfoSource *source, QNetworkAccessManager *nam, QObject *parent)
    : Reply(new LocationQueryReplyPrivate, parent)
{
    Q_D(LocationQueryReply);
    if (!source) {
        d->setError(LocationQueryReply::NoService);
        QMetaObject::invokeMethod(this, &LocationQueryReply::finished, Qt::QueuedConnection);
        return;
    }

    connect(source, &QGeoPositionInfoSource::positionUpdated, this, [this, nam](const QGeoPositionInfo &pos) {
        const auto lat = pos.coordinate().latitude();
        const auto lon = pos.coordinate().longitude();
        QUrl url(QStringLiteral("http://api.geonames.org/findNearbyJSON"));
        QUrlQuery urlQuery;

        urlQuery.addQueryItem(QStringLiteral("lat"), KWeatherCorePrivate::toFixedString(lat));
        urlQuery.addQueryItem(QStringLiteral("lng"), KWeatherCorePrivate::toFixedString(lon));
        urlQuery.addQueryItem(QStringLiteral("username"), QStringLiteral("kweatherdev"));
        url.setQuery(urlQuery);

        auto req = QNetworkRequest(url);

        qWarning() << "lat: " << lat << "lon: " << lon;
        auto reply = nam->get(req);

        QObject::connect(reply, &QNetworkReply::finished, [this, lat, lon, reply] {
            Q_D(LocationQueryReply);
            reply->deleteLater();
            QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
            QJsonObject root = document.object();
            auto array = root[QStringLiteral("geonames")].toArray();
            if (array.size()) {
                d->m_result.push_back(LocationQueryResult(lat,
                                                          lon,
                                                          array.at(0)[QStringLiteral("toponymName")].toString(),
                                                          array.at(0)[QStringLiteral("name")].toString(),
                                                          array.at(0)[QStringLiteral("countryCode")].toString(),
                                                          array.at(0)[QStringLiteral("countryName")].toString(),
                                                          QString::number(root[QStringLiteral("geonameId")].toInt())));
            } else {
                d->setError(Reply::NotFound);
            }

            Q_EMIT finished();
        });
    });

    source->requestUpdate();
}

LocationQueryReply::~LocationQueryReply() = default;

const std::vector<LocationQueryResult> &LocationQueryReply::result() const
{
    Q_D(const LocationQueryReply);
    return d->m_result;
}
