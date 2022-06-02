/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "locationquery.h"
#include "kweathercore_p.h"
#include "locationqueryreply.h"

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
    }
}

LocationQuery::LocationQuery(QObject *parent)
    : QObject(parent)
    , d(new LocationQueryPrivate(this))
{
}

LocationQuery::~LocationQuery() = default;

LocationQueryReply *LocationQuery::query(const QString &name, int number)
{
    auto reply = new LocationQueryReply(name, number, d->manager, this);
    connect(reply, &LocationQueryReply::finished, this, [reply, this]() {
        reply->deleteLater();
        if (reply->error() != Reply::NoError) {
            Q_EMIT queryError();
        } else {
            Q_EMIT queryFinished(reply->result());
        }
    });

    return reply;
}
LocationQueryReply *LocationQuery::locate()
{
    auto reply = new LocationQueryReply(d->locationSource, d->manager, this);
    connect(reply, &LocationQueryReply::finished, this, [reply, this]() {
        reply->deleteLater();
        if (reply->error() == Reply::NoError && !reply->result().empty()) {
            Q_EMIT located(reply->result().front());
        }
    });

    return reply;
}
}
