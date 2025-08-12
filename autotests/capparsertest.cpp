/*
    SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <KWeatherCore/CAPAlertInfo>
#include <KWeatherCore/CAPAlertMessage>
#include <KWeatherCore/CAPArea>
#include <KWeatherCore/CAPParser>
#include <KWeatherCore/CAPReference>
#include <KWeatherCore/CAPResource>

#include <QFile>
#include <QTest>

using namespace Qt::Literals;

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

        QCOMPARE(alert.status(), KWeatherCore::CAPAlertMessage::Status::Actual);
        QCOMPARE(alert.messageType(), KWeatherCore::CAPAlertMessage::MessageType::Alert);
        QCOMPARE(alert.identifier(), QLatin1String("KSTO1055887203"));
        QCOMPARE(alert.sender(), QLatin1String("KSTO@NWS.NOAA.GOV"));
        QCOMPARE(alert.sentTime(), QDateTime({2003, 06, 17}, {14, 57}, QTimeZone::fromSecondsAheadOfUtc(-7 * 60 * 60)));
        QCOMPARE(alert.scope(), KWeatherCore::CAPAlertMessage::Scope::Public);
        QCOMPARE(alert.note(), QString());

        QCOMPARE(alert.alertInfos().size(), 1);
        const auto &info = alert.alertInfos()[0];

        QCOMPARE(info.event(), QLatin1String("SEVERE THUNDERSTORM"));
        QCOMPARE(info.effectiveTime(), QDateTime());
        QCOMPARE(info.onsetTime(), QDateTime());
        QCOMPARE(info.expireTime(), QDateTime({2003, 06, 17}, {16, 0}, QTimeZone::fromSecondsAheadOfUtc(-7 * 60 * 60)));
        QCOMPARE(info.headline(), QLatin1String("SEVERE THUNDERSTORM WARNING"));
        QCOMPARE(info.description().isEmpty(), false);
        QCOMPARE(info.instruction().isEmpty(), false);
        QCOMPARE(info.sender(), QLatin1String("NATIONAL WEATHER SERVICE SACRAMENTO CA"));
        QCOMPARE(info.language(), QLatin1String("en-US"));
        QCOMPARE(info.categories(), KWeatherCore::CAPAlertInfo::Category::Meteorological);
        QCOMPARE(info.urgency(), KWeatherCore::CAPAlertInfo::Urgency::Immediate);
        QCOMPARE(info.severity(), KWeatherCore::CAPAlertInfo::Severity::Severe);
        QCOMPARE(info.certainty(), KWeatherCore::CAPAlertInfo::Certainty::Observed);
        QCOMPARE(info.responseTypes(), KWeatherCore::CAPAlertInfo::ResponseType::Shelter);
        QCOMPARE(info.contact(), QLatin1String("BARUFFALDI/JUSKIE"));

        QCOMPARE(info.hasGeometry(), true);
        QCOMPARE(info.areas().size(), 1);
        const auto area = info.areas()[0];
        QCOMPARE(area.description(),
                 QLatin1String("EXTREME NORTH CENTRAL TUOLUMNE COUNTY IN CALIFORNIA, EXTREME NORTHEASTERN CALAVERAS COUNTY IN CALIFORNIA, SOUTHWESTERN ALPINE "
                               "COUNTY IN CALIFORNIA"));
        QCOMPARE(area.polygons().size(), 1);
        QCOMPARE(area.polygons()[0].size(), 5);
        QCOMPARE(area.geoCodes().size(), 3);
        QCOMPARE(area.geoCodes()[0].name, QLatin1String("SAME"));
        QCOMPARE(area.geoCodes()[0].value, QLatin1String("006109"));
        QCOMPARE(area.geoCodes()[1].name, QLatin1String("SAME"));
        QCOMPARE(area.geoCodes()[1].value, QLatin1String("006009"));

        QCOMPARE(info.eventCodes().size(), 1);
        QCOMPARE(info.eventCodes()[0].name, QLatin1String("SAME"));
        QCOMPARE(info.eventCodes()[0].value, QLatin1String("SVR"));
    }

    void testMultiArea()
    {
        QFile f(QFINDTESTDATA("capdata/multi-area.xml"));
        QVERIFY(f.open(QFile::ReadOnly));
        KWeatherCore::CAPParser parser(f.readAll());
        auto alert = parser.parse();

        QCOMPARE(alert.status(), KWeatherCore::CAPAlertMessage::Status::Actual);
        QCOMPARE(alert.messageType(), KWeatherCore::CAPAlertMessage::MessageType::Alert);
        QCOMPARE(alert.sender(), QLatin1String("vigilance@meteo.fr"));
        QCOMPARE(alert.sentTime(), QDateTime({2022, 8, 10}, {16, 12, 20}, QTimeZone::fromSecondsAheadOfUtc(+2 * 60 * 60)));

        QCOMPARE(alert.alertInfos().size(), 2);
        auto info = alert.alertInfos()[0];
        QCOMPARE(info.language(), QLatin1String("fr-FR"));
        QCOMPARE(info.responseTypes(), KWeatherCore::CAPAlertInfo::ResponseType::Monitor);
        QCOMPARE(info.web(), QLatin1String("http://vigilance.meteofrance.com/"));
        QCOMPARE(info.areas().size(), 4);
        auto area = info.areas()[3];
        QCOMPARE(area.description(), QLatin1String("Haute Garonne"));
        QCOMPARE(area.polygons().size(), 0);
        QCOMPARE(area.geoCodes().size(), 1);
        QCOMPARE(area.geoCodes()[0].name, QLatin1String("NUTS3"));
        QCOMPARE(area.geoCodes()[0].value, QLatin1String("FR623"));

        info = alert.alertInfos()[1];
        QCOMPARE(info.language(), QLatin1String("en-GB"));
        QCOMPARE(info.parameters().size(), 2);
        QCOMPARE(info.parameters()[1].name, QLatin1String("awareness_type"));
        QCOMPARE(info.parameters()[1].value, QLatin1String("3; Thunderstorm"));
        QCOMPARE(info.areas().size(), 4);
        area = info.areas()[0];
        QCOMPARE(area.description(), QLatin1String("Alpes de Haute Provence"));
        QCOMPARE(area.polygons().size(), 0);
        QCOMPARE(area.geoCodes().size(), 1);
        QCOMPARE(area.geoCodes()[0].name, QLatin1String("NUTS3"));
        QCOMPARE(area.geoCodes()[0].value, QLatin1String("FR821"));
    }

    void testCircleArea()
    {
        QFile f(QFINDTESTDATA("capdata/tsunami.xml"));
        QVERIFY(f.open(QFile::ReadOnly));
        KWeatherCore::CAPParser parser(f.readAll());
        auto alert = parser.parse();

        QCOMPARE(alert.status(), KWeatherCore::CAPAlertMessage::Status::Actual);
        QCOMPARE(alert.messageType(), KWeatherCore::CAPAlertMessage::MessageType::Alert);
        QCOMPARE(alert.sender(), QLatin1String("ntwc@noaa.gov"));
        QCOMPARE(alert.sentTime(), QDateTime({2022, 8, 01}, {0, 40, 48}, QTimeZone::fromSecondsAheadOfUtc(0)));

        QCOMPARE(alert.alertInfos().size(), 1);
        auto info = alert.alertInfos()[0];
        QCOMPARE(info.language(), QLatin1String("en-US"));
        QCOMPARE(info.areas().size(), 1);
        QCOMPARE(info.hasGeometry(), true);
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

        QCOMPARE(alert.status(), KWeatherCore::CAPAlertMessage::Status::Actual);
        QCOMPARE(alert.messageType(), KWeatherCore::CAPAlertMessage::MessageType::Alert);
        QCOMPARE(alert.sender(), QLatin1String("info.aviso@inmet.gov.br"));
        QCOMPARE(alert.sentTime(), QDateTime({2022, 8, 12}, {13, 22, 45}, QTimeZone::fromSecondsAheadOfUtc(-3 * 60 * 60)));

        QCOMPARE(alert.alertInfos().size(), 1);
        auto info = alert.alertInfos()[0];
        QCOMPARE(info.hasGeometry(), true);
        QCOMPARE(info.language(), QLatin1String("pt-BR"));
        QCOMPARE(info.areas().size(), 1);
        auto area = info.areas()[0];
        QCOMPARE(area.polygons().size(), 1);
        auto polygon = area.polygons()[0];
        QCOMPARE(polygon.size(), 10);
        for (const auto &p : polygon) {
            QVERIFY(p.latitude != 0.0f);
            QVERIFY(p.longitude != 0.0f);
        }
    }

    void testReferences()
    {
        QFile f(QFINDTESTDATA("capdata/dwd-update.xml"));
        QVERIFY(f.open(QFile::ReadOnly));
        KWeatherCore::CAPParser parser(f.readAll());
        auto alert = parser.parse();

        QCOMPARE(alert.status(), KWeatherCore::CAPAlertMessage::Status::Actual);
        QCOMPARE(alert.messageType(), KWeatherCore::CAPAlertMessage::MessageType::Update);
        QCOMPARE(alert.references().size(), 1);
        auto ref = alert.references()[0];
        QCOMPARE(ref.sender(), QLatin1String("opendata@dwd.de"));
        QCOMPARE(ref.identifier(), QLatin1String("2.49.0.0.276.0.DWD.PVW.1661542140000.7462e657-ce54-473f-91c8-abe408afe703.MUL"));
        QCOMPARE(ref.sent(), QDateTime({2022, 8, 26}, {21, 29}, QTimeZone::fromSecondsAheadOfUtc(2 * 60 * 60)));

        ref = alert.ownReference();
        QCOMPARE(ref.sender(), QLatin1String("opendata@dwd.de"));
        QCOMPARE(ref.identifier(), QLatin1String("2.49.0.0.276.0.DWD.PVW.1661544180000.6ffa85ac-a5ed-4e69-977c-e767a423ecd6.MUL"));
        QCOMPARE(ref.sent(), QDateTime({2022, 8, 26}, {22, 3}, QTimeZone::fromSecondsAheadOfUtc(2 * 60 * 60)));

        QCOMPARE(alert.alertInfos().size(), 1);
        const auto info = alert.alertInfos()[0];
        QCOMPARE(info.areas().size(), 2);
        const auto area = info.areas()[0];
        QCOMPARE(area.altitude(), 0.0f);
        QCOMPARE(area.ceiling(), 9842.5197f);
    }

    void testTabCoordinateSeparator()
    {
        QFile f(QFINDTESTDATA("capdata/mo-cap_monsoon.xml"));
        QVERIFY(f.open(QFile::ReadOnly));
        KWeatherCore::CAPParser parser(f.readAll());
        auto alert = parser.parse();

        QCOMPARE(alert.status(), KWeatherCore::CAPAlertMessage::Status::Actual);
        QCOMPARE(alert.messageType(), KWeatherCore::CAPAlertMessage::MessageType::Update);
        QCOMPARE(alert.references().size(), 1);
        auto ref = alert.references()[0];
        QCOMPARE(ref.sender(), "meteo@smg.gov.mo"_L1);
        QCOMPARE(ref.identifier(), "SMG-Weather_MS_2025_006_03"_L1);
        QCOMPARE(ref.sent(), QDateTime({2025, 4, 13}, {5, 52, 12}, QTimeZone::fromSecondsAheadOfUtc(8 * 60 * 60)));

        QCOMPARE(alert.alertInfos().size(), 3);
        const auto info = alert.alertInfos()[0];
        QCOMPARE(info.hasGeometry(), true);
        QCOMPARE(info.areas().size(), 1);
        const auto area = info.areas()[0];
        QCOMPARE(area.description(), u"澳門特別行政區行政區域圖");
        QCOMPARE(area.polygons().size(), 1);
        const auto poly = area.polygons()[0];
        QCOMPARE(poly.size(), 63);
    }

    void testEmptyPolygon()
    {
        QFile f(QFINDTESTDATA("capdata/il-488abaaf-cad5-4d2b-93b2-703016878453.xml"));
        QVERIFY(f.open(QFile::ReadOnly));
        KWeatherCore::CAPParser parser(f.readAll());
        auto alert = parser.parse();

        QCOMPARE(alert.status(), KWeatherCore::CAPAlertMessage::Status::Actual);
        QCOMPARE(alert.alertInfos().size(), 2);
        const auto info = alert.alertInfos()[0];
        QCOMPARE(info.hasGeometry(), false);
        QCOMPARE(info.areas().size(), 3);
        const auto area = info.areas()[0];
        QVERIFY(!area.description().isEmpty());
        QCOMPARE(area.geoCodes().size(), 1);
        QCOMPARE(area.polygons().size(), 0);
    }

    void testResources()
    {
        QFile f(QFINDTESTDATA("capdata/431022.cap"));
        QVERIFY(f.open(QFile::ReadOnly));
        KWeatherCore::CAPParser parser(f.readAll());
        auto alert = parser.parse();

        QCOMPARE(alert.alertInfos().size(), 1);
        const auto info = alert.alertInfos()[0];

        QCOMPARE(info.resources().size(), 3);
        auto res = info.resources()[0];
        QCOMPARE(res.description(), "Stage 2 Water Restriction Details"_L1);
        QCOMPARE(res.mimeTypeName(), "application/pdf"_L1);
        QCOMPARE(res.uri().toString(), "https://alerts.alertable.ca/alerts/attach/21-20250609181149.pdf"_L1);
        QCOMPARE(res.size(), 370550);
        QVERIFY(res.hasSize());
        QCOMPARE(res.digest(), QByteArray::fromHex("d4cff1ae5902b9d1b97164bb07ee20ab59029906"));

        res = info.resources()[1];
        QCOMPARE(res.mimeTypeName(), "image/png"_L1);

        res = info.resources()[2];
        QCOMPARE(res.mimeTypeName(), "image/jpeg"_L1);
        QVERIFY(res.digest().isEmpty());
        QCOMPARE(res.size(), -1);
        QVERIFY(!res.hasSize());
    }
};

QTEST_GUILESS_MAIN(CapParserTest)

#include "capparsertest.moc"
