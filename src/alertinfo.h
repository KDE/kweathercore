/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "kweathercore/kweathercore_export.h"
#include <QDateTime>
#include <QObject>
#include <QString>
#include <tuple>
#include <vector>

namespace KWeatherCore
{
using Parameter = std::vector<std::pair<QString, QString>>;
class CAPArea;
/**
 * @short Class represents single CAP
 *
 * This class contains the parsed CAP FEED entry
 *
 * @author Han Young <hanyoung@protonmail.com>
 * @author Anjani Kumar <anjanik012@gmail.com>
 */
class KWEATHERCORE_EXPORT AlertInfo
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

public:
    enum class Category {
        Unknown = 0,
        Geo = 0b1,
        Met = 0b10,
        Safety = 0b100,
        Security = 0b1000,
        Rescue = 0b10000,
        Fire = 0b100000,
        Health = 0b1000000,
        Env = 0b10000000,
        Transport = 0b100000000,
        Infra = 0b1000000000,
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

    /**
     * default constructor
     */
    AlertInfo();
    AlertInfo(const AlertInfo &other);
    AlertInfo(AlertInfo &&other);
    ~AlertInfo();
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
     * @return default to Unknown
     */
    Urgency urgency() const;
    /**
     * The severity of the alert message
     * @return default to Unknown
     */
    Severity severity() const;
    /**
     * The certainty of the alert message
     * @return default to Unknown
     */
    Certainty certainty() const;
    /**
     * The Parameter of the alert message
     * refer to CAP protocol v1.2
     */
    const Parameter &parameter() const;
    /** The areas targeted by this CAP alert message. */
    const std::vector<CAPArea> &areas() const;

    void setHeadline(const QString &headline);
    void setDescription(const QString &description);
    void setInstruction(const QString &instruction);
    void setSender(const QString &sender);
    void setLanguage(const QString &language);
    void setCategory(Category category);
    void addCategory(Category category);
    void setEvent(const QString &event);
    void setEffectiveTime(const QDateTime &time);
    void setOnsetTime(const QDateTime &time);
    void setExpireTime(const QDateTime &time);
    void setUrgency(Urgency urgency);
    void setSeverity(Severity severity);
    void setCertainty(Certainty certainty);
    void setParameter(const Parameter &parameter);
    void addParameter(std::pair<QString, QString> &);
    void addArea(CAPArea &&area);
    AlertInfo &operator=(const AlertInfo &other);
    AlertInfo &operator=(AlertInfo &&other);

private:
    class AlertInfoPrivate;
    QSharedDataPointer<AlertInfoPrivate> d;
};
}

Q_DECLARE_OPERATORS_FOR_FLAGS(KWeatherCore::AlertInfo::Categories)
