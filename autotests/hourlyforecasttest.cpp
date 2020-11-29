/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "hourlyforecasttest.h"

void HourlyForecastTest::testJson()
{
    d1.setDate(QDateTime::currentDateTime());

    auto d2 = HourlyWeatherForecast::fromJson(d1.toJson());

    QCOMPARE(d2.date(), d1.date());
}
QTEST_MAIN(HourlyForecastTest)
