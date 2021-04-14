/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "alertinfo.h"
namespace KWeatherCore
{
class AlertInfo::AlertInfoPrivate
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
    Category category = Category::Unknown;
    QString instruction;
    Urgency urgency = Urgency::Unknown;
    Severity severity = Severity::Unknown;
    Certainty certainty = Certainty::Unknown;
    Parameter parameter;
    QString areaDesc;
    AreaCodeVec areaCodes;
    std::vector<std::vector<std::pair<float, float>>> polygon;
};
AlertInfo::AlertInfo()
    : d(std::make_unique<AlertInfoPrivate>())
{
}
AlertInfo::AlertInfo(const AlertInfo &other)
    : d(std::make_unique<AlertInfoPrivate>(*other.d))
{
}
AlertInfo::AlertInfo(AlertInfo &&other) = default;
AlertInfo::~AlertInfo() = default;
AlertInfo &AlertInfo::operator=(AlertInfo &&other) = default;

const QString &AlertInfo::event() const
{
    return d->event;
}
const AreaCodeVec &AlertInfo::areaCodes() const
{
    return d->areaCodes;
}
const QDateTime &AlertInfo::effectiveTime() const
{
    return d->effectiveTime;
}
const QDateTime &AlertInfo::onsetTime() const
{
    return d->onsetTime;
}
const QDateTime &AlertInfo::expireTime() const
{
    return d->expireTime;
}
AlertInfo::Category AlertInfo::category() const
{
    return d->category;
}
const QString &AlertInfo::headline() const
{
    return d->headline;
}
const QString &AlertInfo::description() const
{
    return d->description;
}
const QString &AlertInfo::instruction() const
{
    return d->instruction;
}
const QString &AlertInfo::sender() const
{
    return d->sender;
}
const QString &AlertInfo::language() const
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
const Parameter &AlertInfo::parameter() const
{
    return d->parameter;
}
const QString &AlertInfo::areaDesc() const
{
    return d->areaDesc;
}
const std::vector<std::vector<std::pair<float, float>>> &AlertInfo::polygon() const
{
    return d->polygon;
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
void AlertInfo::setCategory(Category category)
{
    d->category = category;
}
void AlertInfo::addCategory(Category category)
{
    d->category |= category;
}
void AlertInfo::setEvent(const QString &event)
{
    d->event = event;
}
void AlertInfo::setAreaCodes(const AreaCodeVec &areaCodes)
{
    d->areaCodes = areaCodes;
}
void AlertInfo::addAreaCode(std::pair<QString, QString> &areaCode)
{
    d->areaCodes.push_back(areaCode);
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
void AlertInfo::setParameter(const Parameter &parameter)
{
    d->parameter = parameter;
}
void AlertInfo::addParameter(std::pair<QString, QString> &p)
{
    d->parameter.push_back(p);
}

void AlertInfo::setAreaDesc(const QString &areaDesc)
{
    d->areaDesc = areaDesc;
}
void AlertInfo::setPolygon(const std::vector<std::vector<std::pair<float, float>>> &polygon)
{
    d->polygon = polygon;
}
void AlertInfo::setPolygon(std::vector<std::vector<std::pair<float, float>>> &&polygon)
{
    d->polygon = std::move(polygon);
}
void AlertInfo::addPolygon(const std::vector<std::pair<float, float>> &area)
{
    d->polygon.push_back(area);
}
void AlertInfo::addPolygon(std::vector<std::pair<float, float>> &&area)
{
    d->polygon.emplace_back(area);
}
AlertInfo &AlertInfo::operator=(const AlertInfo &other)
{
    *d = *other.d;
    return *this;
}
}
