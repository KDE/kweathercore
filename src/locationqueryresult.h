/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include <QString>
namespace KWeatherCore
{
class LocationQueryResult
{
public:
    LocationQueryResult(double latitude, double longitude, QString toponymName = QString(), QString name = QString(), QString countryCode = QString(), QString countryName = QString(), QString geonameId = QString());
    double latitude() const;
    double longitude() const;
    QString &toponymName() const;
    QString &name() const;
    QString &countryCode() const;
    QString &countryName() const;
    QString &geonameId() const;

private:
    double m_latitude, m_longitude;
    QString m_toponymName, m_name, m_countryCode, m_countryName, m_geonameId;
};
}
