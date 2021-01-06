/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include <QDateTime>
#include <QJsonObject>
#include <QObject>
#include <QPair>
#include <kweathercore/kweathercore_export.h>
#include <memory>
namespace KWeatherCore
{
class SunrisePrivate
{
public:
    QPair<QDateTime, double> highMoon;
    QPair<QDateTime, double> solarMidnight;
    QPair<QDateTime, double> solarNoon;
    QPair<QDateTime, double> lowMoon;
    QDateTime sunRise = QDateTime::currentDateTime();
    QDateTime sunSet = QDateTime::currentDateTime();
    QDateTime moonRise = QDateTime::currentDateTime();
    QDateTime moonSet = QDateTime::currentDateTime();
    double moonPhase;
};
/**
 * @short The Sunrise class contains the information of sunrise/set on a day and
 * more
 *
 * @see SunriseSource
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT Sunrise
{
    Q_GADGET
    Q_PROPERTY(QDateTime highMoonTime READ highMoonTime)
    Q_PROPERTY(QDateTime lowMoonTime READ lowMoonTime)
    Q_PROPERTY(QDateTime solarMidnightTime READ solarMidnightTime)
    Q_PROPERTY(QDateTime solarNoonTime READ solarNoonTime)
    Q_PROPERTY(QDateTime sunRise READ sunRise WRITE setSunRise)
    Q_PROPERTY(QDateTime sunSet READ sunSet WRITE setSunSet)
    Q_PROPERTY(QDateTime moonRise READ moonRise WRITE setMoonRise)
    Q_PROPERTY(QDateTime moonSet READ moonSet WRITE setMoonSet)
    Q_PROPERTY(qreal moonPhase READ moonPhase WRITE setMoonPhase)
    Q_PROPERTY(qreal highMoon READ highMoon)
    Q_PROPERTY(qreal lowMoon READ lowMoon)
    Q_PROPERTY(qreal solarNoon READ solarNoon)
    Q_PROPERTY(qreal solarMidnight READ solarMidnight)

public:
    Sunrise();
    Sunrise(const Sunrise &other);
    Sunrise &operator=(const Sunrise &other);
    /**
     * construct from json
     */
    static Sunrise fromJson(QJsonObject obj);
    /**
     * convert to json
     */
    QJsonObject toJson();
    QDateTime highMoonTime() const;
    double highMoon() const;
    QDateTime lowMoonTime() const;
    double lowMoon() const;
    QDateTime solarMidnightTime() const;
    QDateTime solarNoonTime() const;
    double solarMidnight() const;
    double solarNoon() const;
    const QDateTime &sunRise() const;
    const QDateTime &sunSet() const;
    const QDateTime &moonRise() const;
    const QDateTime &moonSet() const;
    double moonPhase() const;
    /**
     * set high moon time and elevation
     */
    void setHighMoon(const QPair<QDateTime, double> &highMoon);
    /**
     * set solar midnight time and elevation
     */
    void setSolarMidnight(const QPair<QDateTime, double> &solarMidnight);
    /**
     * set solar noon time and elevation
     */
    void setSolarNoon(const QPair<QDateTime, double> &solarNoon);
    /**
     * set low moon time time and elevation
     */
    void setLowMoon(const QPair<QDateTime, double> &lowMoon);
    /**
     * sun rise time
     */
    void setSunRise(const QDateTime &sunRise);
    /**
     * set sun set time
     */
    void setSunSet(const QDateTime &sunSet);
    /**
     * set moon rise time
     */
    void setMoonRise(const QDateTime &moonRise);
    /**
     * set moon set time
     */
    void setMoonSet(const QDateTime &moonSet);
    /**
     * set moon phase
     * @param moonPhase
     *        0..25: "waxing crescent";
     *        25..50: "waxing gibbous";
     *        50..75: "waning gibbous";
     *        75..100: "waning crescent";
     */
    void setMoonPhase(double moonPhase);

private:
    std::unique_ptr<SunrisePrivate> d;
};
}
