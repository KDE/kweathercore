/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once
#include <QTest>
#include <kweathercore/hourlyforecast.h>
using namespace KWeatherCore;
class HourlyForecastTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testJson();

private:
    HourlyWeatherForecast d1;
};
