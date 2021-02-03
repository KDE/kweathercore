/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <QMetaType>
#include <QString>
#include <kweathercore/kweathercore_export.h>
#include <memory>
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
    Q_GADGET
    Q_PROPERTY(qreal latitude READ latitude)
    Q_PROPERTY(qreal longitude READ longitude)
    Q_PROPERTY(QString toponymName READ toponymName)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString countryCode READ countryCode)
    Q_PROPERTY(QString countryName READ countryName)
    Q_PROPERTY(QString geonameId READ geonameId)
public:
    // for QMetaType
    LocationQueryResult();
    /**
     * LocationQueryResult construct location result with given data
     * @param latitude latitude
     * @param longitude longitude
     * @param toponymName toponym name of location, detailed
     * @param name display name, short
     * @param countryCode country code, follow no standard but should be unique
     * @param countryName country name
     * @param geonameId internal unique id
     */
    LocationQueryResult(double latitude,
                        double longitude,
                        QString toponymName = QString(),
                        QString name = QString(),
                        QString countryCode = QString(),
                        QString countryName = QString(),
                        QString geonameId = QString());
    LocationQueryResult(const LocationQueryResult &other);
    LocationQueryResult(LocationQueryResult &&other);
    ~LocationQueryResult();
    LocationQueryResult &operator=(const LocationQueryResult &other);
    LocationQueryResult &operator=(LocationQueryResult &&other);
    double latitude() const;

    double longitude() const;
    /**
     * toponym name of location, detailed
     */
    const QString &toponymName() const;
    /**
     * display name, short
     */
    const QString &name() const;
    /**
     * country code, follow no standard but should be unique
     */
    const QString &countryCode() const;
    /**
     * country name
     */
    const QString &countryName() const;
    /**
     * internal unique id
     */
    const QString &geonameId() const;

private:
    class LocationQueryResultPrivate;
    std::unique_ptr<LocationQueryResultPrivate> d;
};
}
Q_DECLARE_METATYPE(KWeatherCore::LocationQueryResult)
