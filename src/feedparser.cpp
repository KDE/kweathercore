/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "feedparser.h"
#include "kweathercore_p.h"
#include <QDateTime>
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
    const auto obj = configFile.object();

    m_url = QUrl(obj[QLatin1String("url")].toString());
    m_entryMarker = obj[QLatin1String("entry_marker")].toString();
    m_titleMarker = obj[QLatin1String("title")].toString();

    auto cap_it = obj.find(QLatin1String("cap"));
    if (cap_it != obj.end()) {
        m_hasCap = true;
        const auto cap_obj = cap_it->toObject();
        m_capLinkElementMarker = cap_obj[QLatin1String("element")].toString();
        m_capValueType = cap_obj[QLatin1String("type")].toString();
        m_capValueMarker = cap_obj[QLatin1String("value")].toString();
    } else {
        m_hasCap = false;
        m_capLinkElementMarker.clear();
        m_capValueMarker.clear();
        m_capValueType.clear();
    }
    m_summaryMarker = obj[QLatin1String("summary")].toString();

    auto area_it = obj.find(QLatin1String("area"));
    if (area_it != obj.end()) {
        m_hasArea = true;
        const auto area_obj = area_it->toObject();
        m_areaNameMarker = area_obj[QLatin1String("name")].toString();
        m_polygonMarker = area_obj[QLatin1String("polygon")].toString();
        m_FIPS6Marker = area_obj[QLatin1String("FIPS6")].toString();
        m_UGCMarker = area_obj[QLatin1String("UGC")].toString();
    } else {
        m_hasArea = false;
        m_areaNameMarker.clear();
        m_polygonMarker.clear();
        m_FIPS6Marker.clear();
        m_UGCMarker.clear();
    }

    m_urgencyMarker = obj[QLatin1String("urgency")].toString();
    m_severityMarker = obj[QLatin1String("severity")].toString();
    m_certaintyMarker = obj[QLatin1String("certainty")].toString();

    auto date_it = obj.find(QLatin1String("date"));
    if (date_it != obj.end()) {
        m_hasDate = true;
        const auto date_obj = date_it->toObject();
        m_dateFormat = date_obj[QLatin1String("format")].toString();
        m_dateMarker = date_obj[QLatin1String("element")].toString();
    } else {
        m_hasDate = false;
        m_dateFormat.clear();
        m_dateMarker.clear();
    }
}
std::unique_ptr<std::vector<std::unique_ptr<AlertFeedEntry>>> FeedParser::parse(const QByteArray &data) const
{
    QXmlStreamReader reader(data);
    std::unique_ptr<std::vector<std::unique_ptr<AlertFeedEntry>>> result(new std::vector<std::unique_ptr<AlertFeedEntry>>());

    while (!reader.atEnd()) {
        result->push_back(parseOneEntry(reader));
    }

    return result;
}
std::unique_ptr<AlertFeedEntry> FeedParser::parseOneEntry(QXmlStreamReader &reader) const
{
    std::unique_ptr<AlertFeedEntry> entry(new AlertFeedEntry);
    std::vector<std::pair<QString, QString>> areaCodes;
    while (reader.readNextStartElement()) {
        if (reader.name() == m_titleMarker) {
            entry->setTitle(reader.readElementText());
        } else if (reader.name() == m_summaryMarker) {
            entry->setSummary(reader.readElementText());
        } else if (reader.name() == m_urgencyMarker) {
            entry->setUrgency(KWeatherCorePrivate::urgencyStringToEnum(reader.readElementText()));
        } else if (reader.name() == m_severityMarker) {
            entry->setSeverity(KWeatherCorePrivate::severityStringToEnum(reader.readElementText()));
        } else if (reader.name() == m_certaintyMarker) {
            entry->setCertainty(KWeatherCorePrivate::certaintyStringToEnum(reader.readElementText()));
        } else if (m_hasCap && reader.name() == m_capLinkElementMarker) {
            entry->setUrl(parseCapElement(reader));
        } else if (m_hasArea && reader.name() == m_areaNameMarker) {
            entry->setArea(reader.readElementText());
        } else if (m_hasArea && reader.name() == m_polygonMarker) {
            parsePolygonElement(reader, *entry);
        } else if (m_hasArea && reader.name() == m_FIPS6Marker) {
            areaCodes.push_back({QStringLiteral("FIPS6"), reader.readElementText()});
        } else if (m_hasArea && reader.name() == m_UGCMarker) {
            areaCodes.push_back({QStringLiteral("UGC"), reader.readElementText()});
        } else if (m_hasDate && reader.name() == m_dateMarker) {
            if (m_dateFormat == QLatin1String("ISO-8601")) {
                entry->setDate(QDateTime::fromString(reader.readElementText(), Qt::ISODate));
            } else {
                entry->setDate(QDateTime::fromString(reader.readElementText(), m_dateFormat));
            }
        }
    }

    entry->setAreaCodes(std::move(areaCodes));
    return entry;
}
QUrl FeedParser::parseCapElement(QXmlStreamReader &reader) const
{
    Q_ASSERT(m_hasCap && reader.isStartElement() && reader.name() == m_capLinkElementMarker);

    if (m_capValueType == QLatin1String("attribute")) {
        return QUrl(reader.attributes().value(m_capValueMarker).toString());
    } else if (m_capValueType == QLatin1String("text")) {
        return QUrl(reader.readElementText());
    } else {
        return QUrl();
    }
}
void FeedParser::parsePolygonElement(QXmlStreamReader &reader, AlertFeedEntry &entry) const
{
    if (reader.name() == m_polygonMarker) {
        entry.setPolygon(KWeatherCorePrivate::stringToPolygon(reader.readElementText()));
    }
}
}

#include "moc_feedparser.cpp"
