/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "hourlyweatherforecasttest.h"

void HourlyForecastTest::testJson()
{
    d1.setDate(QDateTime::currentDateTime());

    auto d2 = HourlyWeatherForecast::fromJson(d1.toJson());

    QCOMPARE(d2.toJson(), d1.toJson());
}
QTEST_MAIN(HourlyForecastTest)
