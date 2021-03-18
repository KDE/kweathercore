/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "feedparser.h"
#include <QJsonObject>
namespace KWeatherCore
{
FeedParser::FeedParser(const QJsonDocument &configFile, QObject *parent)
    : QObject(parent)
{
    setConfig(configFile);
}

void FeedParser::setConfig(const QJsonDocument &configFile)
{
    auto obj = configFile.object();

    m_url = QUrl(obj[QStringLiteral("url")].toString());
    m_entryMarker = obj[QStringLiteral("entry_marker")].toString();
    m_titleMarker = obj[QStringLiteral("title")].toString();

    auto cap_it = obj.find(QStringLiteral("cap"));
    if (cap_it != obj.end()) {
        m_hasCap = true;
        auto cap_obj = cap_it->toObject();
        m_capLinkElementMarker = cap_obj[QStringLiteral("element")].toString();
        m_capValueType = cap_obj[QStringLiteral("type")].toString();
        m_capValueMarker = cap_obj[QStringLiteral("value")].toString();
    } else {
        m_hasCap = false;
        m_capLinkElementMarker.clear();
        m_capValueMarker.clear();
        m_capValueType.clear();
    }
    m_summaryMarker = obj[QStringLiteral("summary")].toString();

    auto area_it = obj.find(QStringLiteral("area"));
    if (area_it != obj.end()) {
        m_hasArea = true;
        auto area_obj = area_it->toObject();
        m_areaNameMarker = area_obj[QStringLiteral("name")].toString();
        m_polygonMarker = area_obj[QStringLiteral("polygon")].toString();
        m_FIPS6Marker = area_obj[QStringLiteral("FIPS6")].toString();
        m_UGCMarker = area_obj[QStringLiteral("UGC")].toString();
    } else {
        m_hasArea = false;
        m_areaNameMarker.clear();
        m_polygonMarker.clear();
        m_FIPS6Marker.clear();
        m_UGCMarker.clear();
    }

    m_urgencyMarker = obj[QStringLiteral("urgency")].toString();
    m_severityMarker = obj[QStringLiteral("severity")].toString();
    m_certaintyMarker = obj[QStringLiteral("certainty")].toString();

    auto date_it = obj.find(QStringLiteral("date"));
    if (date_it != obj.end()) {
        m_hasDate = true;
        auto date_obj = date_it->toObject();
        m_dateFormat = date_obj[QStringLiteral("format")].toString();
        m_dateMarker = date_obj[QStringLiteral("element")].toString();
    } else {
        m_hasDate = false;
        m_dateFormat.clear();
        m_dateMarker.clear();
    }
}

std::unique_ptr<AlertFeedEntry>
FeedParser::parseOneEntry(QXmlStreamReader &reader) const
{
    std::unique_ptr<AlertFeedEntry> entry(new AlertFeedEntry);
    std::vector<std::pair<QString, QString>> areaCodes;
    while (reader.readNextStartElement()) {
        if (reader.name() == m_titleMarker) {
            entry->setTitle(reader.readElementText());
        } else if (reader.name() == m_summaryMarker) {
            entry->setSummary(reader.readElementText());
        } else if (reader.name() == m_urgencyMarker) {
            entry->setUrgency(urgencyStrToEnum(reader.readElementText()));
        } else if (reader.name() == m_severityMarker) {
            entry->setSeverity(severityStrToEnum(reader.readElementText()));
        } else if (reader.name() == m_certaintyMarker) {
            entry->setCertainty(certaintyStrToEnum(reader.readElementText()));
        } else if (m_hasCap && reader.name() == m_capLinkElementMarker) {
            entry->setUrl(parseCapElement(reader));
        } else if (m_hasArea && reader.name() == m_areaNameMarker) {
            entry->setArea(reader.readElementText());
        } else if (m_hasArea && reader.name() == m_polygonMarker) {
            parsePolygonElement(reader, *entry);
        } else if (m_hasArea && reader.name() == m_FIPS6Marker) {
            areaCodes.push_back(
                {QStringLiteral("FIPS6"), reader.readElementText()});
        } else if (m_hasArea && reader.name() == m_UGCMarker) {
            areaCodes.push_back(
                {QStringLiteral("UGC"), reader.readElementText()});
        }
        // TODO: parse date element
    }

    entry->setAreaCodes(areaCodes);
    return entry;
}
AlertInfo::Urgency FeedParser::urgencyStrToEnum(const QString &str)
{
    if (str == QStringLiteral("Immediate"))
        return AlertInfo::Urgency::Immediate;
    else if (str == QStringLiteral("Expected"))
        return AlertInfo::Urgency::Expected;
    else if (str == QStringLiteral("Future"))
        return AlertInfo::Urgency::Future;
    else if (str == QStringLiteral("Past"))
        return AlertInfo::Urgency::Past;
    else
        return AlertInfo::Urgency::Unknown;
}
AlertInfo::Severity FeedParser::severityStrToEnum(const QString &str)
{
    if (str == QStringLiteral("Extreme"))
        return AlertInfo::Severity::Extreme;
    else if (str == QStringLiteral("Severe"))
        return AlertInfo::Severity::Severe;
    else if (str == QStringLiteral("Moderate"))
        return AlertInfo::Severity::Moderate;
    else if (str == QStringLiteral("Minor"))
        return AlertInfo::Severity::Minor;
    else
        return AlertInfo::Severity::Unknown;
}
AlertInfo::Certainty FeedParser::certaintyStrToEnum(const QString &str)
{
    if (str == QStringLiteral("Observed"))
        return AlertInfo::Certainty::Observed;
    else if (str == QStringLiteral("Likely"))
        return AlertInfo::Certainty::Likely;
    else if (str == QStringLiteral("Possible"))
        return AlertInfo::Certainty::Possible;
    else if (str == QStringLiteral("Unlikely"))
        return AlertInfo::Certainty::Unlikely;
    else
        return AlertInfo::Certainty::Unknown;
}
QUrl FeedParser::parseCapElement(QXmlStreamReader &reader) const
{
    Q_ASSERT(m_hasCap && reader.isStartElement() &&
             reader.name() == m_capLinkElementMarker);

    if (m_capValueType == QStringLiteral("attribute")) {
        return QUrl(reader.attributes().value(m_capValueMarker).toString());
    } else if (m_capValueType == QStringLiteral("text"))
        return QUrl(reader.readElementText());
    else
        return QUrl();
}
void FeedParser::parsePolygonElement(QXmlStreamReader &reader,
                                     AlertFeedEntry &entry) const
{
    using Polygon = std::vector<std::pair<float, float>>;
    static auto stringToPolygon = [](const QString &str) -> Polygon {
        Polygon res;
        const auto pairList = str.split(QLatin1Char(' '));
        for (auto &pair : pairList) {
            auto coordinate = pair.split(QLatin1Char(','));
            res.push_back(
                {coordinate.front().toFloat(), coordinate.back().toFloat()});
        }
        return res;
    };
    if (reader.name() == m_polygonMarker) {
        entry.setPolygon(stringToPolygon(reader.readElementText()));
    }
}
}
