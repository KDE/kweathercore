/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include <kweathercore/weatherforecastsource.h>

#include <QSignalSpy>
#include <QStandardPaths>
#include <QTest>

using namespace KWeatherCore;

class PendingWeatherForecastTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
    }

    void testFetch()
    {
        WeatherForecastSource d;
        auto p = d.requestData(50, 50);
        QSignalSpy finishedSpy(p, &PendingWeatherForecast::finished);
        QVERIFY(finishedSpy.wait(30000));
        if (p->error() == PendingWeatherForecast::NoError) {
            QVERIFY(p->value().dailyWeatherForecast().size() > 0);
        } else {
            QVERIFY(!p->errorMessage().isEmpty());
        }
        delete p;
    }
};

QTEST_GUILESS_MAIN(PendingWeatherForecastTest)

#include "pendingweatherforecasttest.moc"
