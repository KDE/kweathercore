/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "dailyweatherforecasttest.h"
#include <kweathercore/hourlyweatherforecast.h>
#include <kweathercore/sunrise.h>

using namespace KWeatherCore;

void DailyForecastTest::testNull()
{
    QCOMPARE(d1.isValid(), true);
}

void DailyForecastTest::testHourlyMerge()
{
    for (int i = 0; i < 24; ++i) {
        HourlyWeatherForecast h(QDateTime::currentDateTime().addSecs(i * 3600));
        h.setWeatherDescription(QStringLiteral("sunny"));
        h.setWeatherIcon(QStringLiteral("sunny"));
        h.setTemperature(25);
        h.setPressure(100);
        h.setWindDirection(WindDirection::E);
        h.setFog(0.2);
        h.setUvIndex(0.8);
        h.setWindSpeed(5);
        h.setHumidity(0.1);

        d1 += h;
    }
    QCOMPARE(d1.isValid(), false);

    QCOMPARE(d1.hourlyWeatherForecast().size(), 24);
}
void DailyForecastTest::testDailyMerge()
{
    const auto &vec = d1.hourlyWeatherForecast();
    for (auto &h : vec)
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
    Sunrise s;
    s.setSunRise(date);
    d2.setSunrise(s);
    QCOMPARE(d2.sunrise().sunRise(), date);
}
QTEST_MAIN(DailyForecastTest)
