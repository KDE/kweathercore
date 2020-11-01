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
class WeatherForecastSource : public QObject
{
    Q_OBJECT
public:
    WeatherForecastSource(QObject *parent = nullptr);
    ~WeatherForecastSource();
    template<class T> PendingWeatherForecast *requestData(double latitude, double longitude, QString timezone = QString(), T &&sunrise = QVector<Sunrise>());
    template<class T> PendingWeatherForecast *requestData(const LocationQueryResult &location, QString timezone = QString(), T &&sunrise = QVector<Sunrise>());
Q_SIGNALS:
    void networkError();

private:
    WeatherForecastSourcePrivate *d = nullptr;
};
}
