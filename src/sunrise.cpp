/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "sunrise.h"
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

Sunrise::Sunrise(QDateTime sunRise,
                 QDateTime sunSet,
                 QDateTime moonRise,
                 QDateTime moonSet,
                 QPair<QDateTime, double> highMoon,
                 QPair<QDateTime, double> lowMoon,
                 QPair<QDateTime, double> solarMidnight,
                 QPair<QDateTime, double> solarNoon,
                 double moonphase)
    : d(new SunrisePrivate)
{
    d->sunRise = std::move(sunRise);
    d->sunSet = std::move(sunSet);
    d->moonRise = std::move(moonRise);
    d->moonSet = std::move(moonSet);
    d->highMoon = std::move(highMoon);
    d->lowMoon = std::move(lowMoon);
    d->solarMidnight = std::move(solarMidnight);
    d->solarNoon = std::move(solarNoon);
    d->moonPhase = moonphase;
}
Sunrise::Sunrise()
    : d(new SunrisePrivate)
{
}
Sunrise::~Sunrise()
{
    delete d;
}
Sunrise Sunrise::fromJson(QJsonObject obj)
{
    Sunrise s;
    s.setSunSet(QDateTime::fromString(obj[QStringLiteral("sunset")].toString(),
                                      Qt::ISODate));
    s.setSunRise(QDateTime::fromString(
        obj[QStringLiteral("sunrise")].toString(), Qt::ISODate));
    s.setLowMoon(QPair<QDateTime, double>(
        QDateTime::fromString(obj[QStringLiteral("lowmoon")].toString(),
                              Qt::ISODate),
        obj[QStringLiteral("lowmoonEle")].toDouble()));
    s.setHighMoon(QPair<QDateTime, double>(
        QDateTime::fromString(obj[QStringLiteral("highmoon")].toString(),
                              Qt::ISODate),
        obj[QStringLiteral("highmoonEle")].toDouble()));
    s.setSolarMidnight(QPair<QDateTime, double>(
        QDateTime::fromString(obj[QStringLiteral("solarmidnight")].toString(),
                              Qt::ISODate),
        obj[QStringLiteral("solarmidnightEle")].toDouble()));
    s.setSolarNoon(QPair<QDateTime, double>(
        QDateTime::fromString(obj[QStringLiteral("solarnoon")].toString(),
                              Qt::ISODate),
        obj[QStringLiteral("solarnoonEle")].toDouble()));
    s.setMoonPhase(obj[QStringLiteral("moonphase")].toDouble());
    s.setMoonSet(QDateTime::fromString(
        obj[QStringLiteral("moonset")].toString(), Qt::ISODate));
    s.setMoonRise(QDateTime::fromString(
        obj[QStringLiteral("moonrise")].toString(), Qt::ISODate));
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
    obj[QStringLiteral("solarmidnight")] =
        solarMidnightTime().toString(Qt::ISODate);
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
    return d->highMoon.first;
}
double Sunrise::highMoon() const
{
    return d->highMoon.second;
}
QDateTime Sunrise::lowMoonTime() const
{
    return d->lowMoon.first;
}
double Sunrise::lowMoon() const
{
    return d->lowMoon.second;
}
QDateTime Sunrise::solarMidnightTime() const
{
    return d->solarMidnight.first;
}
QDateTime Sunrise::solarNoonTime() const
{
    return d->solarNoon.first;
}
double Sunrise::solarMidnight() const
{
    return d->solarMidnight.second;
}
double Sunrise::solarNoon() const
{
    return d->solarNoon.second;
}
const QDateTime &Sunrise::sunRise() const
{
    return d->sunRise;
}
const QDateTime &Sunrise::sunSet() const
{
    return d->sunSet;
}
const QDateTime &Sunrise::moonRise() const
{
    return d->moonRise;
}
const QDateTime &Sunrise::moonSet() const
{
    return d->moonSet;
}
double Sunrise::moonPhase() const
{
    return d->moonPhase;
}
void Sunrise::setHighMoon(const QPair<QDateTime, double> &highMoon)
{
    d->highMoon = highMoon;
}
void Sunrise::setSolarMidnight(const QPair<QDateTime, double> &solarMidnight)
{
    d->solarMidnight = solarMidnight;
}
void Sunrise::setSolarNoon(const QPair<QDateTime, double> &solarNoon)
{
    d->solarNoon = solarNoon;
}
void Sunrise::setLowMoon(const QPair<QDateTime, double> &lowMoon)
{
    d->lowMoon = lowMoon;
}
void Sunrise::setSunRise(const QDateTime &sunRise)
{
    d->sunRise = sunRise;
}
void Sunrise::setSunSet(const QDateTime &sunSet)
{
    d->sunSet = sunSet;
}
void Sunrise::setMoonRise(const QDateTime &moonRise)
{
    d->moonRise = moonRise;
}
void Sunrise::setMoonSet(const QDateTime &moonSet)
{
    d->moonSet = moonSet;
}
void Sunrise::setMoonPhase(double moonPhase)
{
    d->moonPhase = moonPhase;
}
}
