/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "capnamedvalue.h"
#include "kweathercore/kweathercore_export.h"
#include <QDateTime>
#include <QObject>
#include <QString>
#include <tuple>
#include <vector>

namespace KWeatherCore
{
class CAPAlertInfoPrivate;
class CAPArea;

/*!
 * \qmlvaluetype capAlertInfo
 * \inqmlmodule org.kde.weathercore
 * \nativetype KWeatherCore::CAPAlertInfo
 */

/*!
 * \class KWeatherCore::CAPAlertInfo
 * \inheaderfile KWeatherCore/CAPAlertInfo
 * \inmodule KWeatherCore
 * \brief Represents a single CAP alert message info element.
 *
 * See https://docs.oasis-open.org/emergency/cap/v1.2/CAP-v1.2.html §3.2.2
 *
 * \sa CAPAlertMessage
 */
class KWEATHERCORE_EXPORT CAPAlertInfo
{
    Q_GADGET

    /*!
     * \qmlproperty string capAlertInfo::headline
     *
     * The text headline of the alert message
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::headline
     *
     * The text headline of the alert message
     */
    Q_PROPERTY(QString headline READ headline)

    /*!
     * \qmlproperty string capAlertInfo::description
     *
     * The description of the alert message
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::description
     *
     * The description of the alert message
     */
    Q_PROPERTY(QString description READ description)

    /*!
     * \qmlproperty string capAlertInfo::event
     *
     * The text denoting the type of the subject
     * event of the alert message
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::event
     *
     * The text denoting the type of the subject
     * event of the alert message
     */
    Q_PROPERTY(QString event READ event)

    /*!
     * \qmlproperty datetime capAlertInfo::effectiveTime
     *
     * The effective time of the information of the alert message
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::effectiveTime
     *
     * The effective time of the information of the alert message
     */
    Q_PROPERTY(QDateTime effectiveTime READ effectiveTime)

    /*!
     * \qmlproperty datetime capAlertInfo::onsetTime
     *
     * The onset time of the information of the alert message
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::onsetTime
     *
     * The onset time of the information of the alert message
     */
    Q_PROPERTY(QDateTime onsetTime READ onsetTime)

    /*!
     * \qmlproperty datetime capAlertInfo::expireTime
     *
     * The expire time of the information of the alert message
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::expireTime
     *
     * The expire time of the information of the alert message
     */
    Q_PROPERTY(QDateTime expireTime READ expireTime)

    /*!
     * \qmlproperty flags capAlertInfo::categories
     * \qmlenumeratorsfrom KWeatherCore::CAPAlertInfo::Category
     *
     * The category of the alert message.
     *
     * Defaults to Unknown, value is bit or-ed
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::categories
     *
     * The category of the alert message.
     *
     * Defaults to Unknown, value is bit or-ed
     */
    Q_PROPERTY(Categories categories READ categories)

    /*!
     * \qmlproperty enumeration capAlertInfo::urgency
     * \qmlenumeratorsfrom KWeatherCore::CAPAlertInfo::Urgency
     *
     * The urgency of the alert message.
     *
     * Defaults to UnknownUrgency
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::urgency
     *
     * The urgency of the alert message.
     *
     * Defaults to UnknownUrgency
     */
    Q_PROPERTY(Urgency urgency READ urgency)

    /*!
     * \qmlproperty enumeration capAlertInfo::severity
     * \qmlenumeratorsfrom KWeatherCore::CAPAlertInfo::Severity
     *
     * The severity of the alert message.
     *
     * Defaults to UnknownSeverity
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::severity
     *
     * The severity of the alert message.
     *
     * Defaults to UnknownSeverity
     */
    Q_PROPERTY(Severity severity READ severity)

    /*!
     * \qmlproperty enumeration capAlertInfo::urgency
     * \qmlenumeratorsfrom KWeatherCore::CAPAlertInfo::Urgency
     *
     * The certainty of the alert message.
     *
     * Defaults to UnknownCertainty
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::certainty
     *
     * The certainty of the alert message.
     *
     * Defaults to UnknownCertainty
     */
    Q_PROPERTY(Certainty certainty READ certainty)

    /*!
     * \qmlproperty string capAlertInfo::sender
     *
     * The sender of the alert message.
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::sender
     *
     * The sender of the alert message.
     */
    Q_PROPERTY(QString sender READ sender)

    /*!
     * \qmlproperty string capAlertInfo::instruction
     *
     * The instruction of the alert message.
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::instruction
     *
     * The instruction of the alert message.
     */
    Q_PROPERTY(QString instruction READ instruction)

    /*!
     * \qmlproperty string capAlertInfo::language
     *
     * The code denoting the language of the info.
     *
     * Defaults to "en-US".
     *
     * Natural language identifier per [RFC 3066].
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::language
     *
     * The code denoting the language of the info.
     *
     * Defaults to "en-US".
     *
     * Natural language identifier per [RFC 3066].
     */
    Q_PROPERTY(QString language READ language)

    /*!
     * \qmlproperty flags capAlertInfo::responseTypes
     * \qmlenumeratorsfrom KWeatherCore::CAPAlertInfo::ResponseType
     *
     * Type of action recommended for the target audience of the alert.
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::responseTypes
     *
     * Type of action recommended for the target audience of the alert.
     */
    Q_PROPERTY(ResponseTypes responseTypes READ responseTypes)

    /*!
     * \qmlproperty string capAlertInfo::contact
     *
     * Describing the contact for follow-up and confirmation of the alert message.
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::contact
     *
     * Describing the contact for follow-up and confirmation of the alert message.
     */
    Q_PROPERTY(QString contact READ contact)

    /*!
     * \qmlproperty string capAlertInfo::web
     *
     * Link associating additional information with the alert message.
     */
    /*!
     * \property KWeatherCore::CAPAlertInfo::web
     *
     * Link associating additional information with the alert message.
     */
    Q_PROPERTY(QString web READ web)

public:
    /*!
     * \value Unknown
     * \value Geophysical
     * \value Meteorological
     * \value Safety
     * \value Security
     * \value Rescue
     * \value Fire
     * \value Health
     * \value Environmental
     * \value Transport
     * \value Infrastructure
     * \value CBRNE
     * \value Other
     */
    enum class Category {
        Unknown = 0,
        Geophysical = 0b1,
        Meteorological = 0b10,
        Safety = 0b100,
        Security = 0b1000,
        Rescue = 0b10000,
        Fire = 0b100000,
        Health = 0b1000000,
        Environmental = 0b10000000,
        Transport = 0b100000000,
        Infrastructure = 0b1000000000,
        CBRNE = 0b10000000000,
        Other = 0b100000000000
    };
    Q_DECLARE_FLAGS(Categories, Category)
    Q_FLAG(Categories)

    /*!
     * \value Immediate
     * \value Expected
     * \value Future
     * \value Past
     * \value UnknownUrgency
     */
    enum class Urgency { Immediate, Expected, Future, Past, UnknownUrgency };
    Q_ENUM(Urgency)

    /*!
     * \value Extreme
     * \value Severe
     * \value Moderate
     * \value Minor
     * \value UnknownSeverity
     */
    enum class Severity { Extreme, Severe, Moderate, Minor, UnknownSeverity };
    Q_ENUM(Severity)

    /*!
     * \value Observed
     * \value Likely
     * \value Possible
     * \value Unlikely
     * \value UnknownCertainty
     */
    enum class Certainty { Observed, Likely, Possible, Unlikely, UnknownCertainty };
    Q_ENUM(Certainty)

    /*!
     * \value UnknownResponseType
     * \value Shelter
     * \value Evacuate
     * \value Prepare
     * \value Execute
     * \value Avoid
     * \value Monitor
     * \value Assess
     * \value AllClear
     * \value None
     */
    enum class ResponseType {
        UnknownResponseType = 0,
        Shelter = 1 << 0,
        Evacuate = 1 << 1,
        Prepare = 1 << 2,
        Execute = 1 << 3,
        Avoid = 1 << 4,
        Monitor = 1 << 5,
        Assess = 1 << 6,
        AllClear = 1 << 7,
        None = 1 << 8,
    };
    Q_DECLARE_FLAGS(ResponseTypes, ResponseType)
    Q_FLAG(ResponseTypes)

    CAPAlertInfo();
    CAPAlertInfo(const CAPAlertInfo &other);
    CAPAlertInfo(CAPAlertInfo &&other) noexcept;
    ~CAPAlertInfo();

    QString event() const;
    QDateTime effectiveTime() const;
    QDateTime onsetTime() const;
    QDateTime expireTime() const;
    QString headline() const;
    QString description() const;
    QString instruction() const;
    QString sender() const;
    QString language() const;
    Categories categories() const;
    Urgency urgency() const;
    Severity severity() const;
    Certainty certainty() const;
    ResponseTypes responseTypes() const;
    QString contact() const;
    QString web() const;
    /*!
     * The Parameter of the alert message
     * refer to CAP protocol v1.2
     */
    const std::vector<CAPNamedValue> &parameters() const;
    /*! The areas targeted by this CAP alert message. */
    const std::vector<CAPArea> &areas() const;
    /*! System-specific codes for event typing. */
    const std::vector<CAPNamedValue> &eventCodes() const;

    void setHeadline(const QString &headline);
    void setDescription(const QString &description);
    void setInstruction(const QString &instruction);
    void setSender(const QString &sender);
    void setLanguage(const QString &language);
    void addCategory(Category category);
    void setEvent(const QString &event);
    void setEffectiveTime(const QDateTime &time);
    void setOnsetTime(const QDateTime &time);
    void setExpireTime(const QDateTime &time);
    void setUrgency(Urgency urgency);
    void setSeverity(Severity severity);
    void setCertainty(Certainty certainty);
    void addResponseType(ResponseType responseType);
    void setContact(const QString &contact);
    void setWeb(const QString &web);
    void addParameter(CAPNamedValue &&param);
    void addArea(CAPArea &&area);
    void addEventCode(CAPNamedValue &&code);

    CAPAlertInfo &operator=(const CAPAlertInfo &other);
    CAPAlertInfo &operator=(CAPAlertInfo &&other) noexcept;

private:
    QSharedDataPointer<CAPAlertInfoPrivate> d;
};
}

Q_DECLARE_OPERATORS_FOR_FLAGS(KWeatherCore::CAPAlertInfo::Categories)
Q_DECLARE_OPERATORS_FOR_FLAGS(KWeatherCore::CAPAlertInfo::ResponseTypes)
