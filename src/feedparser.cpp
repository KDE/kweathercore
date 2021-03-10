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
}
