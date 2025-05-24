/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "pendingweatherforecast.h"
#include <kweathercore/kweathercore_export.h>

#include <QObject>

#include <memory>

class QNetworkAccessManager;

namespace KWeatherCore
{

class LocationQueryResult;
class WeatherForecastSourcePrivate;

/*!
 * \class KWeatherCore::WeatherForecastSource
 * \inheaderfile KWeatherCore/WeatherForecastSource
 * \inmodule KWeatherCore
 *
 * \brief The WeatherForecastSource class is intended for query weather
 * information about a location.
 *
 * \sa WeatherForecast, PendingWeatherForecast
 */
class KWEATHERCORE_EXPORT WeatherForecastSource : public QObject
{
    Q_OBJECT
public:
    /*!
     *
     */
    explicit WeatherForecastSource(QObject *parent = nullptr);
    ~WeatherForecastSource() override;

    /*!
     * It is the client's responsibility to delete the
     * PendingWeatherForecast afterhand to avoid memory leak.
     */
    PendingWeatherForecast *requestData(double latitude, double longitude);

    /*!
     * It is the client's responsibility to delete the
     * PendingWeatherForecast afterhand to avoid memory leak.
     */
    PendingWeatherForecast *requestData(const KWeatherCore::LocationQueryResult &result);

    /*!
     * Set the network access manager to use for network operations.
     *
     * If not set, an instance is created internally.
     *
     * Ownership is not transferred.
     */
    void setNetworkAccessManager(QNetworkAccessManager *nam);

private:
    std::unique_ptr<WeatherForecastSourcePrivate> d;
};
}
