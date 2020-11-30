/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once
#include <QDateTime>
#include <QJsonObject>
#include <QObject>
#include <QPair>
#include <kweathercore/kweathercore_export.h>
namespace KWeatherCore
{
/**
 * @short The Sunrise class contains the information of sunrise/set on a day and more
 *
 * @see SunriseSource
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT Sunrise
{
public:
    /**
     * @brief Sunrise construct Sunrise with given data
     * @param sunRise
     * @param sunSet
     * @param moonRise
     * @param moonSet
     * @param highMoon
     * @param lowMoon
     * @param solarMidnight
     * @param solarNoon
     * @param moonphase
     */
    Sunrise(const QDateTime &sunRise,
            const QDateTime &sunSet,
            const QDateTime &moonRise,
            const QDateTime &moonSet,
            const QPair<QDateTime, double> &highMoon,
            const QPair<QDateTime, double> &lowMoon,
            const QPair<QDateTime, double> &solarMidnight,
            const QPair<QDateTime, double> &solarNoon,
            double moonphase);
    Sunrise();
    /**
     * @brief fromJson construct from json
     * @param obj
     * @return
     */
    static Sunrise fromJson(QJsonObject obj);
    /**
     * @brief toJson convert to json
     * @return
     */
    QJsonObject toJson();
    /**
     * @brief highMoonTime in local timezone
     * @return
     */
    QDateTime highMoonTime() const;
    /**
     * @brief highMoon
     * @return
     */
    double highMoon() const;
    /**
     * @brief lowMoonTime in local timezone
     * @return
     */
    QDateTime lowMoonTime() const;
    /**
     * @brief lowMoon
     * @return
     */
    double lowMoon() const;
    /**
     * @brief solarMidnightTime in local timezone
     * @return
     */
    QDateTime solarMidnightTime() const;
    /**
     * @brief solarNoonTime in local timezone
     * @return
     */
    QDateTime solarNoonTime() const;
    /**
     * @brief solarMidnight
     * @return
     */
    double solarMidnight() const;
    /**
     * @brief solarNoon
     * @return
     */
    double solarNoon() const;
    /**
     * @brief sunRise in local timezone
     * @return
     */
    const QDateTime &sunRise() const;
    /**
     * @brief sunSet in local timezone
     * @return
     */
    const QDateTime &sunSet() const;
    /**
     * @brief moonRise in local timezone
     * @return
     */
    const QDateTime &moonRise() const;
    /**
     * @brief moonSet in local timezone
     * @return
     */
    const QDateTime &moonSet() const;
    /**
     * @brief moonPhase
     * @return
     */
    double moonPhase() const;
    /**
     * @brief setHighMoon
     * @param highMoon
     */
    void setHighMoon(const QPair<QDateTime, double> &highMoon);
    /**
     * @brief setSolarMidnight
     * @param solarMidnight
     */
    void setSolarMidnight(const QPair<QDateTime, double> &solarMidnight);
    /**
     * @brief setSolarNoon
     * @param solarNoon
     */
    void setSolarNoon(const QPair<QDateTime, double> &solarNoon);
    /**
     * @brief setLowMoon
     * @param lowMoon
     */
    void setLowMoon(const QPair<QDateTime, double> &lowMoon);
    /**
     * @brief setSunRise
     * @param sunRise
     */
    void setSunRise(const QDateTime &sunRise);
    /**
     * @brief setSunSet
     * @param sunSet in local timezone
     */
    void setSunSet(const QDateTime &sunSet);
    /**
     * @brief setMoonRise
     * @param moonRise in local timezone
     */
    void setMoonRise(const QDateTime &moonRise);
    /**
     * @brief setMoonSet
     * @param moonSet in local timezone
     */
    void setMoonSet(const QDateTime &moonSet);
    /**
     * @brief setMoonPhase
     * @param moonPhase in local timezone
     */
    void setMoonPhase(double moonPhase);

private:
    QPair<QDateTime, double> m_highMoon;
    QPair<QDateTime, double> m_solarMidnight;
    QPair<QDateTime, double> m_solarNoon;
    QPair<QDateTime, double> m_lowMoon;
    QDateTime m_sunRise = QDateTime::currentDateTime();
    QDateTime m_sunSet = QDateTime::currentDateTime();
    QDateTime m_moonRise = QDateTime::currentDateTime();
    QDateTime m_moonSet = QDateTime::currentDateTime();
    double m_moonPhase;
};
}
