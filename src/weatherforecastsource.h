/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "locationqueryresult.h"
#include "pendingweatherforecast.h"
#include "weatherforecast.h"
#include <QObject>
#include <kweathercore/kweathercore_export.h>
class QNetworkReply;
namespace KWeatherCore
{
class WeatherForecastSourcePrivate;
/**
 * @short The WeatherForecastSource class is intened for query weather
 * information about a location
 *
 * @see WeatherForecast, PendingWeatherForecast
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT WeatherForecastSource : public QObject
{
    Q_OBJECT
public:
    WeatherForecastSource(QObject *parent = nullptr);
    /**
     * requestData
     * @param latitude
     * @param longitude
     * @param timezone
     * @param sunrise caching purpose
     * @return it is the client's responsibility to delete the
     * PendingWeatherForecast afterhand to avoid memory leak
     */
    std::pair<PendingWeatherForecast *, bool>
    requestData(double latitude,
                double longitude,
                QString timezone = QString(),
                const std::vector<Sunrise> &sunrise = std::vector<Sunrise>());

    /**
     * requestData
     * @param result
     * @return it is the client's responsibility to delete the
     * PendingWeatherForecast afterhand to avoid memory leak
     */
    std::pair<PendingWeatherForecast *, bool>
    requestData(const KWeatherCore::LocationQueryResult &result);

private:
    WeatherForecastSourcePrivate *d = nullptr;
};
}
