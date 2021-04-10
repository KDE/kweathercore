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
#include <memory>
#include <tuple>
#include <vector>
namespace KWeatherCore
{
// code name (FIPS6, UGC...)/code value (002050, AKZ155)
using AreaCodeVec = std::vector<std::pair<QString, QString>>;
using Parameter = std::vector<std::pair<QString, QString>>;
class KWEATHERCORE_EXPORT AlertInfo
{
    Q_GADGET
    Q_PROPERTY(QString headline READ headline WRITE setHeadline)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QString event READ event WRITE setEvent)
    Q_PROPERTY(
        QDateTime effectiveTime READ effectiveTime WRITE setEffectiveTime)
    Q_PROPERTY(QDateTime expireTime READ expireTime WRITE setExpireTime)

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
    enum class Urgency { Immediate, Expected, Future, Past, Unknown };
    enum class Severity { Extreme, Severe, Moderate, Minor, Unknown };
    enum class Certainty { Observed, Likely, Possible, Unlikely, Unknown };

    AlertInfo();
    AlertInfo(const AlertInfo &other);
    AlertInfo(AlertInfo &&other);
    ~AlertInfo();
    const QString &event() const;
    const AreaCodeVec &areaCodes() const;
    const QDateTime &effectiveTime() const;
    const QDateTime &onsetTime() const;
    const QDateTime &expireTime() const;
    const QString &headline() const;
    const QString &description() const;
    const QString &instruction() const;
    const QString &sender() const;
    const QString &language() const;
    Category category() const;
    Urgency urgency() const;
    Severity severity() const;
    Certainty certainty() const;
    const Parameter &parameter() const;
    const QString &areaDesc() const;
    const std::vector<std::vector<std::pair<float, float>>> &polygon() const;
    void setHeadline(const QString &headline);
    void setDescription(const QString &description);
    void setInstruction(const QString &instruction);
    void setSender(const QString &sender);
    void setLanguage(const QString &language);
    void setCategory(Category category);
    void addCategory(Category category);
    void setEvent(const QString &event);
    void setAreaCodes(const AreaCodeVec &areaCodes);
    void addAreaCode(std::pair<QString, QString> &areaCode);
    void setEffectiveTime(const QDateTime &time);
    void setOnsetTime(const QDateTime &time);
    void setExpireTime(const QDateTime &time);
    void setUrgency(Urgency urgency);
    void setSeverity(Severity severity);
    void setCertainty(Certainty certainty);
    void setParameter(const Parameter &parameter);
    void addParameter(std::pair<QString, QString> &);
    void setAreaDesc(const QString &areaDesc);
    void setPolygon(const std::vector<std::vector<std::pair<float, float>>> &area);
    void setPolygon(std::vector<std::vector<std::pair<float, float>>> &&area);
    void addPolygon(const std::vector<std::pair<float, float>> &area);
    void addPolygon(std::vector<std::pair<float, float>> &&area);
    AlertInfo &operator=(const AlertInfo &other);
    AlertInfo &operator=(AlertInfo &&other);

private:
    class AlertInfoPrivate;
    std::unique_ptr<AlertInfoPrivate> d;
};
using Category = KWeatherCore::AlertInfo::Category;
inline Category operator|(Category a, Category b)
{
    return static_cast<Category>(static_cast<int>(a) | static_cast<int>(b));
}

inline Category operator&(Category a, Category b)
{
    return static_cast<Category>(static_cast<int>(a) & static_cast<int>(b));
}

inline Category& operator|=(Category& a, Category b)
{
    return a = a | b;
}
}
