/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include <QObject>
namespace LibKWeather
{
class LocationQueryResult : public QObject
{
    Q_OBJECT

public:
    explicit LocationQueryResult()
    {
    }
    explicit LocationQueryResult(double latitude, double longitude, QString toponymName, QString name, QString countryCode, QString countryName, QString geonameId);
    double latitude() const;
    double longitude() const;
    QString toponymName() const;
    QString name() const;
    QString countryCode() const;
    QString countryName() const;
    QString geonameId() const;

private:
    double m_latitude, m_longitude;
    QString m_toponymName, m_name, m_countryCode, m_countryName, m_geonameId;
};
}
