/*
    SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <KWeatherCore/AlertEntry>
#include <KWeatherCore/AlertInfo>
#include <KWeatherCore/CAPArea>
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
        QCOMPARE(info.responseTypes(), KWeatherCore::AlertInfo::ResponseType::Shelter);
        QCOMPARE(info.contact(), QLatin1String("BARUFFALDI/JUSKIE"));

        QCOMPARE(info.areas().size(), 1);
        const auto area = info.areas()[0];
        QCOMPARE(area.description(),
                 QLatin1String("EXTREME NORTH CENTRAL TUOLUMNE COUNTY IN CALIFORNIA, EXTREME NORTHEASTERN CALAVERAS COUNTY IN CALIFORNIA, SOUTHWESTERN ALPINE "
                               "COUNTY IN CALIFORNIA"));
        QCOMPARE(area.polygons().size(), 1);
        QCOMPARE(area.polygons()[0].size(), 5);
        QCOMPARE(area.geoCodes().size(), 3);
        QCOMPARE(area.geoCodes()[0].first, QLatin1String("SAME"));
        QCOMPARE(area.geoCodes()[0].second, QLatin1String("006109"));
    }

    void testMultiArea()
    {
        QFile f(QFINDTESTDATA("capdata/multi-area.xml"));
        QVERIFY(f.open(QFile::ReadOnly));
        KWeatherCore::CAPParser parser(f.readAll());
        auto alert = parser.parse();

        QCOMPARE(alert.status(), KWeatherCore::AlertEntry::Status::Actual);
        QCOMPARE(alert.msgType(), KWeatherCore::AlertEntry::MsgType::Alert);
        QCOMPARE(alert.sender(), QLatin1String("vigilance@meteo.fr"));
        QCOMPARE(alert.sentTime(), QDateTime({2022, 8, 10}, {16, 12, 20}, Qt::OffsetFromUTC, +2 * 60 * 60));

        QCOMPARE(alert.infoVec().size(), 2);
        auto info = alert.infoVec()[0];
        QCOMPARE(info.language(), QLatin1String("fr-FR"));
        QCOMPARE(info.responseTypes(), KWeatherCore::AlertInfo::ResponseType::Monitor);
        QCOMPARE(info.web(), QLatin1String("http://vigilance.meteofrance.com/"));
        QCOMPARE(info.areas().size(), 4);
        auto area = info.areas()[3];
        QCOMPARE(area.description(), QLatin1String("Haute Garonne"));
        QCOMPARE(area.polygons().size(), 0);
        QCOMPARE(area.geoCodes().size(), 1);
        QCOMPARE(area.geoCodes()[0].first, QLatin1String("NUTS3"));
        QCOMPARE(area.geoCodes()[0].second, QLatin1String("FR623"));

        info = alert.infoVec()[1];
        QCOMPARE(info.language(), QLatin1String("en-GB"));
        QCOMPARE(info.parameter().size(), 2);
        QCOMPARE(info.parameter()[1].first, QLatin1String("awareness_type"));
        QCOMPARE(info.parameter()[1].second, QLatin1String("3; Thunderstorm"));
        QCOMPARE(info.areas().size(), 4);
        area = info.areas()[0];
        QCOMPARE(area.description(), QLatin1String("Alpes de Haute Provence"));
        QCOMPARE(area.polygons().size(), 0);
        QCOMPARE(area.geoCodes().size(), 1);
        QCOMPARE(area.geoCodes()[0].first, QLatin1String("NUTS3"));
        QCOMPARE(area.geoCodes()[0].second, QLatin1String("FR821"));
    }

    void testCircleArea()
    {
        QFile f(QFINDTESTDATA("capdata/tsunami.xml"));
        QVERIFY(f.open(QFile::ReadOnly));
        KWeatherCore::CAPParser parser(f.readAll());
        auto alert = parser.parse();

        QCOMPARE(alert.status(), KWeatherCore::AlertEntry::Status::Actual);
        QCOMPARE(alert.msgType(), KWeatherCore::AlertEntry::MsgType::Alert);
        QCOMPARE(alert.sender(), QLatin1String("ntwc@noaa.gov"));
        QCOMPARE(alert.sentTime(), QDateTime({2022, 8, 01}, {0, 40, 48}, Qt::OffsetFromUTC, 0));

        QCOMPARE(alert.infoVec().size(), 1);
        auto info = alert.infoVec()[0];
        QCOMPARE(info.language(), QLatin1String("en-US"));
        QCOMPARE(info.areas().size(), 1);
        auto area = info.areas()[0];
        QCOMPARE(area.polygons().size(), 0);
        QCOMPARE(area.circles().size(), 1);
        auto circle = area.circles()[0];
        QCOMPARE(circle.latitude, 53.401f);
        QCOMPARE(circle.longitude, -165.164f);
        QCOMPARE(circle.radius, 1.0f);
    }

    void testPolygon()
    {
        QFile f(QFINDTESTDATA("capdata/polygon-trailing-space.xml"));
        QVERIFY(f.open(QFile::ReadOnly));
        KWeatherCore::CAPParser parser(f.readAll());
        auto alert = parser.parse();

        QCOMPARE(alert.status(), KWeatherCore::AlertEntry::Status::Actual);
        QCOMPARE(alert.msgType(), KWeatherCore::AlertEntry::MsgType::Alert);
        QCOMPARE(alert.sender(), QLatin1String("info.aviso@inmet.gov.br"));
        QCOMPARE(alert.sentTime(), QDateTime({2022, 8, 12}, {13, 22, 45}, Qt::OffsetFromUTC, -3 * 60 * 60));

        QCOMPARE(alert.infoVec().size(), 1);
        auto info = alert.infoVec()[0];
        QCOMPARE(info.language(), QLatin1String("pt-BR"));
        QCOMPARE(info.areas().size(), 1);
        auto area = info.areas()[0];
        QCOMPARE(area.polygons().size(), 1);
        auto polygon = area.polygons()[0];
        QCOMPARE(polygon.size(), 10);
        for (const auto &p : polygon) {
            QVERIFY(p.first != 0.0f);
            QVERIFY(p.second != 0.0f);
        }
    }
};

QTEST_GUILESS_MAIN(CapParserTest)

#include "capparsertest.moc"
