/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "dailyforecasttest.h"
#include <kweathercore/hourlyforecast.h>
#include <kweathercore/sunrise.h>
using namespace KWeatherCore;

void DailyForecastTest::testNull()
{
    QCOMPARE(d1.isNull(), true);
}

void DailyForecastTest::testHourlyMerge()
{
    for (int i = 0; i < 24; ++i)
        d1 << HourlyWeatherForecast(QDateTime::currentDateTime().addSecs(i * 3600), QStringLiteral("sunny"), QStringLiteral("sunny"), QString(), 25, 100, WindDirection::E, 5, 0.8, 0.1, 0.1, 0);

    QCOMPARE(d1.isNull(), false);
    QCOMPARE(d1.hourlyWeatherForecast().size(), 24);
}
void DailyForecastTest::testDailyMerge()
{
    d2 << d1;

    QCOMPARE(d2.isNull(), false);
    QCOMPARE(d2.hourlyWeatherForecast(), d1.hourlyWeatherForecast());
}
void DailyForecastTest::testJson()
{
    auto d3 = DailyWeatherForecast::fromJson(d2.toJson());

    QCOMPARE(d3, d2);
}
void DailyForecastTest::testSunrise()
{
    auto date = QDateTime::currentDateTime();
    d2.setSunrise(Sunrise(date, {}, {}, {}, {}, {}, {}, {}, {}));
    QCOMPARE(d2.sunrise().sunRise(), date);
}
QTEST_MAIN(DailyForecastTest)
