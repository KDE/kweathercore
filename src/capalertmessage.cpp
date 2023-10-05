/*
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "capalertmessage.h"
#include "capalertinfo.h"
#include "capreference.h"

namespace KWeatherCore
{
class CAPAlertMessagePrivate : public QSharedData
{
public:
    QString identifier;
    QString sender;
    QDateTime sentTime;
    CAPAlertMessage::Status status = CAPAlertMessage::Status::UnknownStatus;
    CAPAlertMessage::MessageType msgType = CAPAlertMessage::MessageType::UnknownMessageType;
    CAPAlertMessage::Scope scope = CAPAlertMessage::Scope::UnknownScope;
    QString note;
    std::vector<CAPAlertInfo> infoVec;
    std::vector<CAPReference> references;
};

CAPAlertMessage::CAPAlertMessage()
    : d(new CAPAlertMessagePrivate)
{
}
CAPAlertMessage::CAPAlertMessage(const CAPAlertMessage &other) = default;
CAPAlertMessage::CAPAlertMessage(CAPAlertMessage &&other) = default;
CAPAlertMessage::~CAPAlertMessage() = default;
CAPAlertMessage &CAPAlertMessage::operator=(const CAPAlertMessage &other) = default;
CAPAlertMessage &CAPAlertMessage::operator=(CAPAlertMessage &&other) = default;

QString CAPAlertMessage::identifier() const
{
    return d->identifier;
}
QString CAPAlertMessage::sender() const
{
    return d->sender;
}
QDateTime CAPAlertMessage::sentTime() const
{
    return d->sentTime;
}
CAPAlertMessage::Status CAPAlertMessage::status() const
{
    return d->status;
}
CAPAlertMessage::MessageType CAPAlertMessage::messageType() const
{
    return d->msgType;
}
CAPAlertMessage::Scope CAPAlertMessage::scope() const
{
    return d->scope;
}
QString CAPAlertMessage::note() const
{
    return d->note;
}
const std::vector<CAPAlertInfo> &CAPAlertMessage::alertInfos() const
{
    return d->infoVec;
}
const std::vector<CAPReference> &CAPAlertMessage::references() const
{
    return d->references;
}
CAPReference CAPAlertMessage::ownReference() const
{
    return CAPReference(sender(), identifier(), sentTime());
}
void CAPAlertMessage::setIdentifier(const QString &identifier)
{
    d->identifier = identifier;
}
void CAPAlertMessage::setSender(const QString &sender)
{
    d->sender = sender;
}
void CAPAlertMessage::setSentTime(const QDateTime &dateTime)
{
    d->sentTime = dateTime;
}
void CAPAlertMessage::setStatus(Status status)
{
    d->status = status;
}
void CAPAlertMessage::setMessageType(MessageType msgType)
{
    d->msgType = msgType;
}
void CAPAlertMessage::setScope(Scope scope)
{
    d->scope = scope;
}
void CAPAlertMessage::setNote(const QString &note)
{
    d->note = note;
}
void CAPAlertMessage::addInfo(CAPAlertInfo &&alertInfo)
{
    d->infoVec.emplace_back(alertInfo);
}
void CAPAlertMessage::setReferences(std::vector<CAPReference> &&references)
{
    d->references = std::move(references);
}
}

#include "moc_capalertmessage.cpp"
