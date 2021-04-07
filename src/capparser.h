/*
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "alertentry.h"
#include "alertinfo.h"

#include <QObject>
#include <QXmlStreamReader>
#include <QMap>

#include <memory>

namespace KWeatherCore
{
class CAPParser
{
    Q_OBJECT
public:
    explicit CAPParser(const QByteArray &data = {});
    std::unique_ptr<AlertEntry> parse();

private:
    AlertInfo parseInfo();
    AlertInfo::Urgency getUrgency(const QString &);
    AlertInfo::Severity getSeverity(const QString &);
    AlertInfo::Certainty getCertainity(const QString &);
    QXmlStreamReader m_xml;
    enum class Tags {
        ALERT,
        IDENTIFIER,
        SENDER,
        SENT_TIME,
        STATUS,
        MSG_TYPE,
        SCOPE,
        NOTE,
        INFO
    };
    QMap<QString, Tags> tags = {
        {QStringLiteral("alert"), Tags::ALERT},
        {QStringLiteral("identifier"), Tags::IDENTIFIER},
        {QStringLiteral("sender"), Tags::SENDER},
        {QStringLiteral("sent"), Tags::SENT_TIME},
        {QStringLiteral("msgType"), Tags::MSG_TYPE},
        {QStringLiteral("status"), Tags::STATUS},
        {QStringLiteral("scope"), Tags::SCOPE},
        {QStringLiteral("note"), Tags::NOTE},
        {QStringLiteral("info"), Tags::INFO}};

    enum class InfoTags {
        HEADLINE,
        DESCRIPTION,
        EVENT,
        AREA_NAMES,
        EFFECTIVE_TIME,
        EXPIRE_TIME,
        CATEGORY,
        INSTRUCTION,
        URGENCY,
        SEVERITY,
        CERTAINITY,
        PARAMETER,
        AREA,
    };

    QMap<QString, InfoTags> infoTags = {
        {QStringLiteral("category"), InfoTags::CATEGORY},
        {QStringLiteral("event"), InfoTags::EVENT},
        {QStringLiteral("urgency"), InfoTags::URGENCY},
        {QStringLiteral("severity"), InfoTags::SEVERITY},
        {QStringLiteral("certainity"), InfoTags::CERTAINITY},
        {QStringLiteral("effective"), InfoTags::EFFECTIVE_TIME},
        {QStringLiteral("expire"), InfoTags::EXPIRE_TIME},
        {QStringLiteral("headline"), InfoTags::HEADLINE},
        {QStringLiteral("description"), InfoTags::DESCRIPTION},
        {QStringLiteral("instruction"), InfoTags::INSTRUCTION},
        {QStringLiteral("event"), InfoTags::EVENT},
    };
};
}
