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
class KWEATHERCORE_EXPORT PendingWeatherForecast : public QObject
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
    explicit PendingWeatherForecast(double latitude, double longitude, QNetworkReply *reply = nullptr, const QString &timezone = QString(), const QVector<Sunrise> &sunrise = QVector<Sunrise>());

private:
    PendingWeatherForecastPrivate *d = nullptr;
    bool m_finished = false;
};
}
