/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include <QDateTime>
#include <QObject>
#include <kweathercore/kweathercore_export.h>
namespace KWeatherCore
{
enum class WindDirection { N, NW, W, SW, S, SE, E, NE };

class KWEATHERCORE_EXPORT HourlyWeatherForecast
{
public:
    HourlyWeatherForecast();
    HourlyWeatherForecast(QDateTime date,
                          QString weatherDescription,
                          QString weatherIcon,
                          QString neutralWeatherIcon,
                          double temperature,
                          double pressure,
                          WindDirection windDirection,
                          double windSpeed,
                          double humidity,
                          double fog,
                          double uvIndex,
                          double precipitationAmount);

    QJsonObject toJson();
    static HourlyWeatherForecast fromJson(QJsonObject obj);

    QDateTime date() const;
    void setDate(QDateTime date);
    QString weatherDescription() const;
    void setWeatherDescription(const QString &weatherDescription);
    QString weatherIcon() const;
    void setWeatherIcon(const QString &weatherIcon);
    QString neutralWeatherIcon() const;
    void setNeutralWeatherIcon(QString neutralWeatherIcon);
    QString symbolCode() const;
    void setSymbolCode(QString symbolCode);
    double temperature() const;
    void setTemperature(double temperature);
    double pressure() const;
    void setPressure(double pressure);
    WindDirection windDirection() const;
    void setWindDirection(WindDirection windDirection);
    double windSpeed() const;
    void setWindSpeed(double windSpeed);
    double humidity() const;
    void setHumidity(double humidity);
    double fog() const;
    void setFog(double fog);
    double uvIndex() const;
    void setUvIndex(double uvIndex);
    double precipitationAmount() const;
    void setPrecipitationAmount(double precipitationAmount);

private:
    QDateTime m_date;
    QString m_weatherDescription;
    QString m_weatherIcon;
    QString m_neutralWeatherIcon; // weather icon without time of day
    QString m_symbolCode;
    double m_temperature = 0; // celsius
    double m_pressure = 0;    // hPa
    WindDirection m_windDirection = WindDirection::E;
    double m_windSpeed = 0;           // m/s
    double m_humidity = 0;            // %
    double m_fog = 0;                 // %
    double m_uvIndex = 0;             // 0-1
    double m_precipitationAmount = 0; // mm
};
}
