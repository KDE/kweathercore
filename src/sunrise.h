/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once
#include <QDateTime>
#include <QObject>
#include <QPair>
namespace KWeatherCore
{
class Sunrise
{
public:
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
    static Sunrise fromJson(QJsonObject obj);
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
