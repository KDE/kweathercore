/*
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "kweathercore/kweathercore_export.h"
#include <QDateTime>
#include <QObject>
#include <QSharedDataPointer>
#include <vector>

namespace KWeatherCore
{
class AlertInfo;
/**
 * @short Class represents single CAP Alert Message
 *
 * This class contains the information of a parsed
 * CAP Alert Message file
 * @see AlertInfo
 * @see https://docs.oasis-open.org/emergency/cap/v1.2/CAP-v1.2.html
 *
 * @author Anjani Kumar <anjanik012@gmail.com>
 */
class KWEATHERCORE_EXPORT AlertEntry
{
    Q_GADGET
    Q_PROPERTY(QString identifier READ identifier)
    Q_PROPERTY(QString sender READ sender)
    Q_PROPERTY(QDateTime sentTime READ sentTime)
    Q_PROPERTY(QString note READ note)
    Q_PROPERTY(Status status READ status)
    Q_PROPERTY(MsgType msgType READ msgType)
    Q_PROPERTY(Scope scope READ scope)
public:
    enum class Status {
        UnknownStatus,
        Actual,
        Exercise,
        System,
        Test,
        Draft,
    };
    Q_ENUM(Status)
    enum class MsgType {
        UnknownMsgType,
        Alert,
        Update,
        Cancel,
        Ack,
        Error,
    };
    Q_ENUM(MsgType)
    enum class Scope {
        UnknownScope,
        Public,
        Restricted,
        Private,
    };
    Q_ENUM(Scope)

    /**
     * Default constructor, Status, MsgType, Scope
     * are set to Unknown
     */
    AlertEntry();
    /**
     * Copy Constructor
     */
    AlertEntry(const AlertEntry &other);
    /**
     * overloaded copy constructor
     */
    AlertEntry(AlertEntry &&other);
    ~AlertEntry();

    /**
     * identifier of CAP
     */
    QString identifier() const;
    /**
     * CAP file sender
     */
    QString sender() const;
    /**
     * sentTime of CAP
     * @return
     */
    QDateTime sentTime() const;
    /**
     * status enum, initilized to Unknown
     */
    Status status() const;
    /**
     * msgType enum, initilized to Unknown
     */
    MsgType msgType() const;
    /**
     * scope enum, initilized to Unknown
     */
    Scope scope() const;
    /**
     * note of CAP
     */
    QString note() const;
    /**
     * the parsed info entries in CAP
     * see @AlertInfo
     */
    const std::vector<AlertInfo> &infoVec() const;

    void setIdentifier(const QString &identifier);
    void setSender(const QString &sender);
    void setSentTime(const QDateTime &dateTime);
    void setStatus(Status status);
    void setMsgType(MsgType msgType);
    void setScope(Scope scope);
    void setNote(const QString &note);
    void setInfoVec(const std::vector<AlertInfo> &infoVec);
    void setInfoVec(std::vector<AlertInfo> &&infoVec);
    void addInfo(const AlertInfo &alertInfo);
    void addInfo(AlertInfo &&alertInfo);
    AlertEntry &operator=(const AlertEntry &other);
    AlertEntry &operator=(AlertEntry &&other);

private:
    class AlertEntryPrivate;
    QSharedDataPointer<AlertEntryPrivate> d;
};
}
