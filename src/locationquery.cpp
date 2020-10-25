/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "locationquery.h"
namespace LibKWeather
{
void LocationQuery::locate(LocateMethod method)
{
    switch (method) {
    case LocateMethod::GeoIP:
        geoIPLocate();
        break;
    case LocateMethod::GPS:
        GPSLocate();
        break;
    default:
        geoIPLocate();
    }
}
}
