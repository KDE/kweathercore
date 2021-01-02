/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "geotimezonetest.h"

void GeoTimezoneTest::testTimezone()
{
    // waiting for finished signal for 10 secs
    auto ret = finished_spy.wait(10000);

    if (ret)
        QCOMPARE(finished_spy.takeFirst().at(0).toString(), QStringLiteral("Asia/Oral"));
    else
        QVERIFY(networkError_spy.size() > 0);
}
QTEST_MAIN(GeoTimezoneTest)
