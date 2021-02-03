/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "locationqueryresult.h"
#include <memory>
namespace KWeatherCore
{
class LocationQueryResult::LocationQueryResultPrivate
{
public:
    double latitude, longitude;
    QString toponymName, name, countryCode, countryName, geonameId;
};
LocationQueryResult::LocationQueryResult()
    : d(new LocationQueryResultPrivate())
{
}
LocationQueryResult::LocationQueryResult(LocationQueryResult &&other) = default;
LocationQueryResult::LocationQueryResult(double latitude,
                                         double longitude,
                                         QString toponymName,
                                         QString name,
                                         QString countryCode,
                                         QString countryName,
                                         QString geonameId)
    : d(new LocationQueryResultPrivate())
{
    d->latitude = latitude;
    d->longitude = longitude;
    d->toponymName = std::move(toponymName);
    d->name = std::move(name);
    d->countryCode = std::move(countryCode);
    d->countryName = std::move(countryName);
    d->geonameId = std::move(geonameId);
}
LocationQueryResult::LocationQueryResult(const LocationQueryResult &other)
    : d(new LocationQueryResultPrivate())
{
    *d = *other.d;
}
LocationQueryResult::~LocationQueryResult() = default;
LocationQueryResult &
LocationQueryResult::operator=(const LocationQueryResult &other)
{
    *d = *other.d;
    return *this;
}
LocationQueryResult &
LocationQueryResult::operator=(LocationQueryResult &&other) = default;
double LocationQueryResult::latitude() const
{
    return d->latitude;
}
double LocationQueryResult::longitude() const
{
    return d->longitude;
}
const QString &LocationQueryResult::toponymName() const
{
    return d->toponymName;
}
const QString &LocationQueryResult::name() const
{
    return d->name;
}
const QString &LocationQueryResult::countryCode() const
{
    return d->countryCode;
}
const QString &LocationQueryResult::countryName() const
{
    return d->countryName;
}
const QString &LocationQueryResult::geonameId() const
{
    return d->geonameId;
}
}
