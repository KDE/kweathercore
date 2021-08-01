/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "weatherforecast.h"
#include <QExplicitlySharedDataPointer>
#include <QObject>
#include <kweathercore/kweathercore_export.h>

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
    /**
     * value pointer to the shared weather data
     * the pointer is nullptr until finished() raised
     * @return
     */
    QExplicitlySharedDataPointer<WeatherForecast> value() const;
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
    friend class WeatherForecastSourcePrivate;
    explicit PendingWeatherForecast(double latitude,
                                    double longitude,
                                    const QUrl &url,
                                    const QString &timezone = QString(),
                                    const std::vector<Sunrise> &sunrise = std::vector<Sunrise>());
    explicit PendingWeatherForecast(QExplicitlySharedDataPointer<WeatherForecast> data);

private:
    PendingWeatherForecastPrivate *d = nullptr;
};
}
