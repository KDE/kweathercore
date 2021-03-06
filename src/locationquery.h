/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
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
 * This is a class that locates current location (GPS or IP) and search
 * locations by name
 * ## Usage example
 *
 * Example usage:
 * @code
 * #include <KWeatherCore/LocationQuery>
 * using namespace KWeatherCore;
 *
 *   //...
 *
 * LocationQuery m_locationSource;
 *
 * // find places called "Oslo"
 * m_locationSource.query("Oslo")
 * connect(m_locationSource, &LocationQuery::queryFinished,
 *      [](const std::vector<LocationQueryResult> &result)
 *          {
 *              for(auto location : result)
 *              {
 *                  qDebug() << location.toponymName();
 *              }
 *          });
 * connect(m_locationSource, &LocationQuery::queryError,
 *      []{qDebug() << "can't find this place";});
 *
 * m_locationSource.locate();
 * connect(m_locationSource, &LocationQuery::located,
 *      [](const LocationQueryResult &result){qDebug() << "your coordinate: " <<
 * result.latitude() << ", " << longitude();}
 * //...
 * @endcode
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT LocationQuery : public QObject
{
    Q_OBJECT
public:
    /**
     * LocationQuery
     */
    LocationQuery(QObject *parent = nullptr);
    /**
     * query query locations by name
     * @param name name of location, not necessary in English
     * @param number max numbers of query returned, the actual size could be
     * less than @param number
     */
    void query(QString name, int number = 30);
    /**
     * locate current location
     */
    void locate();
Q_SIGNALS:
    /**
     * the name search has completed
     */
    void queryFinished(std::vector<LocationQueryResult> result);
    /**
     * current location has been determined
     */
    void located(const LocationQueryResult &result);
    /**
     * a error has encounted during query, network error or no result
     * found
     */
    void queryError();

private:
    LocationQueryPrivate *d = nullptr;
};
}
