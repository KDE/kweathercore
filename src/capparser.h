/*
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "alertentry.h"
#include "alertinfo.h"
#include "kweathercore/kweathercore_export.h"

#include <QObject>
#include <QXmlStreamReader>
#include <QHash>

#include <memory>

namespace KWeatherCore
{
class KWEATHERCORE_EXPORT CAPParser
{
    Q_GADGET
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
    inline const static QHash<QString, Tags> tags {
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

    inline const static QHash<QString, InfoTags> infoTags = {
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
    };
};
}
