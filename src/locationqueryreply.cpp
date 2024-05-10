/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "locationqueryreply.h"
#include "kweathercore_p.h"
#include "locationquery.h"
#include "locationqueryresult.h"
#include "reply_p.h"

#include <QCoreApplication>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

#if QT_CONFIG(permissions)
#include <QPermissions>
#endif

using namespace KWeatherCore;

class KWeatherCore::LocationQueryReplyPrivate : public ReplyPrivate
{
public:
    void requestPosition(LocationQueryReply *q, QGeoPositionInfoSource *source, QNetworkAccessManager *nam);
    std::vector<LocationQueryResult> m_result;
};

static std::optional<QString> findSubdivision(const QJsonObject &json)
{
    const auto adminCodeIter = json.constFind(QLatin1String("adminCodes1"));
    if (adminCodeIter == json.constEnd()) {
        return std::nullopt;
    } else {
        return (*adminCodeIter).toObject().value(QLatin1String("ISO3166_2")).toString();
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

        const QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        const QJsonObject root = document.object();

        auto counts = root[QLatin1String("totalResultsCount")].toInt();
        // if no result
        if (!counts) {
            d->setError(Reply::NotFound);
            Q_EMIT finished();
            return;
        }

        // if our api calls reached daily limit
        if (root[QLatin1String("status")].toObject()[QLatin1String("value")].toInt() == 18) {
            d->setError(Reply::RateLimitExceeded);
            qWarning("API calls reached daily limit");
            Q_EMIT finished();
            return;
        }

        const auto geonames = root.value(QLatin1String("geonames")).toArray();
        // add query results
        for (const auto &resRef : std::as_const(geonames)) {
            const auto res = resRef.toObject();
            const auto result = LocationQueryResult(res.value(QLatin1String("lat")).toString().toFloat(),
                                                    res.value(QLatin1String("lng")).toString().toFloat(),
                                                    res.value(QLatin1String("toponymName")).toString(),
                                                    res.value(QLatin1String("name")).toString(),
                                                    res.value(QLatin1String("countryCode")).toString(),
                                                    QString::number(res.value(QLatin1String("geonameId")).toInt()),
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

#if QT_CONFIG(permissions)
    QLocationPermission permission;
    permission.setAccuracy(QLocationPermission::Precise);
    permission.setAvailability(QLocationPermission::WhenInUse);
    switch (QCoreApplication::instance()->checkPermission(permission)) {
    case Qt::PermissionStatus::Undetermined:
        QCoreApplication::instance()->requestPermission(permission, this, [this, nam, source](const auto &permission) {
            Q_D(LocationQueryReply);
            if (permission.status() == Qt::PermissionStatus::Granted) {
                d->requestPosition(this, source, nam);
            } else {
                d->setError(LocationQueryReply::NoService);
                Q_EMIT finished();
            }
        });
        return;
    case Qt::PermissionStatus::Denied:
        d->setError(LocationQueryReply::NoService);
        QMetaObject::invokeMethod(this, &LocationQueryReply::finished, Qt::QueuedConnection);
        return;
    case Qt::PermissionStatus::Granted:
        d->requestPosition(this, source, nam);
        break;
    }
#else
    d->requestPosition(this, source, nam);
#endif
}

LocationQueryReply::~LocationQueryReply() = default;

void LocationQueryReplyPrivate::requestPosition(LocationQueryReply *q, QGeoPositionInfoSource *source, QNetworkAccessManager *nam)
{
    QObject::connect(source, &QGeoPositionInfoSource::positionUpdated, q, [this, q, nam](const QGeoPositionInfo &pos) {
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

        QObject::connect(reply, &QNetworkReply::finished, q, [this, q, lat, lon, reply] {
            reply->deleteLater();
            const QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
            const QJsonObject root = document.object();
            const auto array = root[QLatin1String("geonames")].toArray();
            if (array.size()) {
                m_result.push_back(LocationQueryResult(lat,
                                                       lon,
                                                       array.at(0)[QLatin1String("toponymName")].toString(),
                                                       array.at(0)[QLatin1String("name")].toString(),
                                                       array.at(0)[QLatin1String("countryCode")].toString(),
                                                       array.at(0)[QLatin1String("countryName")].toString(),
                                                       QString::number(root[QLatin1String("geonameId")].toInt())));
            } else {
                setError(Reply::NotFound);
            }

            Q_EMIT q->finished();
        });
    });

    source->requestUpdate();
}

const std::vector<LocationQueryResult> &LocationQueryReply::result() const
{
    Q_D(const LocationQueryReply);
    return d->m_result;
}
