/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "reply.h"
#include "weatherforecast.h"

class QNetworkAccessManager;

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
class KWEATHERCORE_EXPORT PendingWeatherForecast : public Reply
{
    Q_OBJECT
public:
    ~PendingWeatherForecast();

    /**
     * value pointer to the shared weather data
     * the pointer is nullptr until finished() raised
     * @return
     */
    WeatherForecast value() const;
    /**
     * isFinished if the call has finished
     * @deprecated No longer needed, finished() is emitted in all cases now.
     */
    [[deprecated("no longer needed")]] bool isFinished() const;

private:
    friend class WeatherForecastSource;
    explicit PendingWeatherForecast(double latitude,
                                    double longitude,
                                    const QString &timezone,
                                    QNetworkAccessManager *nam,
                                    QObject *parent = nullptr);
    explicit PendingWeatherForecast(WeatherForecast data, QObject *parent = nullptr);
    Q_DECLARE_PRIVATE(PendingWeatherForecast)
};
}
