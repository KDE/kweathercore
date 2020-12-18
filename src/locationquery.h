/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "locationqueryresult.h"
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
 * ## Usage example
 *
 * \snippet snippets/locationquery.cpp example
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT LocationQuery : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief LocationQuery
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
     * @brief locate current location
     */
    void locate();
Q_SIGNALS:
    /**
     * @brief the name search has completed
     */
    void queryFinished(const std::vector<LocationQueryResult> &result);
    /**
     * @brief current location has been determined
     */
    void located(const LocationQueryResult &result);
    /**
     * @brief a error has encounted during query, network error or no result found
     */
    void queryError();
private Q_SLOTS:
    void handleQueryResult(QNetworkReply *reply);

private:
    LocationQueryPrivate *d = nullptr;
};
}
