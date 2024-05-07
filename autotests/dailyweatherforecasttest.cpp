/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "dailyweatherforecasttest.h"
#include <kweathercore/hourlyweatherforecast.h>

using namespace KWeatherCore;

void DailyForecastTest::testHourlyMerge()
{
    for (int i = 0; i < 24; ++i) {
        HourlyWeatherForecast h(QDateTime::currentDateTime().addSecs(i * 3600));
        h.setWeatherDescription(QStringLiteral("sunny"));
        h.setWeatherIcon(QStringLiteral("sunny"));
        h.setTemperature(25);
        h.setPressure(100);
        h.setWindDirectionDegree(90.0);
        h.setFog(0.2);
        h.setUvIndex(0.8);
        h.setWindSpeed(5);
        h.setHumidity(0.1);

        d1 += std::move(h);
    }
    QCOMPARE(d1.hourlyWeatherForecast().size(), 24);
}
void DailyForecastTest::testDailyMerge()
{
    const auto &vec = d1.hourlyWeatherForecast();
    for (auto &h : vec) {
        d2 += HourlyWeatherForecast(h);
    }

    QCOMPARE(d2.hourlyWeatherForecast().size(), d1.hourlyWeatherForecast().size());
}
void DailyForecastTest::testJson()
{
    auto d3 = DailyWeatherForecast::fromJson(d2.toJson());

    QCOMPARE(d3.toJson(), d2.toJson());
}
QTEST_MAIN(DailyForecastTest)

#include "moc_dailyweatherforecasttest.cpp"
