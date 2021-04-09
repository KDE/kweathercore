/*
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "capparser.h"
#include <QDateTime>
#include <QDebug>
#include <KLocalizedString>

namespace KWeatherCore
{
CAPParser::CAPParser(const QByteArray &data)
    : m_xml(data)
{
    bool flag = false;
    if (!data.isEmpty()) {
        while (m_xml.readNextStartElement()) {
            if (m_xml.name() == QStringLiteral("alert")) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            qWarning() << "Not a CAP XML";
        }
    }
}

std::unique_ptr<AlertEntry> CAPParser::parse()
{
    auto entry = std::make_unique<AlertEntry>();
    while (m_xml.readNextStartElement()) {
        switch (tags[m_xml.name().toString()]) {
        case Tags::IDENTIFIER:
            entry->setIdentifier(m_xml.readElementText());
            break;
        case Tags::SENDER:
            entry->setSender(m_xml.readElementText());
            break;
        case Tags::SENT_TIME:
            entry->setSentTime(
                QDateTime::fromString(m_xml.readElementText(), Qt::ISODate));
            break;
        case Tags::STATUS: {
            AlertEntry::Status status;
            auto elementText = m_xml.readElementText();
            if (elementText == QStringLiteral("Actual"))
                status = AlertEntry::Status::Actual;
            else if (elementText == QStringLiteral("Excercise"))
                status = AlertEntry::Status::Exercise;
            else if (elementText == QStringLiteral("System"))
                status = AlertEntry::Status::System;
            else if (elementText == QStringLiteral("Test"))
                status = AlertEntry::Status::Test;
            else if (elementText == QStringLiteral("Draft"))
                status = AlertEntry::Status::Draft;
            else
                qWarning() << "Unknown status field";
            entry->setStatus(status);
            break;
        }
        case Tags::MSG_TYPE: {
            AlertEntry::MsgType msgType;
            auto elementText = m_xml.readElementText();
            if (elementText == QStringLiteral("Alert"))
                msgType = AlertEntry::MsgType::Alert;
            else if (elementText == QStringLiteral("Update"))
                msgType = AlertEntry::MsgType::Update;
            else if (elementText == QStringLiteral("Cancel"))
                msgType = AlertEntry::MsgType::Cancel;
            else if (elementText == QStringLiteral("Ack"))
                msgType = AlertEntry::MsgType::Ack;
            else if (elementText == QStringLiteral("Error"))
                msgType = AlertEntry::MsgType::Error;
            else
                qWarning() << "Unknown msgType field";
            entry->setMsgType(msgType);
            break;
        }
        case Tags::SCOPE: {
            AlertEntry::Scope scope;
            auto elementText = m_xml.readElementText();
            if (elementText == QStringLiteral("Public"))
                scope = AlertEntry::Scope::Public;
            else if (elementText == QStringLiteral("Private"))
                scope = AlertEntry::Scope::Private;
            else if (elementText == QStringLiteral("Restricted"))
                scope = AlertEntry::Scope::Restricted;
            else
                qWarning() << "Unknown scope field";

            entry->setScope(scope);
            break;
        }
        case Tags::NOTE:
            entry->setNote(m_xml.readElementText());
            break;
        case Tags::INFO: {
            auto info = parseInfo();
            entry->addInfo(info);
            break;
        }
        default:
            m_xml.skipCurrentElement();
        }
    }
    return entry;
}

AlertInfo CAPParser::parseInfo()
{
    AlertInfo info;

    if (m_xml.name() == QStringLiteral("info")) {
        while (
            !(m_xml.isEndElement() && m_xml.name() == QStringLiteral("info"))) {
            if (m_xml.isStartElement()) {
                switch (infoTags[m_xml.name().toString()]) {
                case InfoTags::CATEGORY: {
                    auto s = m_xml.readElementText();
                    // Supporting only Meteorological sources for now.
                    if (s == QStringLiteral("Met"))
                        info.setCategory(AlertInfo::Category::Met);
                    break;
                }
                case InfoTags::EVENT:
                    info.setEvent(m_xml.readElementText());
                    break;
                case InfoTags::URGENCY:
                    info.setUrgency(getUrgency(m_xml.readElementText()));
                    break;
                case InfoTags::SEVERITY:
                    info.setSeverity(getSeverity(m_xml.readElementText()));
                    break;
                case InfoTags::CERTAINITY:
                    info.setCertainty(getCertainity(m_xml.readElementText()));
                    break;
                case InfoTags::EFFECTIVE_TIME:
                    info.setEffectiveTime(QDateTime::fromString(
                        m_xml.readElementText(), Qt::ISODate));
                    break;
                case InfoTags::EXPIRE_TIME:
                    info.setExpireTime(QDateTime::fromString(
                        m_xml.readElementText(), Qt::ISODate));
                    break;
                case InfoTags::HEADLINE:
                    info.setHeadline(m_xml.readElementText());
                    break;
                case InfoTags::DESCRIPTION:
                    info.setDescription(m_xml.readElementText());
                    break;
                case InfoTags::INSTRUCTION:
                    info.setInstruction(m_xml.readElementText());
                    break;
                case InfoTags::PARAMETER: {
                    std::pair<QString, QString> p;
                    m_xml.readNextStartElement();
                    if (m_xml.name() == QStringLiteral("valueName"))
                        p.first = m_xml.readElementText();
                    m_xml.readNextStartElement();
                    if (m_xml.name() == QStringLiteral("value"))
                        p.second = m_xml.readElementText();
                    info.addParameter(p);
                    break;
                }
                case InfoTags::AREA: {
                    while (!(m_xml.isEndElement() &&
                             m_xml.name() == QStringLiteral("area"))) {
                        if (m_xml.name() == QStringLiteral("areaDesc") &&
                            !m_xml.isEndElement())
                            info.setAreaDesc(m_xml.readElementText());
                        else if (m_xml.name() == QStringLiteral("geocode") &&
                                 !m_xml.isEndElement()) {
                            std::pair<QString, QString> p;
                            m_xml.readNextStartElement();
                            if (m_xml.name() == QStringLiteral("valueName"))
                                p.first = m_xml.readElementText();
                            m_xml.readNextStartElement();
                            if (m_xml.name() == QStringLiteral("value"))
                                p.second = m_xml.readElementText();
                            info.addAreaCode(p);
                        }
                        m_xml.readNext();
                    }
                    break;
                }
                }
            }
            m_xml.readNext();
        }
    }
    return info;
}

AlertInfo::Urgency getUrgency(const QString &s)
{
    if (s == QStringLiteral("Expected"))
        return AlertInfo::Urgency::Expected;
    else if (s == QStringLiteral("Immediate"))
        return AlertInfo::Urgency::Immediate;
    else if (s == QStringLiteral("Future"))
        return AlertInfo::Urgency::Future;
    else if (s == QStringLiteral("Past"))
        return AlertInfo::Urgency::Past;
    else
        return AlertInfo::Urgency::Unknown;
}

AlertInfo::Severity getSeverity(const QString &s)
{
    if (s == QStringLiteral("Extreme"))
        return AlertInfo::Severity::Extreme;
    else if (s == QStringLiteral("Severe"))
        return AlertInfo::Severity::Severe;
    else if (s == QStringLiteral("Moderate"))
        return AlertInfo::Severity::Moderate;
    else if (s == QStringLiteral("Minor"))
        return AlertInfo::Severity::Minor;
    else
        return AlertInfo::Severity::Unknown;
}

AlertInfo::Certainty getCertainity(const QString &s)
{
    if (s == QStringLiteral("Observed"))
        return AlertInfo::Certainty::Observed;
    else if (s == QStringLiteral("Likely"))
        return AlertInfo::Certainty::Likely;
    else if (s == QStringLiteral("Possible"))
        return AlertInfo::Certainty::Possible;
    else if (s == QStringLiteral("Unlikely"))
        return AlertInfo::Certainty::Unlikely;
    else
        return AlertInfo::Certainty::Unknown;
}

}
