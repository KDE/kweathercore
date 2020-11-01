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
class PendingWeatherForecast : public QObject
{
    Q_OBJECT
public:
    ~PendingWeatherForecast();
    QExplicitlySharedDataPointer<WeatherForecast> value() const;
    bool isFinished();

Q_SIGNALS:
    void finished();
    void networkError();

protected:
    friend class WeatherForecastSource;
    explicit PendingWeatherForecast(PendingWeatherForecastPrivate *dd);

private:
    PendingWeatherForecastPrivate *d = nullptr;
    bool m_finished = false;
};
}
