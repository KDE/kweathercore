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
    std::vector<Polygon> polygons;
    std::vector<CAPCircle> circles;
    std::vector<CAPNamedValue> geoCodes;
};
}

CAPArea::CAPArea()
    : d(new CAPAreaPrivate)
{
}

CAPArea::CAPArea(const CAPArea &other) = default;
CAPArea::CAPArea(CAPArea &&other) = default;
CAPArea::~CAPArea() = default;
CAPArea &CAPArea::operator=(const CAPArea &other) = default;
CAPArea &CAPArea::operator=(CAPArea &&other) = default;

QString CAPArea::description() const
{
    return d->description;
}

void CAPArea::setDescription(const QString &areaDesc)
{
    d->description = areaDesc;
}

const std::vector<Polygon> &CAPArea::polygons() const
{
    return d->polygons;
}

void CAPArea::addPolygon(Polygon &&polygon)
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
