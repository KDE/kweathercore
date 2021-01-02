/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <QObject>
#include <kweathercore/kweathercore_export.h>
class QNetworkReply;
class QNetworkAccessManager;
namespace KWeatherCore
{
class GeoTimezonePrivate;
/**
 * @short Class to obtain timezone via coordinate
 *
 * This is a class to obtain timezone via coordinate and signals finished() with
 * a QString
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT GeoTimezone : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief GeoTimezone
     * @param latitude latitude for the request location
     * @param longitude longitude for the request location
     */
    GeoTimezone(double latitude, double longitude, QObject *parent = nullptr);

Q_SIGNALS:
    /**
     * @brief finished emit when the timezone has been obtained
     * @param timezone IANA timezone string
     */
    void finished(const QString &timezone);
    /**
     * @brief networkError encounted network error
     */
    void networkErrorOccured();

private:
    GeoTimezonePrivate *d;
};
}
