/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "alertentry.h"
namespace KWeatherCore
{
class AlertEntry::AlertEntryPrivate
{
public:
    QString title;
    QString summary;
    QString event;
    QStringList areaNames;
    QDateTime effectiveTime;
    QDateTime expireTime;
    MsgType msgType;
    Urgency urgency;
    Severity severity;
    Certainty certainty;
    AreaCodeVec areaCodes;
    std::vector<std::pair<float, float>> polygon;
};
AlertEntry::AlertEntry()
    : d(new AlertEntryPrivate())
{
}
AlertEntry::AlertEntry(const AlertEntry &other)
    : d(new AlertEntryPrivate())
{
    *d = *other.d;
}
AlertEntry::AlertEntry(AlertEntry &&other) = default;
AlertEntry::~AlertEntry() = default;
AlertEntry &AlertEntry::operator=(AlertEntry &&other) = default;
const QString &AlertEntry::title() const
{
    return d->title;
}
const QString &AlertEntry::summary() const
{
    return d->summary;
}
const QString &AlertEntry::event() const
{
    return d->event;
}
const QStringList &AlertEntry::areaNames() const
{
    return d->areaNames;
}
const AreaCodeVec &AlertEntry::areaCodes() const
{
    return d->areaCodes;
}
const QDateTime &AlertEntry::effectiveTime() const
{
    return d->effectiveTime;
}
const QDateTime &AlertEntry::expireTime() const
{
    return d->expireTime;
}
AlertEntry::MsgType AlertEntry::msgType() const
{
    return d->msgType;
}
AlertEntry::Urgency AlertEntry::urgency() const
{
    return d->urgency;
}
AlertEntry::Severity AlertEntry::severity() const
{
    return d->severity;
}
AlertEntry::Certainty AlertEntry::certainty() const
{
    return d->certainty;
}
const std::vector<std::pair<float, float>> &AlertEntry::polygon() const
{
    return d->polygon;
}
void AlertEntry::setTitle(const QString &title)
{
    d->title = title;
}
void AlertEntry::setSummary(const QString &summary)
{
    d->summary = summary;
}
void AlertEntry::setEvent(const QString &event)
{
    d->event = event;
}
void AlertEntry::setAreaNames(const QStringList &areas)
{
    d->areaNames = areas;
}
void AlertEntry::setAreaCodes(const AreaCodeVec &areaCodes)
{
    d->areaCodes = areaCodes;
}
void AlertEntry::setEffectiveTime(const QDateTime &time)
{
    d->effectiveTime = time;
}
void AlertEntry::setExpireTime(const QDateTime &time)
{
    d->expireTime = time;
}
void AlertEntry::setMsgType(MsgType type)
{
    d->msgType = type;
}
void AlertEntry::setUrgency(Urgency urgency)
{
    d->urgency = urgency;
}
void AlertEntry::setSeverity(Severity severity)
{
    d->severity = severity;
}
void AlertEntry::setCertainty(Certainty certainty)
{
    d->certainty = certainty;
}
void AlertEntry::setPolygon(const std::vector<std::pair<float, float>> &polygon)
{
    d->polygon = polygon;
}
void AlertEntry::setPolygon(std::vector<std::pair<float, float>> &&polygon)
{
    d->polygon = std::move(polygon);
}
AlertEntry &AlertEntry::operator=(const AlertEntry &other)
{
    *d = *other.d;
    return *this;
}
}
