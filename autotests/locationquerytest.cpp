/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include <kweathercore/locationquery.h>
#include <kweathercore/locationqueryreply.h>

#include <QSignalSpy>
#include <QTest>

using namespace KWeatherCore;

class LocationQueryTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testLocate()
    {
        LocationQuery queryMgr;
        auto reply = queryMgr.locate();
        QVERIFY(reply);

        QSignalSpy finished_spy(reply, &LocationQueryReply::finished);
        auto ret = finished_spy.wait(30000);

        // if failed to located, abort
        if (!ret) {
            return;
        }

        if (reply->error() == LocationQueryReply::NoError) {
            const auto location = reply->result().front();
            QVERIFY(!location.name().isEmpty());
        } else {
            QCOMPARE(reply->error(), LocationQueryReply::NoService);
        }

        reply->deleteLater();
    }

    void testQuery()
    {
        LocationQuery queryMgr;
        auto reply = queryMgr.query(QStringLiteral("London"));
        QVERIFY(reply);

        QSignalSpy finished_spy(reply, &LocationQueryReply::finished);
        QVERIFY(finished_spy.wait(30000));
        QVERIFY(reply->error() != LocationQueryReply::NoError || !reply->result().empty());
        reply->deleteLater();
    }
};

QTEST_MAIN(LocationQueryTest)

#include "locationquerytest.moc"
