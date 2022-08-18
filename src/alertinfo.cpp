/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "alertinfo.h"
#include "caparea.h"
namespace KWeatherCore
{
class AlertInfo::AlertInfoPrivate : public QSharedData
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
    Categories categories = Category::Unknown;
    QString instruction;
    Urgency urgency = Urgency::UnknownUrgency;
    Severity severity = Severity::UnknownSeverity;
    Certainty certainty = Certainty::UnknownCertainty;
    ResponseTypes responseTypes = ResponseType::UnknownResponseType;
    QString contact;
    QString web;
    std::vector<CAPNamedValue> parameters;
    std::vector<CAPArea> areas;
    std::vector<CAPNamedValue> eventCodes;
};
AlertInfo::AlertInfo()
    : d(new AlertInfoPrivate)
{
}
AlertInfo::AlertInfo(const AlertInfo &other) = default;
AlertInfo::AlertInfo(AlertInfo &&other) = default;
AlertInfo::~AlertInfo() = default;
AlertInfo &AlertInfo::operator=(const AlertInfo &other) = default;
AlertInfo &AlertInfo::operator=(AlertInfo &&other) = default;

QString AlertInfo::event() const
{
    return d->event;
}
QDateTime AlertInfo::effectiveTime() const
{
    return d->effectiveTime;
}
QDateTime AlertInfo::onsetTime() const
{
    return d->onsetTime;
}
QDateTime AlertInfo::expireTime() const
{
    return d->expireTime;
}
AlertInfo::Categories AlertInfo::categories() const
{
    return d->categories;
}
QString AlertInfo::headline() const
{
    return d->headline;
}
QString AlertInfo::description() const
{
    return d->description;
}
QString AlertInfo::instruction() const
{
    return d->instruction;
}
QString AlertInfo::sender() const
{
    return d->sender;
}
QString AlertInfo::language() const
{
    return d->language;
}
AlertInfo::Urgency AlertInfo::urgency() const
{
    return d->urgency;
}
AlertInfo::Severity AlertInfo::severity() const
{
    return d->severity;
}
AlertInfo::Certainty AlertInfo::certainty() const
{
    return d->certainty;
}
AlertInfo::ResponseTypes AlertInfo::responseTypes() const
{
    return d->responseTypes;
}
QString AlertInfo::contact() const
{
    return d->contact;
}
QString AlertInfo::web() const
{
    return d->web;
}
const std::vector<CAPNamedValue> &AlertInfo::parameters() const
{
    return d->parameters;
}
const std::vector<CAPArea> &AlertInfo::areas() const
{
    return d->areas;
}
const std::vector<CAPNamedValue> &AlertInfo::eventCodes() const
{
    return d->eventCodes;
}

void AlertInfo::setHeadline(const QString &headline)
{
    d->headline = headline;
}
void AlertInfo::setDescription(const QString &description)
{
    d->description = description;
}
void AlertInfo::setInstruction(const QString &instruction)
{
    d->instruction = instruction;
}
void AlertInfo::setSender(const QString &sender)
{
    d->sender = sender;
}
void AlertInfo::setLanguage(const QString &language)
{
    d->language = language;
}
void AlertInfo::addCategory(Category category)
{
    d->categories |= category;
}
void AlertInfo::setEvent(const QString &event)
{
    d->event = event;
}

void AlertInfo::setEffectiveTime(const QDateTime &time)
{
    d->effectiveTime = time;
}
void AlertInfo::setOnsetTime(const QDateTime &time)
{
    d->onsetTime = time;
}
void AlertInfo::setExpireTime(const QDateTime &time)
{
    d->expireTime = time;
}
void AlertInfo::setUrgency(Urgency urgency)
{
    d->urgency = urgency;
}
void AlertInfo::setSeverity(Severity severity)
{
    d->severity = severity;
}
void AlertInfo::setCertainty(Certainty certainty)
{
    d->certainty = certainty;
}
void AlertInfo::addResponseType(ResponseType responseType)
{
    d->responseTypes |= responseType;
}
void AlertInfo::setContact(const QString &contact)
{
    d->contact = contact;
}
void AlertInfo::setWeb(const QString &web)
{
    d->web = web;
}
void AlertInfo::addParameter(CAPNamedValue &&param)
{
    d->parameters.push_back(std::move(param));
}

void AlertInfo::addArea(CAPArea &&area)
{
    d->areas.push_back(std::move(area));
}

void AlertInfo::addEventCode(CAPNamedValue &&code)
{
    d->eventCodes.push_back(std::move(code));
}
}
