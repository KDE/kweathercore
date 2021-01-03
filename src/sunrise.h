/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include <memory>
#include <QDateTime>
#include <QJsonObject>
#include <QObject>
#include <QPair>
#include <kweathercore/kweathercore_export.h>
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
public:
    /**
     * Sunrise construct Sunrise with given data, all QDateTime are in
     * location's timezone
     * @param sunRise sun rise time
     * @param sunSet sun set time
     * @param moonRise moon rise time
     * @param moonSet moon set time
     * @param highMoon high moon time and elevation
     * @param lowMoon low moon time time and elevation
     * @param solarMidnight solar midnight time and elevation
     * @param solarNoon solar noon time and elevation
     * @param moonphase time and value
     *        0..25: "waxing crescent";
     *        25..50: "waxing gibbous";
     *        50..75: "waning gibbous";
     *        75..100: "waning crescent";
     */
    Sunrise(QDateTime sunRise,
            QDateTime sunSet,
            QDateTime moonRise,
            QDateTime moonSet,
            QPair<QDateTime, double> highMoon,
            QPair<QDateTime, double> lowMoon,
            QPair<QDateTime, double> solarMidnight,
            QPair<QDateTime, double> solarNoon,
            double moonphase);
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
    void setHighMoon(const QPair<QDateTime, double> &highMoon);
    void setSolarMidnight(const QPair<QDateTime, double> &solarMidnight);
    void setSolarNoon(const QPair<QDateTime, double> &solarNoon);
    void setLowMoon(const QPair<QDateTime, double> &lowMoon);
    void setSunRise(const QDateTime &sunRise);
    void setSunSet(const QDateTime &sunSet);
    void setMoonRise(const QDateTime &moonRise);
    void setMoonSet(const QDateTime &moonSet);
    void setMoonPhase(double moonPhase);

private:
    std::unique_ptr<SunrisePrivate> d;
};
}
