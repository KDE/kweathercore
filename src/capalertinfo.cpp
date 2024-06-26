/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "capalertinfo.h"
#include "caparea.h"

namespace KWeatherCore
{
class CAPAlertInfoPrivate : public QSharedData
{
public:
    QString headline;
    QString description;
    QString event;
    QString sender;
    QString language = QStringLiteral("en-US"); // default value according to CAP specification
    QDateTime effectiveTime;
    QDateTime onsetTime;
    QDateTime expireTime;
    CAPAlertInfo::Categories categories = CAPAlertInfo::Category::Unknown;
    QString instruction;
    CAPAlertInfo::Urgency urgency = CAPAlertInfo::Urgency::UnknownUrgency;
    CAPAlertInfo::Severity severity = CAPAlertInfo::Severity::UnknownSeverity;
    CAPAlertInfo::Certainty certainty = CAPAlertInfo::Certainty::UnknownCertainty;
    CAPAlertInfo::ResponseTypes responseTypes = CAPAlertInfo::ResponseType::UnknownResponseType;
    QString contact;
    QString web;
    std::vector<CAPNamedValue> parameters;
    std::vector<CAPArea> areas;
    std::vector<CAPNamedValue> eventCodes;
};

CAPAlertInfo::CAPAlertInfo()
    : d(new CAPAlertInfoPrivate)
{
}
CAPAlertInfo::CAPAlertInfo(const CAPAlertInfo &other) = default;
CAPAlertInfo::CAPAlertInfo(CAPAlertInfo &&other) noexcept = default;
CAPAlertInfo::~CAPAlertInfo() = default;
CAPAlertInfo &CAPAlertInfo::operator=(const CAPAlertInfo &other) = default;
CAPAlertInfo &CAPAlertInfo::operator=(CAPAlertInfo &&other) noexcept = default;

QString CAPAlertInfo::event() const
{
    return d->event;
}
QDateTime CAPAlertInfo::effectiveTime() const
{
    return d->effectiveTime;
}
QDateTime CAPAlertInfo::onsetTime() const
{
    return d->onsetTime;
}
QDateTime CAPAlertInfo::expireTime() const
{
    return d->expireTime;
}
CAPAlertInfo::Categories CAPAlertInfo::categories() const
{
    return d->categories;
}
QString CAPAlertInfo::headline() const
{
    return d->headline;
}
QString CAPAlertInfo::description() const
{
    return d->description;
}
QString CAPAlertInfo::instruction() const
{
    return d->instruction;
}
QString CAPAlertInfo::sender() const
{
    return d->sender;
}
QString CAPAlertInfo::language() const
{
    return d->language;
}
CAPAlertInfo::Urgency CAPAlertInfo::urgency() const
{
    return d->urgency;
}
CAPAlertInfo::Severity CAPAlertInfo::severity() const
{
    return d->severity;
}
CAPAlertInfo::Certainty CAPAlertInfo::certainty() const
{
    return d->certainty;
}
CAPAlertInfo::ResponseTypes CAPAlertInfo::responseTypes() const
{
    return d->responseTypes;
}
QString CAPAlertInfo::contact() const
{
    return d->contact;
}
QString CAPAlertInfo::web() const
{
    return d->web;
}
const std::vector<CAPNamedValue> &CAPAlertInfo::parameters() const
{
    return d->parameters;
}
const std::vector<CAPArea> &CAPAlertInfo::areas() const
{
    return d->areas;
}
const std::vector<CAPNamedValue> &CAPAlertInfo::eventCodes() const
{
    return d->eventCodes;
}

void CAPAlertInfo::setHeadline(const QString &headline)
{
    d->headline = headline;
}
void CAPAlertInfo::setDescription(const QString &description)
{
    d->description = description;
}
void CAPAlertInfo::setInstruction(const QString &instruction)
{
    d->instruction = instruction;
}
void CAPAlertInfo::setSender(const QString &sender)
{
    d->sender = sender;
}
void CAPAlertInfo::setLanguage(const QString &language)
{
    d->language = language;
}
void CAPAlertInfo::addCategory(Category category)
{
    d->categories |= category;
}
void CAPAlertInfo::setEvent(const QString &event)
{
    d->event = event;
}

void CAPAlertInfo::setEffectiveTime(const QDateTime &time)
{
    d->effectiveTime = time;
}
void CAPAlertInfo::setOnsetTime(const QDateTime &time)
{
    d->onsetTime = time;
}
void CAPAlertInfo::setExpireTime(const QDateTime &time)
{
    d->expireTime = time;
}
void CAPAlertInfo::setUrgency(Urgency urgency)
{
    d->urgency = urgency;
}
void CAPAlertInfo::setSeverity(Severity severity)
{
    d->severity = severity;
}
void CAPAlertInfo::setCertainty(Certainty certainty)
{
    d->certainty = certainty;
}
void CAPAlertInfo::addResponseType(ResponseType responseType)
{
    d->responseTypes |= responseType;
}
void CAPAlertInfo::setContact(const QString &contact)
{
    d->contact = contact;
}
void CAPAlertInfo::setWeb(const QString &web)
{
    d->web = web;
}
void CAPAlertInfo::addParameter(CAPNamedValue &&param)
{
    d->parameters.push_back(std::move(param));
}

void CAPAlertInfo::addArea(CAPArea &&area)
{
    d->areas.push_back(std::move(area));
}

void CAPAlertInfo::addEventCode(CAPNamedValue &&code)
{
    d->eventCodes.push_back(std::move(code));
}
}

#include "moc_capalertinfo.cpp"
