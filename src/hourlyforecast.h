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
     * @brief convert this to QJsonObject
     */
    QJsonObject toJson() const;
    /**
     * @brief construct from QJsonObject
     */
    static HourlyWeatherForecast fromJson(QJsonObject obj);
    /**
     * @brief date of the forecast
     * @return
     */
    const QDateTime &date() const
    {
        return m_date;
    };
    /**
     * @brief set date
     */
    void setDate(QDateTime date)
    {
        m_date = std::move(date);
    };
    /**
     * @brief weather description
     */
    const QString &weatherDescription() const
    {
        return m_weatherDescription;
    };
    /**
     * @brief set weather description
     */
    void setWeatherDescription(QString weatherDescription)
    {
        m_weatherDescription = std::move(weatherDescription);
    };
    /**
     * @brief weather icon, breeze icon if construct by WeatherForecastSource
     */
    const QString &weatherIcon() const
    {
        return m_weatherIcon;
    };
    /**
     * @brief set weather icon
     */
    void setWeatherIcon(QString weatherIcon)
    {
        m_weatherIcon = std::move(weatherIcon);
    };
    /**
     * @brief icon without "day" or "night" attached
     */
    const QString &neutralWeatherIcon() const
    {
        return m_neutralWeatherIcon;
    };
    /**
     * @brief set neutral weatherIcon
     */
    void setNeutralWeatherIcon(QString neutralWeatherIcon)
    {
        m_neutralWeatherIcon = std::move(neutralWeatherIcon);
    };
    /**
     * @brief internal symbolcode from api, normally you can ignore this
     */
    const QString &symbolCode() const
    {
        return m_symbolCode;
    };
    /**
     * @brief set internal symbolcode from api, normally you can ignore this
     */
    void setSymbolCode(QString symbolCode)
    {
        m_symbolCode = std::move(symbolCode);
    };
    /**
     * @brief temperature in celsius
     */
    double temperature() const
    {
        return m_temperature;
    };
    /**
     * @brief set temperature in celsius
     */
    void setTemperature(double temperature)
    {
        m_temperature = temperature;
    };
    /**
     * @brief pressure in hpa
     */
    double pressure() const
    {
        return m_pressure;
    };
    /**
     * @brief set pressure in hpa
     */
    void setPressure(double pressure)
    {
        m_pressure = pressure;
    };
    /**
     * @brief scoped enum
     */
    WindDirection windDirection() const
    {
        return m_windDirection;
    };
    /**
     * @brief set wind direction
     */
    void setWindDirection(WindDirection windDirection)
    {
        m_windDirection = windDirection;
    };
    /**
     * @brief wind speed in km/h
     */
    double windSpeed() const
    {
        return m_windSpeed;
    };
    /**
     * @brief set wind speed in km/h
     */
    void setWindSpeed(double windSpeed)
    {
        m_windSpeed = windSpeed;
    };
    /**
     * @brief humidity in percentage
     */
    double humidity() const
    {
        return m_humidity;
    };
    /**
     * @brief set humidity in percentage
     */
    void setHumidity(double humidity)
    {
        m_humidity = humidity;
    };
    /**
     * @brief fog in percentage
     */
    double fog() const
    {
        return m_fog;
    };
    /**
     * @brief set fog in percentage
     */
    void setFog(double fog)
    {
        m_fog = fog;
    };
    /**
     * @brief uv index, 0-1
     */
    double uvIndex() const
    {
        return m_uvIndex;
    };
    /**
     * @brief set uv index, 0-1
     */
    void setUvIndex(double uvIndex)
    {
        m_uvIndex = uvIndex;
    };
    /**
     * @brief precipitation in mm
     */
    double precipitationAmount() const
    {
        return m_precipitationAmount;
    };
    /**
     * @brief set precipitation in mm
     */
    void setPrecipitationAmount(double precipitationAmount)
    {
        m_precipitationAmount = precipitationAmount;
    };
    /**
     * @return true if date, weather icon and description is same
     */
    bool operator==(const KWeatherCore::HourlyWeatherForecast &) const;

private:
    QDateTime m_date;
    QString m_weatherDescription;
    QString m_weatherIcon;
    QString m_neutralWeatherIcon; // weather icon without time of day
    QString m_symbolCode;
    double m_temperature = 0; // celsius
    double m_pressure = 0; // hPa
    WindDirection m_windDirection = WindDirection::E;
    double m_windSpeed = 0; // m/s
    double m_humidity = 0; // %
    double m_fog = 0; // %
    double m_uvIndex = 0; // 0-1
    double m_precipitationAmount = 0; // mm
};
}
