/*
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "alertentry.h"
#include "alertinfo.h"

#include <QHash>
#include <QObject>
#include <QXmlStreamReader>

#include <memory>

namespace KWeatherCore
{
class CAPParser
{
public:
    explicit CAPParser(const QByteArray &data = {});
    void setData(const QByteArray &data);
    std::unique_ptr<AlertEntry> parse();

private:
    AlertInfo parseInfo();
    QXmlStreamReader m_xml;
    enum class Tags { ALERT, IDENTIFIER, SENDER, SENT_TIME, STATUS, MSG_TYPE, SCOPE, NOTE, INFO };
    inline const static QHash<QString, Tags> tags{{QStringLiteral("alert"), Tags::ALERT},
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
        ONSET_TIME,
        EXPIRE_TIME,
        CATEGORY,
        INSTRUCTION,
        URGENCY,
        SEVERITY,
        CERTAINITY,
        PARAMETER,
        AREA,
        SENDERNAME,
        LANGUAGE
    };

    inline const static QHash<QString, InfoTags> infoTags = {{QStringLiteral("category"), InfoTags::CATEGORY},
                                                             {QStringLiteral("event"), InfoTags::EVENT},
                                                             {QStringLiteral("urgency"), InfoTags::URGENCY},
                                                             {QStringLiteral("severity"), InfoTags::SEVERITY},
                                                             {QStringLiteral("certainty"), InfoTags::CERTAINITY},
                                                             {QStringLiteral("effective"), InfoTags::EFFECTIVE_TIME},
                                                             {QStringLiteral("onset"), InfoTags::ONSET_TIME},
                                                             {QStringLiteral("expires"), InfoTags::EXPIRE_TIME},
                                                             {QStringLiteral("headline"), InfoTags::HEADLINE},
                                                             {QStringLiteral("description"), InfoTags::DESCRIPTION},
                                                             {QStringLiteral("instruction"), InfoTags::INSTRUCTION},
                                                             {QStringLiteral("area"), InfoTags::AREA},
                                                             {QStringLiteral("senderName"), InfoTags::SENDERNAME},
                                                             {QStringLiteral("language"), InfoTags::LANGUAGE}};

    // constexpr hashmap when?
    inline const static QHash<QString, AlertInfo::Category> categoryMap = {{QStringLiteral("Geo"), AlertInfo::Category::Geo},
                                                                           {QStringLiteral("Met"), AlertInfo::Category::Met},
                                                                           {QStringLiteral("Safety"), AlertInfo::Category::Safety},
                                                                           {QStringLiteral("Security"), AlertInfo::Category::Security},
                                                                           {QStringLiteral("Rescue"), AlertInfo::Category::Rescue},
                                                                           {QStringLiteral("Fire"), AlertInfo::Category::Fire},
                                                                           {QStringLiteral("Health"), AlertInfo::Category::Health},
                                                                           {QStringLiteral("Env"), AlertInfo::Category::Env},
                                                                           {QStringLiteral("Transport"), AlertInfo::Category::Transport},
                                                                           {QStringLiteral("Infra"), AlertInfo::Category::Infra},
                                                                           {QStringLiteral("CBRNE"), AlertInfo::Category::CBRNE},
                                                                           {QStringLiteral("Other"), AlertInfo::Category::Other}};
};
}
