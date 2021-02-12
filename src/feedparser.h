/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include <QJsonDocument>
#include <QXmlStreamReader>
#include <memory>
#include "alertfeedentry.h"
namespace KWeatherCore {
class FeedParser : public QObject
{
    Q_OBJECT
public:
    FeedParser(const QJsonDocument &configFile, QObject *parent = nullptr);
    void setConfig(const QJsonDocument &configFile);
    std::unique_ptr<std::vector<AlertFeedEntry>> parse(const QByteArray &data) const;

private:
    AlertFeedEntry parseOneEntry(QXmlStreamReader &reader) const;

    QUrl m_url;
    QString m_entryMarker;
    QString m_titleMarker;
    bool m_hasCap;
    QString m_capLinkElementMarker;
    QString m_capValueType; // ie.  "attribute"
    QString m_capValueMarker; // ie. "href"
    QString m_summaryMarker;
    bool m_hasArea;
    QString m_areaNameMarker;
    QString m_polygonMarker;
    QString m_FIPS6Marker;
    QString m_UGCMarker;
    QString m_urgencyMarker;
    QString m_severityMarker;
    QString m_certaintyMarker;
    bool m_hasDate;
    QString m_dateMarker;
    QString m_dateFormat;
};
}
