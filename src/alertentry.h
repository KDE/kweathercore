/*
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "kweathercore/kweathercore_export.h"
#include <QDateTime>
#include <QObject>
#include <memory>
#include <vector>
namespace KWeatherCore
{
class AlertInfo;
/**
 * @short Class represents single CAP
 *
 * This class contains the information of a parsed
 * CAP file
 * @see AlertInfo
 *
 * @author Anjani Kumar <anjanik012@gmail.com>
 */
class KWEATHERCORE_EXPORT AlertEntry
{
    Q_GADGET
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier)
    Q_PROPERTY(QString sender READ sender WRITE setSender)
    Q_PROPERTY(QDateTime sentTime READ sentTime WRITE setSentTime)
    Q_PROPERTY(QString note READ note WRITE setNote)
public:
    enum class Status {
        Unknown,
        Actual,
        Exercise,
        System,
        Test,
        Draft,
    };
    enum class MsgType {
        Unknown,
        Alert,
        Update,
        Cancel,
        Ack,
        Error,
    };
    enum class Scope {
        Unknown,
        Public,
        Restricted,
        Private,
    };

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
    const QString &identifier() const;
    /**
     * CAP file sender
     */
    const QString &sender() const;
    /**
     * sentTime of CAP
     * @return
     */
    const QDateTime &sentTime() const;
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
    const QString &note() const;
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
    std::unique_ptr<AlertEntryPrivate> d;
};
}
