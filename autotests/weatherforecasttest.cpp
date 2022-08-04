/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "weatherforecasttest.h"
#include <kweathercore/dailyweatherforecast.h>
#include <kweathercore/hourlyweatherforecast.h>
void WeatherForecastTest::testHourlyMerge()
{
    auto date = QDateTime({2022, 8, 4}, {22, 0});
    for (int i = 0; i < 100; ++i) {
        HourlyWeatherForecast f(date.addSecs(i * 3600));
        d1 += std::move(f);
    }

    QCOMPARE(d1.dailyWeatherForecast().size(), 6);
}

void WeatherForecastTest::testDailyMerge()
{
    auto date = QDate::currentDate();
    std::vector<DailyWeatherForecast> vec;
    for (int i = 0; i < 3; i++) {
        DailyWeatherForecast d;
        d.setDate(date.addDays(i));
        vec.push_back(d);
    }
    d1.setDailyWeatherForecast(vec);

    QCOMPARE(d1.dailyWeatherForecast().size(), 3);
}

void WeatherForecastTest::testCreatedTime()
{
    QCOMPARE(d1.createdTime().isValid(), true);
}

void WeatherForecastTest::testJson()
{
    auto d2 = WeatherForecast::fromJson(d1.toJson());
    QCOMPARE(d2.toJson(), d1.toJson());
}
QTEST_MAIN(WeatherForecastTest)
