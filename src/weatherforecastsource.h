/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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
 * @short The WeatherForecastSource class is intened for query weather information about a location
 *
 * ## Usage example
 *
 * \snippet snippets/example.cpp example
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
    ~WeatherForecastSource();
    /**
     * @brief requestData
     * @param latitude
     * @param longitude
     * @param timezone
     * @param sunrise caching purpose
     * @return it is the client's responsibility to delete the PendingWeatherForecast afterhand to avoid memory leak
     */
    PendingWeatherForecast *requestData(double latitude, double longitude, QString timezone = QString(), const std::vector<Sunrise> &sunrise = std::vector<Sunrise>());

    /**
     * @brief requestData
     * @param result
     * @return it is the client's responsibility to delete the PendingWeatherForecast afterhand to avoid memory leak
     */
    PendingWeatherForecast *requestData(const KWeatherCore::LocationQueryResult &result);

private:
    WeatherForecastSourcePrivate *d = nullptr;
};
}
