/*
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <QObject>
#include <QDateTime>
#include <vector>
#include <memory>

namespace KWeatherCore
{
class AlertInfo;

class AlertEntry
{
    Q_GADGET
public:
    enum class Status {
        Actual,
        Exercise,
        System,
        Test,
        Draft
    };
    enum class MsgType {
        Alert,
        Update,
        Cancel,
        Ack,
        Error
    };
    enum class Scope {
        Public,
        Restricted,
        Private
    };
    AlertEntry();
    AlertEntry(const AlertEntry &other);
    AlertEntry(AlertEntry &&other);
    ~AlertEntry();

    const QString &identifier() const;
    const QString &sender() const;
    const QDateTime &sentTime() const;
    Status status() const;
    MsgType msgType() const;
    Scope scope() const;
    const QString &note() const;
    const std::vector<AlertInfo> &infoVec() const;

    void setIdentifier(const QString &identifier);
    void setSender(const QString &sender);
    void setSentTime(const QDateTime &dateTime);
    void setStatus(const Status &status);
    void setMsgType(const MsgType &msgType);
    void setScope(const Scope &scope);
    void setNote(const QString &note);
    void addInfo(const AlertInfo &alertInfo);

private:
    class AlertEntryPrivate;
    std::unique_ptr<AlertEntryPrivate> d;
    std::vector<AlertInfo> m_infoVec;
};
}