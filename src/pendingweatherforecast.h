/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "weatherforecast.h"
#include <kweathercore/kweathercore_export.h>

#include <QObject>

#include <memory>

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
class KWEATHERCORE_EXPORT PendingWeatherForecast : public QObject
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
     * @return
     */
    bool isFinished() const;

Q_SIGNALS:
    /**
     * signals the call has finished
     */
    void finished();
    /**
     * indicate there is a network error
     */
    void networkError();

protected:
    friend class WeatherForecastSource;
    explicit PendingWeatherForecast(double latitude,
                                    double longitude,
                                    const QString &timezone,
                                    const std::vector<Sunrise> &sunrise,
                                    QNetworkAccessManager *nam);
    explicit PendingWeatherForecast(WeatherForecast data);

private:
    std::unique_ptr<PendingWeatherForecastPrivate> d;
};
}
