/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include <QTest>
#include <kweathercore/hourlyweatherforecast.h>
using namespace KWeatherCore;
class HourlyForecastTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testJson();
};
