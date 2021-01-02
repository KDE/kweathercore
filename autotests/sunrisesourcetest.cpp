/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "sunrisesourcetest.h"

void SunriseSourceTest::initTestCase()
{
    // Hangzhou, which has static offset from UTC so we can hardcode it here
    d = new SunriseSource(30.29, 120.16, 3600 * 8);
    finished_spy = new QSignalSpy(d, &SunriseSource::finished);
    networkError_spy = new QSignalSpy(d, &SunriseSource::networkError);

    d->requestData();
}

void SunriseSourceTest::testFetch()
{
    auto ret = finished_spy->wait(30000);

    if (ret) {
        auto data = d->value();
        QVERIFY(data.size() > 0);

        auto count = finished_spy->size();

        // fetch again, this should return immediately
        d->requestData();
        QVERIFY(count < finished_spy->size());
    } else {
        QVERIFY(networkError_spy->count() > 0);
    }
}
QTEST_MAIN(SunriseSourceTest)
