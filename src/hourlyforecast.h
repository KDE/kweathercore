/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include <QDateTime>
#include <QObject>
namespace KWeatherCore
{
enum class WindDirection { N, NW, W, SW, S, SE, E, NE };

class HourlyWeatherForecast
{
public:
    HourlyWeatherForecast();
    HourlyWeatherForecast(QDateTime date,
                          QString weatherDescription,
                          QString weatherIcon,
                          QString neutralWeatherIcon,
                          float temperature,
                          float pressure,
                          WindDirection windDirection,
                          float windSpeed,
                          float humidity,
                          float fog,
                          float uvIndex,
                          float precipitationAmount);

    QJsonObject toJson();
    static HourlyWeatherForecast fromJson(QJsonObject obj);

    QDateTime &date() const;
    void setDate(QDateTime date);
    QString weatherDescription() const;
    void setWeatherDescription(const QString &weatherDescription);
    QString weatherIcon() const;
    void setWeatherIcon(const QString &weatherIcon);
    QString neutralWeatherIcon() const;
    void setNeutralWeatherIcon(QString neutralWeatherIcon);
    QString symbolCode() const;
    void setSymbolCode(QString symbolCode);
    float temperature() const;
    void setTemperature(float temperature);
    float pressure() const;
    void setPressure(float pressure);
    WindDirection windDirection() const;
    void setWindDirection(WindDirection windDirection);
    float windSpeed() const;
    void setWindSpeed(float windSpeed);
    float humidity() const;
    void setHumidity(float humidity);
    float fog() const;
    void setFog(float fog);
    float uvIndex() const;
    void setUvIndex(float uvIndex);
    float precipitationAmount() const;
    void setPrecipitationAmount(float precipitationAmount);

private:
    QDateTime m_date;
    QString m_weatherDescription;
    QString m_weatherIcon;
    QString m_neutralWeatherIcon; // weather icon without time of day
    QString m_symbolCode;
    float m_temperature = 0; // celsius
    float m_pressure = 0;    // hPa
    WindDirection m_windDirection = WindDirection::E;
    float m_windSpeed = 0;           // m/s
    float m_humidity = 0;            // %
    float m_fog = 0;                 // %
    float m_uvIndex = 0;             // 0-1
    float m_precipitationAmount = 0; // mm
};
}
