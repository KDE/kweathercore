/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "alertentry.h"
namespace KWeatherCore
{
AlertEntry::AlertEntry()
    : d(new AlertEntryPrivate())
{
}
AlertEntry::AlertEntry(const AlertEntry &other)
    : d(new AlertEntryPrivate())
{
    *d = *other.d;
}
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
const QDateTime &AlertEntry::effectiveTime() const
{
    return d->effectiveTime;
}
const QDateTime &AlertEntry::expireTime() const
{
    return d->expireTime;
}
MsgType AlertEntry::msgType() const
{
    return d->msgType;
}
Urgency AlertEntry::urgency() const
{
    return d->urgency;
}
const std::vector<float> &AlertEntry::area() const
{
    return d->area;
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
void AlertEntry::setArea(const std::vector<float> &area)
{
    d->area = area;
}
void AlertEntry::setArea(std::vector<float> &&area)
{
    d->area = std::move(area);
}
AlertEntry &AlertEntry::operator=(const AlertEntry &other)
{
    *d = *other.d;
    return *this;
}
}
