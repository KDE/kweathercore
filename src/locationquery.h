/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "locationqueryresult.h"
#include <QObject>
class QNetworkAccessManager;
namespace LibKWeather
{
class LocationQuery : public QObject
{
    Q_OBJECT
public:
    enum class LocateMethod { GeoIP, GPS };
    LocationQuery();

    void query(QString name, int number = 30);
    void locate(LocateMethod method = LocateMethod::GeoIP);
Q_SIGNALS:
    void queryFinished(QVector<LocationQueryResult> result);
    void located(LocationQueryResult result);

private:
    void geoIPLocate();
    void GPSLocate();
    QNetworkAccessManager *m_manager = nullptr;
};
}
