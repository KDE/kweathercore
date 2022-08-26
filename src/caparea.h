/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef KWEATHERCORE_CAPAREA_H
#define KWEATHERCORE_CAPAREA_H

#include "capnamedvalue.h"
#include <kweathercore/kweathercore_export.h>

#include <QMetaType>
#include <QSharedDataPointer>

#include <cmath>

namespace KWeatherCore
{

using Polygon = std::vector<std::pair<float, float>>;

/** Geographic circle describing the target area of a CAP alert message. */
struct KWEATHERCORE_EXPORT CAPCircle {
    Q_GADGET
    Q_PROPERTY(float latitude MEMBER latitude)
    Q_PROPERTY(float longitude MEMBER longitude)
    Q_PROPERTY(float radius MEMBER radius)

public:
    float latitude = NAN;
    float longitude = NAN;
    float radius = NAN;
};

class CAPAreaPrivate;

/** Affected area of a CAP alert message.
 *  @see https://docs.oasis-open.org/emergency/cap/v1.2/CAP-v1.2.html §3.2.4
 */
class KWEATHERCORE_EXPORT CAPArea
{
    Q_GADGET
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(float altitude READ altitude)
    Q_PROPERTY(float ceiling READ ceiling)
public:
    CAPArea();
    CAPArea(const CAPArea &other);
    CAPArea(CAPArea &&other);
    ~CAPArea();
    CAPArea &operator=(const CAPArea &other);
    CAPArea &operator=(CAPArea &&other);

    /** A text description of the message target area. */
    QString description() const;
    void setDescription(const QString &areaDesc);

    /** Geographic polygon(s) enclosing the message target area. */
    const std::vector<Polygon> &polygons() const;
    void addPolygon(Polygon &&polygon);

    /** Geographic circles(s) enclosing the message target area. */
    const std::vector<CAPCircle> &circles() const;
    void addCircle(CAPCircle &&circle);

    /** Any geographically-based code to describe a message target area, as key/value pair. */
    const std::vector<CAPNamedValue> &geoCodes() const;
    void addGeoCode(CAPNamedValue &&geoCode);

    /** The specific or minimum altitude of the affected area of the alert message.
     *  The altitude measure is in feet above mean sea level.
     *  If not set, NAN is returned.
     */
    float altitude() const;
    void setAltitude(float altitude);

    /** The maximum altitude of the affected area of the alert message.
     *  The altitude measure is in feet above mean sea level.
     *  If not set, NAN is returned.
     */
    float ceiling() const;
    void setCeiling(float ceiling);

private:
    QSharedDataPointer<CAPAreaPrivate> d;
};

}

Q_DECLARE_METATYPE(KWeatherCore::CAPCircle)
Q_DECLARE_METATYPE(KWeatherCore::CAPArea)

#endif // KWEATHERCORE_CAPAREA_H
