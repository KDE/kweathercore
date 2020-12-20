/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "sunrisetest.h"

void SunriseTest::testJson()
{
    auto date = QDateTime::currentDateTime();
    d1 = Sunrise(date, date.addSecs(3600 * 12), date.addSecs(3600 * 13), date.addSecs(3600 * 23), {date, 10}, {date, 10}, {date, 10}, {date, 10}, 0.5);
    d2 = Sunrise::fromJson(d1.toJson());

    QCOMPARE(d2.highMoon(), d1.highMoon());
    QCOMPARE(std::abs(d2.highMoonTime().secsTo(d1.highMoonTime())) < 1, true);
    QCOMPARE(d2.lowMoon(), d1.lowMoon());
    QCOMPARE(std::abs(d2.lowMoonTime().secsTo(d1.lowMoonTime())) < 1, true);
    QCOMPARE(d2.moonPhase(), d1.moonPhase());
    QCOMPARE(std::abs(d2.moonRise().secsTo(d1.moonRise())) < 1, true);
    QCOMPARE(std::abs(d2.moonSet().secsTo(d1.moonSet())) < 1, true);
    QCOMPARE(d2.solarMidnight(), d1.solarMidnight());
    QCOMPARE(std::abs(d2.solarMidnightTime().secsTo(d1.solarMidnightTime())) < 1, true);
    QCOMPARE(d2.solarNoon(), d2.solarNoon());
    QCOMPARE(std::abs(d2.solarNoonTime().secsTo(d1.solarNoonTime())) < 1, true);
}
QTEST_MAIN(SunriseTest)
