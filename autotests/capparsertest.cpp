/*
    SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <KWeatherCore/AlertEntry>
#include <KWeatherCore/AlertInfo>
#include <KWeatherCore/CAPParser>

#include <QFile>
#include <QTest>

class CapParserTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testParseCap()
    {
        QFile f(QFINDTESTDATA("capdata/thunderstorm.xml"));
        QVERIFY(f.open(QFile::ReadOnly));
        KWeatherCore::CAPParser parser(f.readAll());
        auto alert = parser.parse();

        QCOMPARE(alert.status(), KWeatherCore::AlertEntry::Status::Actual);
        QCOMPARE(alert.msgType(), KWeatherCore::AlertEntry::MsgType::Alert);
        QCOMPARE(alert.identifier(), QLatin1String("KSTO1055887203"));
        QCOMPARE(alert.sender(), QLatin1String("KSTO@NWS.NOAA.GOV"));
        QCOMPARE(alert.sentTime(), QDateTime({2003, 06, 17}, {14, 57}, Qt::OffsetFromUTC, -7 * 60 * 60));
        QCOMPARE(alert.scope(), KWeatherCore::AlertEntry::Scope::Public);
        QCOMPARE(alert.note(), QString());

        QCOMPARE(alert.infoVec().size(), 1);
        const auto &info = alert.infoVec()[0];

        QCOMPARE(info.event(), QLatin1String("SEVERE THUNDERSTORM"));
        // TODO area codes
        QCOMPARE(info.effectiveTime(), QDateTime());
        QCOMPARE(info.onsetTime(), QDateTime());
        QCOMPARE(info.expireTime(), QDateTime({2003, 06, 17}, {16, 0}, Qt::OffsetFromUTC, -7 * 60 * 60));
        QCOMPARE(info.headline(), QLatin1String("SEVERE THUNDERSTORM WARNING"));
        QCOMPARE(info.description().isEmpty(), false);
        QCOMPARE(info.instruction().isEmpty(), false);
        QCOMPARE(info.sender(), QLatin1String("NATIONAL WEATHER SERVICE SACRAMENTO CA"));
        QCOMPARE(info.language(), QLatin1String("en-US"));
        QCOMPARE(info.categories(), KWeatherCore::AlertInfo::Category::Met);
        QCOMPARE(info.urgency(), KWeatherCore::AlertInfo::Urgency::Immediate);
        QCOMPARE(info.severity(), KWeatherCore::AlertInfo::Severity::Severe);
        QCOMPARE(info.certainty(), KWeatherCore::AlertInfo::Certainty::Observed);
        // TODO parameter
        // TODO areaDesc
        // TODO areaPolygon
    }
};

QTEST_GUILESS_MAIN(CapParserTest)

#include "capparsertest.moc"
