/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "sunrisetest.h"

void SunriseTest::testJson()
{
    auto date = QDateTime::currentDateTime();
    d1.setHighMoon({date, 0.1});
    d1.setLowMoon({date, 0.1});
    d1.setMoonPhase(0.5);
    d1.setMoonRise(date);
    d1.setMoonSet(date);
    d1.setSolarMidnight({date, 0.6});
    d1.setSolarNoon({date, 0.5});
    d1.setSunRise(date);
    d1.setSunSet(date);
    d2 = Sunrise::fromJson(d1.toJson());

    QCOMPARE(d2.highMoon(), d1.highMoon());
    QCOMPARE(std::abs(d2.highMoonTime().secsTo(d1.highMoonTime())) < 1, true);
    QCOMPARE(d2.lowMoon(), d1.lowMoon());
    QCOMPARE(std::abs(d2.lowMoonTime().secsTo(d1.lowMoonTime())) < 1, true);
    QCOMPARE(d2.moonPhase(), d1.moonPhase());
    QCOMPARE(std::abs(d2.moonRise().secsTo(d1.moonRise())) < 1, true);
    QCOMPARE(std::abs(d2.moonSet().secsTo(d1.moonSet())) < 1, true);
    QCOMPARE(d2.solarMidnight(), d1.solarMidnight());
    QCOMPARE(std::abs(d2.solarMidnightTime().secsTo(d1.solarMidnightTime())) <
                 1,
             true);
    QCOMPARE(d2.solarNoon(), d2.solarNoon());
    QCOMPARE(std::abs(d2.solarNoonTime().secsTo(d1.solarNoonTime())) < 1, true);
}
QTEST_MAIN(SunriseTest)
