/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include <QTest>
#include <kweathercore/weatherforecast.h>
using namespace KWeatherCore;
class WeatherForecastTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testHourlyMerge();
    void testDailyMerge();
    void testCreatedTime();
    void testJson();

private:
    WeatherForecast d1;
};
