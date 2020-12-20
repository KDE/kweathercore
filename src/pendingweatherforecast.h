/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once
#include "weatherforecast.h"
#include <QExplicitlySharedDataPointer>
#include <QObject>
#include <kweathercore/kweathercore_export.h>
class QNetworkReply;
namespace KWeatherCore
{
class PendingWeatherForecastPrivate;
/**
 * @short The PendingWeatherForecast class contains the reply to an asynchronous
 * weather forecast request.
 *
 * @see WeatherForecastSource
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT PendingWeatherForecast : public QObject
{
    Q_OBJECT
public:
    ~PendingWeatherForecast();
    /**
     * @brief value pointer to the shared weather data
     * the pointer is nullptr until finished() raised
     * @return
     */
    QExplicitlySharedDataPointer<WeatherForecast> value() const;
    /**
     * @brief isFinished if the call has finished
     * @return
     */
    bool isFinished();

Q_SIGNALS:
    /**
     * @brief signals the call has finished
     */
    void finished();
    /**
     * @brief indicate there is a network error
     */
    void networkError();

protected:
    friend class WeatherForecastSource;
    explicit PendingWeatherForecast(
        double latitude,
        double longitude,
        QNetworkReply *reply = nullptr,
        const QString &timezone = QString(),
        const std::vector<Sunrise> &sunrise = std::vector<Sunrise>());

private:
    PendingWeatherForecastPrivate *d = nullptr;
    bool m_finished = false;
};
}
