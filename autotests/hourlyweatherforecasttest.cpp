/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include <kweathercore/hourlyweatherforecast.h>

#include <QJsonObject>
#include <QTest>

using namespace KWeatherCore;

class HourlyForecastTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testJson()
    {
        HourlyWeatherForecast d1(QDateTime::currentDateTime());

        auto d2 = HourlyWeatherForecast::fromJson(d1.toJson());

        QCOMPARE(d2.toJson(), d1.toJson());
    }

    void testWindDirectionCardinal_data()
    {
        QTest::addColumn<double>("degree");
        QTest::addColumn<WindDirection>("dir");

        QTest::addRow("0") << 0.0 << WindDirection::N;
        QTest::addRow("355") << 355.0 << WindDirection::N;
        QTest::addRow("360") << 360.0 << WindDirection::N;
        QTest::addRow("10") << 10.0 << WindDirection::N;
        QTest::addRow("22.4") << 22.4 << WindDirection::N;
        QTest::addRow("22.6") << 22.6 << WindDirection::NE;
        QTest::addRow("90") << 90.0 << WindDirection::E;
        QTest::addRow("135") << 135.0 << WindDirection::SE;
        QTest::addRow("280") << 280.0 << WindDirection::W;
    }

    void testWindDirectionCardinal()
    {
        QFETCH(double, degree);
        QFETCH(WindDirection, dir);
        HourlyWeatherForecast f;
        f.setWindDirectionDegree(degree);
        QCOMPARE(f.windDirectionCardinal(), dir);
    }
};
QTEST_APPLESS_MAIN(HourlyForecastTest)

#include "hourlyweatherforecasttest.moc"
