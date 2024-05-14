/*
    SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "metnoparser_p.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QTest>
#include <QTimeZone>

class MetNoParserTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
    }

    void testParseLocationForecast_data()
    {
        QTest::addColumn<QString>("inputFile");
        QTest::addColumn<QString>("timezone");

        QTest::newRow("berlin") << QFINDTESTDATA("met.no/berlin.json") << QStringLiteral("Europe/Berlin");
    }

    void testParseLocationForecast()
    {
        QFETCH(QString, inputFile);
        QFETCH(QString, timezone);

        QFile f(inputFile);
        QVERIFY(f.open(QFile::ReadOnly));
        KWeatherCore::MetNoParser p;
        p.forecast.setTimezone(timezone);
        p.parseLocationForecast(f.readAll());
        QVERIFY(p.hasData());
        p.applySunriseToForecast(QTimeZone(timezone.toUtf8()));

        auto forecastData = p.forecast.toJson();
        forecastData.remove(QLatin1String("createdTime")); // varies between runs
        forecastData.remove(QLatin1String("lat")); // not set
        forecastData.remove(QLatin1String("lon"));

        QFile ref(inputFile + QLatin1String(".ref"));
        QVERIFY(ref.open(QFile::ReadOnly));
        const auto refData = QJsonDocument::fromJson(ref.readAll()).object();
        if (refData != forecastData) {
            qDebug().noquote() << QJsonDocument(forecastData).toJson();
        }
        QCOMPARE(refData, forecastData);
    }
};

QTEST_GUILESS_MAIN(MetNoParserTest)

#include "metnoparsertest.moc"
