/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "locationqueryresult.h"
#include <QGeoPositionInfoSource>
#include <QObject>
#include <kweathercore/kweathercore_export.h>
class QNetworkReply;
namespace KWeatherCore
{
class LocationQueryPrivate;
/**
 * @short Class locates current location and search locations by name
 *
 * This is a class that locates current location (GPS or IP) and search locations by name
 *
 * @see LocationQuery
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT LocationQuery : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief LocationQuery
     * @param parent
     */
    LocationQuery(QObject *parent = nullptr);
    ~LocationQuery();
    /**
     * @brief query query locations by name
     * @param name name of location, not necessary in English
     * @param number max numbers of query returned, the actual size could be less than @param number
     */
    void query(QString name, int number = 30);
    /**
     * @brief locate locate current location
     */
    void locate();
Q_SIGNALS:
    /**
     * @brief queryFinished the name search has completed
     * @param result
     */
    void queryFinished(std::vector<LocationQueryResult> result);
    /**
     * @brief located current location has been determined
     * @param result
     */
    void located(LocationQueryResult result);
    /**
     * @brief queryError a error has encounted during query, network error or no result found
     */
    void queryError();
private Q_SLOTS:
    void handleQueryResult(QNetworkReply *reply);
    void positionUpdated(const QGeoPositionInfo &update);

private:
    LocationQueryPrivate *d = nullptr;
};
}
