/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <QObject>
#include <QTest>
#include <kweathercore/dailyweatherforecast.h>

using namespace KWeatherCore;

class DailyForecastTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testNull();
    void testHourlyMerge();
    void testDailyMerge();
    void testJson();
    void testSunrise();

private:
    DailyWeatherForecast d1;
    DailyWeatherForecast d2;
};
