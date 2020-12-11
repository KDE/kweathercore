/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include <QMetaType>
#include <QString>
#include <kweathercore/kweathercore_export.h>
namespace KWeatherCore
{
/**
 * @short Class represents location query result
 *
 * This is a class to hold general information about a location
 *
 * @see LocationQuery
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT LocationQueryResult
{
public:
    // for QMetaType
    LocationQueryResult() {};
    /**
     * @brief LocationQueryResult construct location result with given data
     * @param latitude latitude
     * @param longitude longitude
     * @param toponymName toponym name of location, detailed
     * @param name display name, short
     * @param countryCode country code, follow no standard but should be unique
     * @param countryName country name
     * @param geonameId internal unique id
     */
    LocationQueryResult(double latitude, double longitude, QString toponymName = QString(), QString name = QString(), QString countryCode = QString(), QString countryName = QString(), QString geonameId = QString());
    /**
     * @brief latitude latitude
     * @return
     */
    double latitude() const;
    /**
     * @brief longitude longitude
     * @return
     */
    double longitude() const;
    /**
     * @brief toponymName toponym name of location, detailed
     * @return
     */
    const QString &toponymName() const;
    /**
     * @brief name display name, short
     * @return
     */
    const QString &name() const;
    /**
     * @brief countryCode country code, follow no standard but should be unique
     * @return
     */
    const QString &countryCode() const;
    /**
     * @brief countryName country name
     * @return
     */
    const QString &countryName() const;
    /**
     * @brief geonameId internal unique id
     * @return
     */
    const QString &geonameId() const;

private:
    double m_latitude, m_longitude;
    QString m_toponymName, m_name, m_countryCode, m_countryName, m_geonameId;
};
}
Q_DECLARE_METATYPE(KWeatherCore::LocationQueryResult)
