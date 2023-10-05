/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "geotimezonetest.h"

#include <kweathercore/geotimezone.h>

#include <QNetworkAccessManager>
#include <QStandardPaths>

using namespace KWeatherCore;

void GeoTimezoneTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
}

void GeoTimezoneTest::testTimezone()
{
    QNetworkAccessManager nam;
    GeoTimezone d{&nam, 50, 50};
    QSignalSpy finished_spy{&d, &GeoTimezone::finished};

    // waiting for finished signal for 10 secs
    QVERIFY(finished_spy.wait(10000));
    if (d.error() == GeoTimezone::NoError) {
        QCOMPARE(d.timezone(), QStringLiteral("Asia/Oral"));
    }
}
QTEST_MAIN(GeoTimezoneTest)

#include "moc_geotimezonetest.cpp"
