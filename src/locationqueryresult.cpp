/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "locationqueryresult.h"

#include <KCountry>

#include <memory>
#include <optional>

namespace KWeatherCore
{
class LocationQueryResult::LocationQueryResultPrivate
{
public:
    double latitude, longitude;
    QString toponymName, name, countryCode, geonameId;
    std::optional<QString> subdivision;
};
LocationQueryResult::LocationQueryResult()
    : d(std::make_unique<LocationQueryResultPrivate>())
{
}
LocationQueryResult::LocationQueryResult(LocationQueryResult &&other) noexcept = default;
LocationQueryResult::LocationQueryResult(double latitude,
                                         double longitude,
                                         QString toponymName,
                                         QString name,
                                         QString countryCode,
                                         QString geonameId,
                                         std::optional<QString> subdivision)
    : d(std::make_unique<LocationQueryResultPrivate>())
{
    d->latitude = latitude;
    d->longitude = longitude;
    d->toponymName = std::move(toponymName);
    d->name = std::move(name);
    d->countryCode = std::move(countryCode);
    d->geonameId = std::move(geonameId);
    d->subdivision = std::move(subdivision);
}
LocationQueryResult::LocationQueryResult(const LocationQueryResult &other)
    : d(std::make_unique<LocationQueryResultPrivate>())
{
    *d = *other.d;
}
LocationQueryResult::~LocationQueryResult() = default;
LocationQueryResult &LocationQueryResult::operator=(const LocationQueryResult &other)
{
    *d = *other.d;
    return *this;
}
LocationQueryResult &LocationQueryResult::operator=(LocationQueryResult &&other) noexcept = default;
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
QString LocationQueryResult::countryName() const
{
    return KCountry::fromAlpha2(d->countryCode).name();
}
const QString &LocationQueryResult::geonameId() const
{
    return d->geonameId;
}
const std::optional<QString> &LocationQueryResult::subdivision() const
{
    return d->subdivision;
}
}

#include "moc_locationqueryresult.cpp"
