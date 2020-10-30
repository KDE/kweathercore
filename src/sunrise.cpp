/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "sunrise.h"
#include <QJsonObject>
namespace KWeatherCore
{
Sunrise::Sunrise(const QDateTime &sunRise,
                 const QDateTime &sunSet,
                 const QDateTime &moonRise,
                 const QDateTime &moonSet,
                 const QPair<QDateTime, double> &highMoon,
                 const QPair<QDateTime, double> &lowMoon,
                 const QPair<QDateTime, double> &solarMidnight,
                 const QPair<QDateTime, double> &solarNoon,
                 double moonphase)
    : m_sunRise(sunRise)
    , m_sunSet(sunSet)
    , m_moonRise(moonRise)
    , m_moonSet(moonSet)
    , m_highMoon(highMoon)
    , m_lowMoon(lowMoon)
    , m_solarMidnight(solarMidnight)
    , m_solarNoon(solarNoon)
    , m_moonPhase(moonphase)
{
}

Sunrise Sunrise::fromJson(QJsonObject obj)
{
    Sunrise s;
    s.setSunSet(QDateTime::fromString(obj[QStringLiteral("sunset")].toString(), Qt::ISODate));
    s.setSunRise(QDateTime::fromString(obj[QStringLiteral("sunrise")].toString(), Qt::ISODate));
    s.setLowMoon(QPair<QDateTime, double>(QDateTime::fromString(obj[QStringLiteral("lowmoon")].toString(), Qt::ISODate), obj[QStringLiteral("lowmoonEle")].toDouble()));
    s.setHighMoon(QPair<QDateTime, double>(QDateTime::fromString(obj[QStringLiteral("highmoon")].toString(), Qt::ISODate), obj[QStringLiteral("highmoonEle")].toDouble()));
    s.setSolarMidnight(QPair<QDateTime, double>(QDateTime::fromString(obj[QStringLiteral("solarmidnight")].toString(), Qt::ISODate), obj[QStringLiteral("solarmidnightEle")].toDouble()));
    s.setSolarNoon(QPair<QDateTime, double>(QDateTime::fromString(obj[QStringLiteral("solarnoon")].toString(), Qt::ISODate), obj[QStringLiteral("solarnoonEle")].toDouble()));
    s.setMoonPhase(obj[QStringLiteral("moonphase")].toDouble());
    s.setMoonSet(QDateTime::fromString(obj[QStringLiteral("moonset")].toString(), Qt::ISODate));
    s.setMoonRise(QDateTime::fromString(obj[QStringLiteral("moonrise")].toString(), Qt::ISODate));
    return s;
}
QJsonObject Sunrise::toJson()
{
    QJsonObject obj;
    obj[QStringLiteral("sunrise")] = sunRise().toString(Qt::ISODate);
    obj[QStringLiteral("sunset")] = sunSet().toString(Qt::ISODate);
    obj[QStringLiteral("moonrise")] = moonRise().toString(Qt::ISODate);
    obj[QStringLiteral("moonset")] = moonSet().toString(Qt::ISODate);
    obj[QStringLiteral("lowmoon")] = lowMoonTime().toString(Qt::ISODate);
    obj[QStringLiteral("solarmidnight")] = solarMidnightTime().toString(Qt::ISODate);
    obj[QStringLiteral("solarnoon")] = solarNoonTime().toString(Qt::ISODate);
    obj[QStringLiteral("highmoon")] = highMoonTime().toString(Qt::ISODate);
    obj[QStringLiteral("lowmoonEle")] = lowMoon();
    obj[QStringLiteral("highmoonEle")] = highMoon();
    obj[QStringLiteral("solarmidnightEle")] = solarMidnight();
    obj[QStringLiteral("solarNoonEle")] = solarNoon();
    obj[QStringLiteral("moonphase")] = moonPhase();
    return obj;
}

QDateTime Sunrise::highMoonTime() const
{
    return m_highMoon.first;
}
double Sunrise::highMoon() const
{
    return m_highMoon.second;
}
QDateTime Sunrise::lowMoonTime() const
{
    return m_lowMoon.first;
}
double Sunrise::lowMoon() const
{
    return m_lowMoon.second;
}
QDateTime Sunrise::solarMidnightTime() const
{
    return m_solarMidnight.first;
}
QDateTime Sunrise::solarNoonTime() const
{
    return m_solarNoon.first;
}
double Sunrise::solarMidnight() const
{
    return m_solarMidnight.second;
}
double Sunrise::solarNoon() const
{
    return m_solarNoon.second;
}
const QDateTime &Sunrise::sunRise() const
{
    return m_sunRise;
}
const QDateTime &Sunrise::sunSet() const
{
    return m_sunSet;
}
const QDateTime &Sunrise::moonRise() const
{
    return m_moonRise;
}
const QDateTime &Sunrise::moonSet() const
{
    return m_moonSet;
}
double Sunrise::moonPhase() const
{
    return m_moonPhase;
}
void Sunrise::setHighMoon(const QPair<QDateTime, double> &highMoon)
{
    m_highMoon = highMoon;
}
void Sunrise::setSolarMidnight(const QPair<QDateTime, double> &solarMidnight)
{
    m_solarMidnight = solarMidnight;
}
void Sunrise::setSolarNoon(const QPair<QDateTime, double> &solarNoon)
{
    m_solarNoon = solarNoon;
}
void Sunrise::setLowMoon(const QPair<QDateTime, double> &lowMoon)
{
    m_lowMoon = lowMoon;
}
void Sunrise::setSunRise(const QDateTime &sunRise)
{
    m_sunRise = sunRise;
}
void Sunrise::setSunSet(const QDateTime &sunSet)
{
    m_sunSet = sunSet;
}
void Sunrise::setMoonRise(const QDateTime &moonRise)
{
    m_moonRise = moonRise;
}
void Sunrise::setMoonSet(const QDateTime &moonSet)
{
    m_moonSet = moonSet;
}
void Sunrise::setMoonPhase(double moonPhase)
{
    m_moonPhase = moonPhase;
}
}
