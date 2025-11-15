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
#include <QNetworkAccessManager>
#include <QStandardPaths>

namespace KWeatherCore
{
class LocationQueryPrivate
{
public:
    explicit LocationQueryPrivate(LocationQuery *parent);
    void positionUpdated(const QGeoPositionInfo &update);
    QNetworkAccessManager *networkAccessManager();

    LocationQuery *q = nullptr;
    QNetworkAccessManager *manager = nullptr;
    QGeoPositionInfoSource *locationSource = nullptr;
};

LocationQueryPrivate::LocationQueryPrivate(LocationQuery *parent)
    : q(parent)
    , locationSource(QGeoPositionInfoSource::createDefaultSource(q))
{
    if (locationSource) {
        locationSource->stopUpdates();
    }
}

QNetworkAccessManager *LocationQueryPrivate::networkAccessManager()
{
    if (!manager) {
        manager = new QNetworkAccessManager(q);
        manager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
        manager->setStrictTransportSecurityEnabled(true);
        manager->enableStrictTransportSecurityStore(true,
                                                    QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation)
                                                        + QLatin1String("/org.kde.kweathercore/hsts/"));
    }
    return manager;
}

LocationQuery::LocationQuery(QObject *parent)
    : QObject(parent)
    , d(new LocationQueryPrivate(this))
{
}

LocationQuery::~LocationQuery() = default;

LocationQueryReply *LocationQuery::query(const QString &name, int number)
{
    return new LocationQueryReply(name, number, d->networkAccessManager(), this);
}

LocationQueryReply *LocationQuery::locate()
{
    return new LocationQueryReply(d->locationSource, d->networkAccessManager(), this);
}

void LocationQuery::setNetworkAccessManager(QNetworkAccessManager *nam)
{
    if (d->manager == nam) {
        return;
    }

    if (d->manager->parent() == this) {
        delete d->manager;
    }
    d->manager = nam;
}
}

#include "moc_locationquery.cpp"
