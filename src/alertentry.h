/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include <QDateTime>
#include <QObject>
#include <QString>
#include <memory>
#include <vector>
#include <tuple>
namespace KWeatherCore
{
// code name (FIPS6, UGC...)/code value (002050, AKZ155)
using AreaCodeVec = std::vector<std::pair<QString, QString>>;
class AlertEntry
{
    Q_GADGET
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString summary READ summary WRITE setSummary)
    Q_PROPERTY(QString event READ event WRITE setEvent)
    Q_PROPERTY(
        QDateTime effectiveTime READ effectiveTime WRITE setEffectiveTime)
    Q_PROPERTY(QDateTime expireTime READ expireTime WRITE setExpireTime)

public:
    enum class MsgType { Alert, Update, Cancel, Ack, Error };
    enum class Urgency {
        Immediate,
        Expected,
        Future,
        Past,
        Unknown
    };
    enum class Severity {
        Extreme,
        Severe,
        Moderate,
        Minor,
        Unknown
    };
    enum class Certainty {
        Observed,
        Likely,
        Possible,
        Unlikely,
        Unknown
    };

    AlertEntry();
    AlertEntry(const AlertEntry &other);
    AlertEntry(AlertEntry &&other);
    ~AlertEntry();
    const QString &title() const;
    const QString &summary() const;
    const QString &event() const;
    const QStringList &areaNames() const;
    const AreaCodeVec &areaCodes() const;
    const QDateTime &effectiveTime() const;
    const QDateTime &expireTime() const;
    MsgType msgType() const;
    Urgency urgency() const;
    Severity severity() const;
    Certainty certainty() const;
    const std::vector<std::pair<float, float>> &polygon() const;
    void setTitle(const QString &title);
    void setSummary(const QString &summary);
    void setEvent(const QString &event);
    void setAreaNames(const QStringList &areas);
    void setAreaCodes(const AreaCodeVec &areaCodes);
    void setEffectiveTime(const QDateTime &time);
    void setExpireTime(const QDateTime &time);
    void setMsgType(MsgType type);
    void setUrgency(Urgency urgency);
    void setSeverity(Severity severity);
    void setCertainty(Certainty certainty);
    void setPolygon(const std::vector<std::pair<float, float>> &area);
    void setPolygon(std::vector<std::pair<float, float>> &&area);
    AlertEntry &operator=(const AlertEntry &other);
    AlertEntry &operator=(AlertEntry &&other);

private:
    class AlertEntryPrivate;
    std::unique_ptr<AlertEntryPrivate> d;
};
}
