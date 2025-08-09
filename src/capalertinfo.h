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
/**
 * @short Represents a single CAP alert message info element.
 *
 * @see CAPAlertMessage
 * @see https://docs.oasis-open.org/emergency/cap/v1.2/CAP-v1.2.html §3.2.2
 * @author Han Young <hanyoung@protonmail.com>
 * @author Anjani Kumar <anjanik012@gmail.com>
 */
class KWEATHERCORE_EXPORT CAPAlertInfo
{
    Q_GADGET
    Q_PROPERTY(QString headline READ headline)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QString event READ event)
    Q_PROPERTY(QDateTime effectiveTime READ effectiveTime)
    Q_PROPERTY(QDateTime onsetTime READ onsetTime)
    Q_PROPERTY(QDateTime expireTime READ expireTime)
    Q_PROPERTY(Categories categories READ categories)
    Q_PROPERTY(Urgency urgency READ urgency)
    Q_PROPERTY(Severity severity READ severity)
    Q_PROPERTY(Certainty certainty READ certainty)
    Q_PROPERTY(QString sender READ sender)
    Q_PROPERTY(QString instruction READ instruction)
    Q_PROPERTY(QString language READ language)
    Q_PROPERTY(ResponseTypes responseTypes READ responseTypes)
    Q_PROPERTY(QString contact READ contact)
    Q_PROPERTY(QString web READ web)

    Q_PROPERTY(bool hasGeometry READ hasGeometry STORED false)

public:
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
    enum class Urgency { Immediate, Expected, Future, Past, UnknownUrgency };
    Q_ENUM(Urgency)
    enum class Severity { Extreme, Severe, Moderate, Minor, UnknownSeverity };
    Q_ENUM(Severity)
    enum class Certainty { Observed, Likely, Possible, Unlikely, UnknownCertainty };
    Q_ENUM(Certainty)

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
    /**
     * The text denoting the type of the subject
     * event of the alert message
     */
    QString event() const;
    /**
     * The effective time of the information of the alert message
     */
    QDateTime effectiveTime() const;
    /**
     * The onset time of the information of the alert message
     */
    QDateTime onsetTime() const;
    /**
     * The expire time of the information of the alert message
     */
    QDateTime expireTime() const;
    /**
     * The text headline of the alert message
     */
    QString headline() const;
    /**
     * The description of the alert message
     */
    QString description() const;
    /**
     * The instruction of the alert message
     */
    QString instruction() const;
    /**
     * The sender of the alert message
     */
    QString sender() const;
    /**
     * The code denoting the language of the info
     * default to "en-US"
     * @return Natural language identifier per [RFC 3066].
     */
    QString language() const;
    /**
     * The category of the alert message
     * @return default to Unknown, value is bit or-ed
     */
    Categories categories() const;
    /**
     * The urgency of the alert message
     * @return default to UnknownUrgency
     */
    Urgency urgency() const;
    /**
     * The severity of the alert message
     * @return default to UnknownSeverity
     */
    Severity severity() const;
    /**
     * The certainty of the alert message
     * @return default to UnknownCertainty
     */
    Certainty certainty() const;
    /**
     * Type of action recommended for the target audience of the alert.
     */
    ResponseTypes responseTypes() const;
    /**
     * Describing the contact for follow-up and confirmation of the alert message.
     */
    QString contact() const;
    /**
     * Link associating additional information with the alert message.
     */
    QString web() const;
    /**
     * The Parameter of the alert message
     * refer to CAP protocol v1.2
     */
    const std::vector<CAPNamedValue> &parameters() const;
    /** The areas targeted by this CAP alert message. */
    const std::vector<CAPArea> &areas() const;
    /** System-specific codes for event typing. */
    const std::vector<CAPNamedValue> &eventCodes() const;

    /** Returns @c true if there is at least one CAPArea with at least one polygon
     *  or circle geometry (which implies that intersection tests or map display are possible).
     *  @since 25.12
     */
    [[nodiscard]] bool hasGeometry() const;

    ///@cond internal
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
    ///@endcond

    CAPAlertInfo &operator=(const CAPAlertInfo &other);
    CAPAlertInfo &operator=(CAPAlertInfo &&other) noexcept;

private:
    QSharedDataPointer<CAPAlertInfoPrivate> d;
};
}

Q_DECLARE_OPERATORS_FOR_FLAGS(KWeatherCore::CAPAlertInfo::Categories)
Q_DECLARE_OPERATORS_FOR_FLAGS(KWeatherCore::CAPAlertInfo::ResponseTypes)
