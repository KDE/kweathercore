/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "locationqueryresult.h"
#include <QGeoPositionInfo>
#include <QObject>
#include <kweathercore/kweathercore_export.h>
class QNetworkReply;
namespace KWeatherCore
{
class LocationQueryPrivate;
class LocationQuery : public QObject
{
    Q_OBJECT
public:
    LocationQuery(QObject *parent = nullptr);
    ~LocationQuery();
    void query(QString name, int number = 30);
    void locate();
Q_SIGNALS:
    void queryFinished(QVector<LocationQueryResult> result);
    void located(LocationQueryResult result);
    void queryError();
private Q_SLOTS:
    void handleQueryResult(QNetworkReply *reply);
    void positionUpdated(const QGeoPositionInfo &update);

private:
    LocationQueryPrivate *d = nullptr;
};
}
