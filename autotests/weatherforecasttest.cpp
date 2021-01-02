/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "weatherforecasttest.h"
#include <kweathercore/dailyweatherforecast.h>
#include <kweathercore/hourlyweatherforecast.h>
void WeatherForecastTest::testHourlyMerge()
{
    auto date = QDateTime::currentDateTime();
    for (int i = 0; i < 100; ++i) {
        d1 += HourlyWeatherForecast(date.addSecs(i * 3600), {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {});
    }

    QCOMPARE(d1.dailyWeatherForecast().size() > 0, true);
}

void WeatherForecastTest::testDailyMerge()
{
    auto date = QDate::currentDate();

    d1.setDailyWeatherForecast({DailyWeatherForecast(30, 20, 0, 0, 0, 0, QString(), QString(), date),
                                DailyWeatherForecast(30, 20, 0, 0, 0, 0, QString(), QString(), date.addDays(1)),
                                DailyWeatherForecast(30, 20, 0, 0, 0, 0, QString(), QString(), date.addDays(2))});

    QCOMPARE(d1.dailyWeatherForecast().size(), 3);
}

void WeatherForecastTest::testCreatedTime()
{
    QCOMPARE(d1.createdTime().isValid(), true);
}

void WeatherForecastTest::testJson()
{
    auto d2 = WeatherForecast::fromJson(d1.toJson());
    QCOMPARE(d2->toJson(), d1.toJson());
}
QTEST_MAIN(WeatherForecastTest)
