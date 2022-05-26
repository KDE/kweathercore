/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "reply.h"

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
class KWEATHERCORE_EXPORT GeoTimezone : public Reply
{
    Q_OBJECT
public:
    /**
     * GeoTimezone
     * @param latitude latitude for the request location
     * @param longitude longitude for the request location
     */
    GeoTimezone(QNetworkAccessManager *nam, double latitude, double longitude, QObject *parent = nullptr);

    /** The result IANA timezone string. */
    QString timezone() const;

private:
    Q_DECLARE_PRIVATE(GeoTimezone)
};
}
