/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "caparea.h"

using namespace KWeatherCore;

namespace KWeatherCore
{
class CAPAreaPrivate : public QSharedData
{
public:
    QString description;
    std::vector<CAPPolygon> polygons;
    std::vector<CAPCircle> circles;
    std::vector<CAPNamedValue> geoCodes;
    float altitude = NAN;
    float ceiling = NAN;
};
}

CAPArea::CAPArea()
    : d(new CAPAreaPrivate)
{
}

CAPArea::CAPArea(const CAPArea &other) = default;
CAPArea::CAPArea(CAPArea &&other) noexcept = default;
CAPArea::~CAPArea() = default;
CAPArea &CAPArea::operator=(const CAPArea &other) = default;
CAPArea &CAPArea::operator=(CAPArea &&other) noexcept = default;

QString CAPArea::description() const
{
    return d->description;
}

void CAPArea::setDescription(const QString &areaDesc)
{
    d->description = areaDesc;
}

const std::vector<CAPPolygon> &CAPArea::polygons() const
{
    return d->polygons;
}

void CAPArea::addPolygon(CAPPolygon &&polygon)
{
    d->polygons.push_back(std::move(polygon));
}

const std::vector<CAPCircle> &CAPArea::circles() const
{
    return d->circles;
}

void CAPArea::addCircle(CAPCircle &&circle)
{
    d->circles.push_back(std::move(circle));
}

const std::vector<CAPNamedValue> &CAPArea::geoCodes() const
{
    return d->geoCodes;
}

void CAPArea::addGeoCode(CAPNamedValue &&geoCode)
{
    d->geoCodes.push_back(std::move(geoCode));
}

float CAPArea::altitude() const
{
    return d->altitude;
}

void CAPArea::setAltitude(float altitude)
{
    d->altitude = altitude;
}

float CAPArea::ceiling() const
{
    return d->ceiling;
}

void CAPArea::setCeiling(float ceiling)
{
    d->ceiling = ceiling;
}

#include "moc_caparea.cpp"
