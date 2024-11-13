/*
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "capparser.h"
#include "capalertinfo.h"
#include "capalertmessage.h"
#include "caparea.h"
#include "capnamedvalue.h"
#include "capreference.h"

#include <KLocalizedString>

#include <QDateTime>
#include <QDebug>
#include <QStringTokenizer>

#include <optional>

using namespace Qt::Literals;

namespace KWeatherCore
{

template<typename T>
struct MapEntry {
    const char *name;
    T value;
};

template<typename QStringT, typename EnumT, std::size_t N>
static std::optional<EnumT> stringToValue(const QStringT &s, const MapEntry<EnumT> (&map)[N])
{
    const auto it = std::lower_bound(std::begin(map), std::end(map), s, [](auto lhs, auto rhs) {
        return QLatin1String(lhs.name) < rhs;
    });
    if (it != std::end(map) && QLatin1String((*it).name) == s) {
        return (*it).value;
    }
    return {};
}

// ### important: keep all the following tables sorted by name!
static constexpr const MapEntry<CAPAlertInfo::Category> category_map[] = {
    {"CBRNE", CAPAlertInfo::Category::CBRNE},
    {"Env", CAPAlertInfo::Category::Environmental},
    {"Fire", CAPAlertInfo::Category::Fire},
    {"Geo", CAPAlertInfo::Category::Geophysical},
    {"Health", CAPAlertInfo::Category::Health},
    {"Infra", CAPAlertInfo::Category::Infrastructure},
    {"Met", CAPAlertInfo::Category::Meteorological},
    {"Other", CAPAlertInfo::Category::Other},
    {"Rescue", CAPAlertInfo::Category::Rescue},
    {"Safety", CAPAlertInfo::Category::Safety},
    {"Security", CAPAlertInfo::Category::Security},
    {"Transport", CAPAlertInfo::Category::Transport},
};

enum class Tags { ALERT, IDENTIFIER, SENDER, SENT_TIME, STATUS, MSG_TYPE, SCOPE, NOTE, INFO, REFERENCES };

static constexpr const MapEntry<Tags> tag_map[] = {
    {"alert", Tags::ALERT},
    {"identifier", Tags::IDENTIFIER},
    {"info", Tags::INFO},
    {"msgType", Tags::MSG_TYPE},
    {"note", Tags::NOTE},
    {"references", Tags::REFERENCES},
    {"scope", Tags::SCOPE},
    {"sender", Tags::SENDER},
    {"sent", Tags::SENT_TIME},
    {"status", Tags::STATUS},
};

enum class InfoTags {
    HEADLINE,
    DESCRIPTION,
    EVENT,
    EVENTCODE,
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
    LANGUAGE,
    RESPONSETYPE,
    CONTACT,
    WEB,
};

static constexpr const MapEntry<InfoTags> info_tag_map[] = {
    {"area", InfoTags::AREA},
    {"category", InfoTags::CATEGORY},
    {"certainty", InfoTags::CERTAINITY},
    {"contact", InfoTags::CONTACT},
    {"description", InfoTags::DESCRIPTION},
    {"effective", InfoTags::EFFECTIVE_TIME},
    {"event", InfoTags::EVENT},
    {"eventCode", InfoTags::EVENTCODE},
    {"expires", InfoTags::EXPIRE_TIME},
    {"headline", InfoTags::HEADLINE},
    {"instruction", InfoTags::INSTRUCTION},
    {"language", InfoTags::LANGUAGE},
    {"onset", InfoTags::ONSET_TIME},
    {"parameter", InfoTags::PARAMETER},
    {"responseType", InfoTags::RESPONSETYPE},
    {"senderName", InfoTags::SENDERNAME},
    {"severity", InfoTags::SEVERITY},
    {"urgency", InfoTags::URGENCY},
    {"web", InfoTags::WEB},
};

static constexpr const MapEntry<CAPAlertMessage::Status> status_map[] = {
    {"Actual", CAPAlertMessage::Status::Actual},
    {"Draft", CAPAlertMessage::Status::Draft},
    {"Excercise", CAPAlertMessage::Status::Exercise},
    {"System", CAPAlertMessage::Status::System},
    {"Test", CAPAlertMessage::Status::Test},
};

static constexpr const MapEntry<CAPAlertMessage::MessageType> msgtype_map[] = {
    {"Ack", CAPAlertMessage::MessageType::Acknowledge},
    {"Alert", CAPAlertMessage::MessageType::Alert},
    {"Cancel", CAPAlertMessage::MessageType::Cancel},
    {"Error", CAPAlertMessage::MessageType::Error},
    {"Update", CAPAlertMessage::MessageType::Update},
};

static constexpr const MapEntry<CAPAlertMessage::Scope> scope_map[] = {
    {"Private", CAPAlertMessage::Scope::Private},
    {"Public", CAPAlertMessage::Scope::Public},
    {"Restricted", CAPAlertMessage::Scope::Restricted},
};

static constexpr const MapEntry<CAPAlertInfo::ResponseType> response_type_map[] = {
    {"AllClear", CAPAlertInfo::ResponseType::AllClear},
    {"Assess", CAPAlertInfo::ResponseType::Assess},
    {"Avoid", CAPAlertInfo::ResponseType::Avoid},
    {"Evacuate", CAPAlertInfo::ResponseType::Evacuate},
    {"Execute", CAPAlertInfo::ResponseType::Execute},
    {"Monitor", CAPAlertInfo::ResponseType::Monitor},
    {"None", CAPAlertInfo::ResponseType::None},
    {"Prepare", CAPAlertInfo::ResponseType::Prepare},
    {"Shelter", CAPAlertInfo::ResponseType::Shelter},
};

static constexpr const MapEntry<CAPAlertInfo::Urgency> urgency_map[] = {
    {"Expected", CAPAlertInfo::Urgency::Expected},
    {"Future", CAPAlertInfo::Urgency::Future},
    {"Immediate", CAPAlertInfo::Urgency::Immediate},
    {"Past", CAPAlertInfo::Urgency::Past},
};

static constexpr const MapEntry<CAPAlertInfo::Severity> severity_map[] = {
    {"Extreme", CAPAlertInfo::Severity::Extreme},
    {"Minor", CAPAlertInfo::Severity::Minor},
    {"Moderate", CAPAlertInfo::Severity::Moderate},
    {"Severe", CAPAlertInfo::Severity::Severe},
};

static constexpr const MapEntry<CAPAlertInfo::Certainty> certainty_map[] = {
    {"Likely", CAPAlertInfo::Certainty::Likely},
    {"Observed", CAPAlertInfo::Certainty::Observed},
    {"Possible", CAPAlertInfo::Certainty::Possible},
    {"Unlikely", CAPAlertInfo::Certainty::Unlikely},
};

[[nodiscard]] static CAPPolygon stringToPolygon(QStringView str)
{
    CAPPolygon res;

    for (auto coordinate : QStringTokenizer(str, ' '_L1, Qt::SkipEmptyParts)) {
        const auto idx = coordinate.indexOf(','_L1);
        if (idx < 0) {
            continue;
        }
        bool latOk = false, lonOk = false;
        res.push_back({coordinate.left(idx).toFloat(&latOk), coordinate.mid(idx + 1).toFloat(&lonOk)});
        if (!latOk || !lonOk) {
            res.pop_back();
        }
    }
    return res;
}

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

CAPAlertMessage CAPParser::parse()
{
    CAPAlertMessage entry;
    while (m_xml.readNextStartElement()) {
        const auto tag = stringToValue(m_xml.name(), tag_map);
        if (!tag) {
            m_xml.skipCurrentElement();
            continue;
        }
        switch (*tag) {
        case Tags::IDENTIFIER:
            entry.setIdentifier(m_xml.readElementText());
            break;
        case Tags::SENDER:
            entry.setSender(m_xml.readElementText());
            break;
        case Tags::SENT_TIME:
            entry.setSentTime(QDateTime::fromString(m_xml.readElementText(), Qt::ISODate));
            break;
        case Tags::STATUS: {
            const auto elementText = m_xml.readElementText();
            const auto status = stringToValue(elementText, status_map);
            if (status) {
                entry.setStatus(*status);
            } else {
                qWarning() << "Unknown status field" << elementText;
            }
            break;
        }
        case Tags::MSG_TYPE: {
            const auto elementText = m_xml.readElementText();
            const auto msgType = stringToValue(elementText, msgtype_map);
            if (msgType) {
                entry.setMessageType(*msgType);
            } else {
                qWarning() << "Unknown msgType field" << elementText;
            }
            break;
        }
        case Tags::SCOPE: {
            const auto elementText = m_xml.readElementText();
            const auto scope = stringToValue(elementText, scope_map);
            if (scope) {
                entry.setScope(*scope);
            } else {
                qWarning() << "Unknown scope field" << elementText;
            }
            break;
        }
        case Tags::NOTE:
            entry.setNote(m_xml.readElementText());
            break;
        case Tags::INFO: {
            auto info = parseInfo();
            entry.addInfo(std::move(info));
            break;
        }
        case Tags::REFERENCES:
            entry.setReferences(parseReferences(m_xml.readElementText()));
            break;
        default:
            m_xml.skipCurrentElement();
        }
    }
    return entry;
}

CAPAlertInfo CAPParser::parseInfo()
{
    CAPAlertInfo info;

    if (m_xml.name() == QLatin1String("info")) {
        while (!m_xml.atEnd() && !(m_xml.isEndElement() && m_xml.name() == QLatin1String("info"))) {
            m_xml.readNext();
            if (!m_xml.isStartElement()) {
                continue;
            }
            const auto tag = stringToValue(m_xml.name(), info_tag_map);
            if (tag) {
                switch (*tag) {
                case InfoTags::CATEGORY: {
                    const auto s = m_xml.readElementText();
                    const auto category = stringToValue(s, category_map);
                    if (category) {
                        info.addCategory(*category);
                    }
                    break;
                }
                case InfoTags::EVENT:
                    info.setEvent(m_xml.readElementText());
                    break;
                case InfoTags::URGENCY: {
                    const auto s = m_xml.readElementText();
                    if (const auto urgency = stringToValue(s, urgency_map); urgency) {
                        info.setUrgency(*urgency);
                    } else {
                        qWarning() << "Unknown urgency type:" << s;
                    }
                    break;
                }
                case InfoTags::SEVERITY: {
                    const auto s = m_xml.readElementText();
                    if (const auto severity = stringToValue(s, severity_map); severity) {
                        info.setSeverity(*severity);
                    } else {
                        qWarning() << "Unknown severity type:" << s;
                    }
                    break;
                }
                case InfoTags::CERTAINITY: {
                    const auto s = m_xml.readElementText();
                    if (const auto certainty = stringToValue(s, certainty_map); certainty) {
                        info.setCertainty(*certainty);
                    } else {
                        qWarning() << "Unknown certainty type:" << s;
                    }
                    break;
                }
                case InfoTags::EFFECTIVE_TIME:
                    info.setEffectiveTime(QDateTime::fromString(m_xml.readElementText(), Qt::ISODate));
                    break;
                case InfoTags::ONSET_TIME:
                    info.setOnsetTime(QDateTime::fromString(m_xml.readElementText(), Qt::ISODate));
                    break;
                case InfoTags::EXPIRE_TIME:
                    info.setExpireTime(QDateTime::fromString(m_xml.readElementText(), Qt::ISODate));
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
                    info.addParameter(parseNamedValue());
                    break;
                }
                case InfoTags::AREA: {
                    info.addArea(parseArea());
                    break;
                }
                case InfoTags::SENDERNAME: {
                    info.setSender(m_xml.readElementText());
                    break;
                }
                case InfoTags::LANGUAGE:
                    info.setLanguage(m_xml.readElementText());
                    break;
                case InfoTags::RESPONSETYPE: {
                    const auto elementText = m_xml.readElementText();
                    if (const auto respType = stringToValue(elementText, response_type_map)) {
                        info.addResponseType(*respType);
                    } else {
                        qWarning() << "Unknown respone type value" << elementText;
                    }
                    break;
                }
                case InfoTags::CONTACT:
                    info.setContact(m_xml.readElementText());
                    break;
                case InfoTags::WEB:
                    info.setWeb(m_xml.readElementText());
                    break;
                case InfoTags::EVENTCODE:
                    info.addEventCode(parseNamedValue());
                    break;
                }
            } else {
                if (m_xml.isStartElement()) {
                    qWarning() << "unknown element: " << m_xml.name();
                }
            }
        }
    }
    return info;
}

CAPArea CAPParser::parseArea()
{
    CAPArea area;
    while (!(m_xml.isEndElement() && m_xml.name() == QLatin1String("area"))) {
        m_xml.readNext();
        if (m_xml.name() == QLatin1String("areaDesc") && !m_xml.isEndElement()) {
            area.setDescription(m_xml.readElementText());
        } else if (m_xml.name() == QLatin1String("geocode") && !m_xml.isEndElement()) {
            area.addGeoCode(parseNamedValue());
        } else if (m_xml.name() == QLatin1String("polygon") && !m_xml.isEndElement()) {
            area.addPolygon(stringToPolygon(m_xml.readElementText()));
        } else if (m_xml.name() == QLatin1String("circle") && !m_xml.isEndElement()) {
            const auto t = m_xml.readElementText();
            const auto commaIdx = t.indexOf(QLatin1Char(','));
            const auto spaceIdx = t.indexOf(QLatin1Char(' '));
            if (commaIdx > 0 && spaceIdx > commaIdx && commaIdx < t.size()) {
                CAPCircle circle;
                circle.latitude = QStringView(t).left(commaIdx).toFloat();
                circle.longitude = QStringView(t).mid(commaIdx + 1, spaceIdx - commaIdx - 1).toFloat();
                circle.radius = QStringView(t).mid(spaceIdx).toFloat();
                area.addCircle(std::move(circle));
            }
        } else if (m_xml.name() == QLatin1String("altitude") && !m_xml.isEndElement()) {
            area.setAltitude(m_xml.readElementText().toFloat());
        } else if (m_xml.name() == QLatin1String("ceiling") && !m_xml.isEndElement()) {
            area.setCeiling(m_xml.readElementText().toFloat());
        } else if (m_xml.isStartElement()) {
            qDebug() << "unknown area element:" << m_xml.name();
        }
    }
    return area;
}

CAPNamedValue CAPParser::parseNamedValue()
{
    CAPNamedValue value;
    const auto elementName = m_xml.name().toString();
    while (!m_xml.isEndElement() || m_xml.name() != elementName) {
        m_xml.readNext();
        if (m_xml.isStartElement() && m_xml.name() == QLatin1String("valueName")) {
            value.name = m_xml.readElementText();
        } else if (m_xml.isStartElement() && m_xml.name() == QLatin1String("value")) {
            value.value = m_xml.readElementText();
        } else if (m_xml.isStartElement()) {
            qDebug() << "unknown named value element:" << m_xml.name();
        }
    }
    return value;
}

std::vector<CAPReference> CAPParser::parseReferences(const QString &refsString)
{
    std::vector<CAPReference> refs;
    // TODO for Qt 6: use QStringTokenizer
    const auto refsSplit = refsString.split(QLatin1Char(' '), Qt::SkipEmptyParts);
    refs.reserve(refsSplit.size());
    for (const auto &refString : refsSplit) {
        const auto refSplit = refString.split(QLatin1Char(','));
        if (refSplit.size() != 3) {
            qDebug() << "failed to parse CAP reference:" << refString;
            continue;
        }
        refs.emplace_back(refSplit.at(0), refSplit.at(1), QDateTime::fromString(refSplit.at(2), Qt::ISODate));
    }

    return refs;
}
}
