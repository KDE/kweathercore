/*
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "kweathercore/kweathercore_export.h"

#include <QDateTime>
#include <QMetaType>
#include <QSharedDataPointer>
#include <vector>

namespace KWeatherCore
{
class CAPAlertInfo;
class CAPAlertMessagePrivate;
class CAPReference;
/*!
 * @short Represents a single CAP Alert Message
 *
 * This class contains the information of a parsed CAP alert message.
 * @see CAPAlertInfo
 * @see https://docs.oasis-open.org/emergency/cap/v1.2/CAP-v1.2.html $3.2.1
 *
 * @author Anjani Kumar <anjanik012@gmail.com>
 */
class KWEATHERCORE_EXPORT CAPAlertMessage
{
    Q_GADGET
    Q_PROPERTY(QString identifier READ identifier)
    Q_PROPERTY(QString sender READ sender)
    Q_PROPERTY(QDateTime sentTime READ sentTime)
    Q_PROPERTY(QString note READ note)
    Q_PROPERTY(Status status READ status)
    Q_PROPERTY(MessageType messageType READ messageType)
    Q_PROPERTY(Scope scope READ scope)
public:
    /*! The code denoting the appropriate handling of the alert message. */
    enum class Status {
        UnknownStatus,
        Actual, ///< Actionable by all targeted recipients
        Exercise, ///< Actionable only by designated exercise participants
        System, //< For messages that support alert network internal functions
        Test, ///< Technical testing only, all recipients disregard
        Draft, ///< A preliminary template or draft, not actionable in its current form
    };
    Q_ENUM(Status)
    enum class MessageType {
        UnknownMessageType,
        Alert, ///< Initial information requiring attention by targeted recipients
        Update, ///< Updates and supercedes the earlier message(s) identified in references()
        Cancel, ///< Cancels the earlier message(s) identified in references()
        Acknowledge, ///< Acknowledges receipt and acceptance of the message(s) identified in references()
        Error, ///< Indicates rejection of the message(s) identified in references()
    };
    Q_ENUM(MessageType)
    enum class Scope {
        UnknownScope,
        Public, ///< For general dissemination to unrestricted audiences
        Restricted, ///< For dissemination only to users with a known operational requirement
        Private, ///< For dissemination only to specified addresses
    };
    Q_ENUM(Scope)

    /*!
     * Default constructor, Status, MsgType, Scope
     * are set to Unknown
     */
    CAPAlertMessage();
    CAPAlertMessage(const CAPAlertMessage &other);
    CAPAlertMessage(CAPAlertMessage &&other) noexcept;
    ~CAPAlertMessage();

    /*!
     * Unique alert message identifier.
     */
    QString identifier() const;
    /*!
     * The identifier of the sender of the alert message.
     */
    QString sender() const;
    /*!
     * The time and date of the origination of the alert message.
     */
    QDateTime sentTime() const;
    /*!
     * The code denoting the appropriate handling of the alert message.
     * Returns UnknownStatus if not set.
     */
    Status status() const;
    /*!
     * The code denoting the nature of the alert message.
     * Returns UnknownMsgType if not set.
     */
    MessageType messageType() const;
    /*!
     * The code denoting the intended distribution of the alert message.
     * Returns UnknownScope if not set.
     */
    Scope scope() const;
    /*!
     * The text describing the purpose or significance of the alert message.
     * Relevant for Exercise and Error status.
     */
    QString note() const;
    /*!
     * The alert info elements of this alert message.
     * @see CAPAlertInfo
     */
    const std::vector<CAPAlertInfo> &alertInfos() const;
    /*!
     * References to previous CAP alert messages.
     * Relevant for Update, Cancel and Ack message types.
     */
    const std::vector<CAPReference> &references() const;
    /*!
     * Reference of this CAP alert message.
     * @see references()
     */
    CAPReference ownReference() const;

    ///@cond internal
    // only for internal use by CAPParser
    void setIdentifier(const QString &identifier);
    void setSender(const QString &sender);
    void setSentTime(const QDateTime &dateTime);
    void setStatus(Status status);
    void setMessageType(MessageType msgType);
    void setScope(Scope scope);
    void setNote(const QString &note);
    void addInfo(CAPAlertInfo &&alertInfo);
    void setReferences(std::vector<CAPReference> &&references);
    ///@endcond

    CAPAlertMessage &operator=(const CAPAlertMessage &other);
    CAPAlertMessage &operator=(CAPAlertMessage &&other) noexcept;

private:
    QSharedDataPointer<CAPAlertMessagePrivate> d;
};
}

Q_DECLARE_METATYPE(KWeatherCore::CAPAlertMessage)
