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
enum class MsgType { Alert, Update, Cancel, Ack, Error };
enum class Urgency {
    DefaultUrgency = -1,
    LowUrgency = 10,
    NormalUrgency = 50,
    HighUrgency = 70,
    CriticalUrgency = 90
};

class AlertEntryPrivate
{
public:
    QString title;
    QString summary;
    QString event;
    QDateTime effectiveTime;
    QDateTime expireTime;
    MsgType msgType;
    Urgency urgency;
    std::vector<std::tuple<float, float>> area;
};
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
    AlertEntry();
    AlertEntry(const AlertEntry &other);
    const QString &title() const;
    const QString &summary() const;
    const QString &event() const;
    const QDateTime &effectiveTime() const;
    const QDateTime &expireTime() const;
    MsgType msgType() const;
    Urgency urgency() const;
    const std::vector<std::tuple<float, float>> &area() const;
    void setTitle(const QString &title);
    void setSummary(const QString &summary);
    void setEvent(const QString &event);
    void setEffectiveTime(const QDateTime &time);
    void setExpireTime(const QDateTime &time);
    void setMsgType(MsgType type);
    void setUrgency(Urgency urgency);
    void setArea(const std::vector<std::tuple<float, float>> &area);
    void setArea(std::vector<std::tuple<float, float>> &&area);
    AlertEntry &operator=(const AlertEntry &other);

private:
    std::unique_ptr<AlertEntryPrivate> d;
};
}
