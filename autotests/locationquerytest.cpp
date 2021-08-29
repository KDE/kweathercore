/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "locationquerytest.h"

void LocationQueryTest::testLocate()
{
    qRegisterMetaType<LocationQueryResult>("LocationQueryResult");
    qRegisterMetaType<std::vector<LocationQueryResult>>("std::vector<LocationQueryResult>");
    finished_spy = new QSignalSpy(d, &LocationQuery::located);
    d->locate();

    auto ret = finished_spy->wait(30000);

    // if failed to located, abort
    if (!ret) {
        return;
    }

    auto location = finished_spy->takeFirst().at(0).value<LocationQueryResult>();
    QVERIFY(!location.name().isEmpty());
}

void LocationQueryTest::testQuery()
{
    d->query(QStringLiteral("London"));

    finished_spy = new QSignalSpy(d, &LocationQuery::queryFinished);
    networkError_spy = new QSignalSpy(d, &LocationQuery::queryError);

    auto ret = finished_spy->wait(30000);

    if (ret) {
        QVERIFY(finished_spy->takeFirst().at(0).value<std::vector<LocationQueryResult>>().size() > 0);
    } else {
        QVERIFY(networkError_spy->count() > 0);
    }
}
QTEST_MAIN(LocationQueryTest)
