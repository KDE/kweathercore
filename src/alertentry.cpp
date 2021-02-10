/*
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "alertentry.h"
#include "alertinfo.h"

namespace KWeatherCore
{
class AlertEntry::AlertEntryPrivate
{
public:
    QString identifier;
    QString sender;
    QDateTime sentTime;
    Status status;
    MsgType msgType;
    Scope scope;
    QString note;
    std::vector<AlertInfo> infoVec;
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

const QString &AlertEntry::identifier() const
{
    return d->identifier;
}
const QString &AlertEntry::sender() const
{
    return d->sender;
}
const QDateTime &AlertEntry::sentTime() const
{
    return d->sentTime;
}
AlertEntry::Status AlertEntry::status() const
{
    return d->status;
}
AlertEntry::MsgType AlertEntry::msgType() const
{
    return d->msgType;
}
AlertEntry::Scope AlertEntry::scope() const
{
    return d->scope;
}
const QString &AlertEntry::note() const
{
    return d->note;
}
const std::vector<AlertInfo> &AlertEntry::infoVec() const
{
    return d->infoVec;
}
void AlertEntry::setIdentifier(const QString &identifier)
{
    d->identifier = identifier;
}
void AlertEntry::setSender(const QString &sender)
{
    d->sender = sender;
}
void AlertEntry::setSentTime(const QDateTime &dateTime)
{
    d->sentTime = dateTime;
}
void AlertEntry::setStatus(const Status &status)
{
    d->status = status;
}
void AlertEntry::setMsgType(const MsgType &msgType)
{
    d->msgType = msgType;
}
void AlertEntry::setScope(const Scope &scope)
{
    d->scope = scope;
}
void AlertEntry::setNote(const QString &note)
{
    d->note = note;
}
void AlertEntry::setInfoVec(const std::vector<AlertInfo> &infoVec)
{
    d->infoVec = infoVec;
}
void AlertEntry::setInfoVec(std::vector<AlertInfo> &&infoVec)
{
    d->infoVec = std::move(infoVec);
}
void AlertEntry::addInfo(const AlertInfo &alertInfo)
{
    d->infoVec.push_back(alertInfo);
}
}
