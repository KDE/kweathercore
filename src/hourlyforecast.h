/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include <QDateTime>
#include <QJsonObject>
#include <QObject>
#include <kweathercore/kweathercore_export.h>
namespace KWeatherCore
{
enum class WindDirection { N, NW, W, SW, S, SE, E, NE };
/**
 * @short Class represents weatherforecast in a hour
 *
 * This is a class to hold hourly forecast
 *
 * @see DailyWeatherForecast
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT HourlyWeatherForecast
{
public:
    /**
     * @brief HourlyWeatherForecast construct a null forecast
     */
    HourlyWeatherForecast();
    /**
     * @brief HourlyWeatherForecast construct a forecast with given data
     * @param date start time of the forecast
     * @param weatherDescription description of weather
     * @param weatherIcon icon of the weather
     * @param neutralWeatherIcon icon without "day" or "night" attached
     * @param temperature temperature, in celsius
     * @param pressure pressure, in hpa
     * @param windDirection scoped enum
     * @param windSpeed wind speed, in km/h
     * @param humidity humidity,in percentage
     * @param fog fog, in percentage
     * @param uvIndex uv index, 0-1
     * @param precipitationAmount precipitation, in mm
     */
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
    /**
     * @brief toJson convert this to QJsonObject
     * @return
     */
    QJsonObject toJson() const;
    /**
     * @brief fromJson construct from QJsonObject
     * @param obj
     * @return
     */
    static HourlyWeatherForecast fromJson(QJsonObject obj);
    /**
     * @brief date date of the forecast
     * @return
     */
    const QDateTime &date() const;
    /**
     * @brief setDate set date
     * @param date
     */
    void setDate(QDateTime date);
    /**
     * @brief weatherDescription weather description
     * @return
     */
    const QString &weatherDescription() const;
    /**
     * @brief setWeatherDescription set weather description
     * @param weatherDescription
     */
    void setWeatherDescription(const QString &weatherDescription);
    /**
     * @brief weatherIcon weather icon, breeze icon if construct by WeatherForecastSource
     * @return
     */
    const QString &weatherIcon() const;
    /**
     * @brief setWeatherIcon set weather icon
     * @param weatherIcon
     */
    void setWeatherIcon(const QString &weatherIcon);
    /**
     * @brief neutralWeatherIcon icon without "day" or "night" attached
     * @return
     */
    const QString &neutralWeatherIcon() const;
    /**
     * @brief setNeutralWeatherIcon set neutral weatherIcon
     * @param neutralWeatherIcon
     */
    void setNeutralWeatherIcon(QString neutralWeatherIcon);
    /**
     * @brief symbolCode internal symbolcode from api, normally you can ignore this
     * @return
     */
    const QString &symbolCode() const;
    /**
     * @brief setSymbolCode set internal symbolcode from api, normally you can ignore this
     * @param symbolCode
     */
    void setSymbolCode(QString symbolCode);
    /**
     * @brief temperature temperature in celsius
     * @return
     */
    double temperature() const;
    /**
     * @brief setTemperature set temperature in celsius
     * @param temperature
     */
    void setTemperature(double temperature);
    /**
     * @brief pressure pressure in hpa
     * @return
     */
    double pressure() const;
    /**
     * @brief setPressure set pressure in hpa
     * @param pressure
     */
    void setPressure(double pressure);
    /**
     * @brief windDirection scoped enum
     * @return
     */
    WindDirection windDirection() const;
    /**
     * @brief setWindDirection set wind direction
     * @param windDirection
     */
    void setWindDirection(WindDirection windDirection);
    /**
     * @brief windSpeed wind speed in km/h
     * @return
     */
    double windSpeed() const;
    /**
     * @brief setWindSpeed set wind speed in km/h
     * @param windSpeed
     */
    void setWindSpeed(double windSpeed);
    /**
     * @brief humidity humidity in percentage
     * @return
     */
    double humidity() const;
    /**
     * @brief setHumidity set humidity in percentage
     * @param humidity
     */
    void setHumidity(double humidity);
    /**
     * @brief fog fog in percentage
     * @return
     */
    double fog() const;
    /**
     * @brief setFog set fog in percentage
     * @param fog
     */
    void setFog(double fog);
    /**
     * @brief uvIndex uv index, 0-1
     * @return
     */
    double uvIndex() const;
    /**
     * @brief setUvIndex set uv index, 0-1
     * @param uvIndex
     */
    void setUvIndex(double uvIndex);
    /**
     * @brief precipitationAmount precipitation in mm
     * @return
     */
    double precipitationAmount() const;
    /**
     * @brief setPrecipitationAmount set precipitation in mm
     * @param precipitationAmount
     */
    void setPrecipitationAmount(double precipitationAmount);

    bool operator==(const KWeatherCore::HourlyWeatherForecast &) const;

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
