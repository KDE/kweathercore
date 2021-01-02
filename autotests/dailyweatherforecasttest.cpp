/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "dailyweatherforecasttest.h"
#include <kweathercore/hourlyweatherforecast.h>
#include <kweathercore/sunrise.h>

using namespace KWeatherCore;

void DailyForecastTest::testNull()
{
    QCOMPARE(d1.isNull(), true);
}

void DailyForecastTest::testHourlyMerge()
{
    for (int i = 0; i < 24; ++i)
        d1 += HourlyWeatherForecast(QDateTime::currentDateTime().addSecs(i * 3600), QStringLiteral("sunny"), QStringLiteral("sunny"), QString(), 25, 100, WindDirection::E, 5, 0.8, 0.1, 0.1, 0);

    QCOMPARE(d1.isNull(), false);

    QCOMPARE(d1.hourlyWeatherForecast().size(), 24);
}
void DailyForecastTest::testDailyMerge()
{
    const auto &vec = d1.hourlyWeatherForecast();
    for (auto h : vec)
        d2 += h;

    QCOMPARE(d2.hourlyWeatherForecast().size(), d1.hourlyWeatherForecast().size());
}
void DailyForecastTest::testJson()
{
    auto d3 = DailyWeatherForecast::fromJson(d2.toJson());

    QCOMPARE(d3.toJson(), d2.toJson());
}
void DailyForecastTest::testSunrise()
{
    auto date = QDateTime::currentDateTime();
    d2.setSunrise(Sunrise(date, {}, {}, {}, {}, {}, {}, {}, {}));
    QCOMPARE(d2.sunrise().sunRise(), date);
}
QTEST_MAIN(DailyForecastTest)
