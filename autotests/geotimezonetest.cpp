/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "geotimezonetest.h"

#include <kweathercore/geotimezone.h>

#include <QNetworkAccessManager>

using namespace KWeatherCore;

void GeoTimezoneTest::testTimezone()
{
    QNetworkAccessManager nam;
    GeoTimezone d{&nam, 50, 50};
    QSignalSpy finished_spy{&d, &GeoTimezone::finished};
    QSignalSpy networkError_spy{&d, &GeoTimezone::networkErrorOccured};

    // waiting for finished signal for 10 secs
    auto ret = finished_spy.wait(10000);

    if (ret) {
        QCOMPARE(finished_spy.takeFirst().at(0).toString(), QStringLiteral("Asia/Oral"));
    } else {
        QVERIFY(networkError_spy.size() > 0);
    }
}
QTEST_MAIN(GeoTimezoneTest)
