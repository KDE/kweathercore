/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once
#include "dailyforecast.h"
#include "sunrise.h"
#include <QSharedData>
#include <kweathercore/kweathercore_export.h>
namespace KWeatherCore
{
class KWEATHERCORE_EXPORT WeatherForecast : public QSharedData
{
public:
    WeatherForecast();
    QJsonObject toJson() const;
    static QExplicitlySharedDataPointer<WeatherForecast> fromJson(QJsonObject obj);
    const QVector<DailyWeatherForecast> &dailyWeatherForecast() const;
    double latitude() const;
    double longitude() const;
    const QDateTime &createdTime() const;
    QVector<DailyWeatherForecast> &dailyWeatherForecast();
    const QString &timezone() const;
    void setCoordinate(double latitude, double longitude);
    void setTimezone(const QString &timezone);
    void setTimezone(QString &&timezone);
    void setDailyWeatherForecast(const QVector<DailyWeatherForecast> &forecast);
    void setDailyWeatherForecast(QVector<DailyWeatherForecast> &&forecast);
    void setSunriseForecast(const QVector<Sunrise> &sunrise);
    void setSunriseForecast(QVector<Sunrise> &&sunrise);

    WeatherForecast &operator+=(const DailyWeatherForecast &forecast);
    WeatherForecast &operator+=(DailyWeatherForecast &&forecast);
    WeatherForecast &operator+=(const HourlyWeatherForecast &forecast);
    WeatherForecast &operator+=(HourlyWeatherForecast &&forecast);

private:
    QVector<DailyWeatherForecast> m_dailyWeatherForecast;
    // QVector<Sunrise> m_sunriseForecast;
    QString m_timezone;
    double m_latitude, m_longitude;
    QDateTime m_createdTime;
};
}
