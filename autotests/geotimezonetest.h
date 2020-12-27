/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include <QSignalSpy>
#include <QTest>
#include <kweathercore/geotimezone.h>
using namespace KWeatherCore;
class GeoTimezoneTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testTimezone();

private:
    GeoTimezone d {50, 50};
    QSignalSpy finished_spy {&d, &GeoTimezone::finished};
    QSignalSpy networkError_spy {&d, &GeoTimezone::networkError};
};
