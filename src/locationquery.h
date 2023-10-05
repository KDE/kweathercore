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
#include <memory>

class QNetworkAccessManager;

namespace KWeatherCore
{
class LocationQueryReply;
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
 * auto reply = m_locationSource.query("Oslo")
 * connect(reply, &LocationQueryReply::finished, []()
 *          {
 *              reply->deleteLater();
 *              if (reply != LocationQueryReply::NoError) {
 *                  qDebug() << "can't find this place";
 *                  return;
 *              }
 *              for(auto location : reply->result())
 *              {
 *                  qDebug() << location.toponymName();
 *              }
 *          });
 *
 * auto reply = m_locationSource.locate();
 * connect(reply, &LocationQuery::finsihed, []() {
 *    reply->deleteLater();
 *    if (reply->error() == LocationQueryReply::NoError)
 *      qDebug() << "your coordinate: " << reply->result.first().latitude() << ", " << reply->result().first().replylongitude();
 * }
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
    explicit LocationQuery(QObject *parent = nullptr);
    ~LocationQuery() override;
    /**
     * query query locations by name
     * @param name name of location, not necessary in English
     * @param number max numbers of query returned, the actual size could be
     * less than @param number
     */
    LocationQueryReply *query(const QString &name, int number = 30);
    /**
     * locate current location
     */
    LocationQueryReply *locate();

    /** Set the network access manager to use for network operations.
     *  If not set, an instance is created internally.
     *  Ownership is not transferred.
     */
    void setNetworkAccessManager(QNetworkAccessManager *nam);

private:
    std::unique_ptr<LocationQueryPrivate> d;
};
}
