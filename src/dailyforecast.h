/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once
#include "hourlyforecast.h"
#include "sunrise.h"
#include <QDate>
#include <QJsonObject>
#include <kweathercore/kweathercore_export.h>
namespace KWeatherCore
{
/**
 * @short Class represents weatherforecast in a day
 *
 * This is a class to hold general weather conditions, hourly forecast and sunrise in a day
 * All QDate/DateTime are on the location's timezone
 * @see HourlyForecast, Sunrise
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT DailyWeatherForecast
{
public:
    /**
     * Creates a null DailyWeatherForecast.
     */
    DailyWeatherForecast();
    /**
     * Creates a DailyWeatherForecast with given params.
     * @param maxTemp maximum temperature of the day, in celsius
     * @param minTemp minimum temperature of the day, in celsius
     * @param precipitation precipitation of the day, in mm
     * @param uvIndex uvIndex of the day, 0-1
     * @param humidity humidity of the day, in percentage
     * @param pressure pressure of the day, in hpa
     * @param weatherIcon icon that presents the weather condition of the day.
     * @param weatherDescription text describes the general weather condition of the day
     * @param date the date this object represents
     */
    DailyWeatherForecast(double maxTemp, double minTemp, double precipitation, double uvIndex, double humidity, double pressure, QString weatherIcon, QString weatherDescription, QDate date);
    /**
     * Return a QJsonObject that can be converted back with DailyWeatherForecast::fromJson
     */
    QJsonObject toJson();
    /**
     * Construct a DailyWeatherForecast from QJsonObject
     */
    static DailyWeatherForecast fromJson(QJsonObject obj);
    /**
     * @return @c true if the object is created without data
     * this value won't change once the class is created
     */
    bool isNull() const
    {
        return m_isNull;
    };
    /**
     * @brief set the maximum temperature of the day
     * @param maxTemp maximum temperature of the day, in celsius
     */
    void setMaxTemp(double maxTemp)
    {
        m_maxTemp = maxTemp;
    };
    /**
     * @brief set the minimum temperature of the day
     * @param minTemp minimum temperature of the day, in celsius
     */
    void setMinTemp(double minTemp)
    {
        m_minTemp = minTemp;
    };
    /**
     * @brief set the precipitation of the day
     * @param precipitation precipitation of the day, in mm
     */
    void setPrecipitation(double precipitation)
    {
        m_precipitation = precipitation;
    };
    /**
     * @brief set the UvIndex of the day
     * @param uvIndex 0-1
     */
    void setUvIndex(double uvIndex)
    {
        m_uvIndex = uvIndex;
    };
    /**
     * @brief set the humidity of the day
     * @param humidity humidity of the day, in percentage
     */
    void setHumidity(double humidity)
    {
        m_humidity = humidity;
    };
    /**
     * @brief set the pressure of the day
     * @param pressure pressure of the day, in hpa
     */
    void setPressure(double pressure)
    {
        m_pressure = pressure;
    };
    /**
     * @brief set the weather icon of the day
     * @param icon
     */
    void setWeatherIcon(QString icon)
    {
        m_weatherIcon = std::move(icon);
    };
    /**
     * @brief set the weather description of the day
     * @param description
     */
    void setWeatherDescription(QString description)
    {
        m_weatherDescription = std::move(description);
    };
    /**
     * @brief set the date this object represents
     * @param date
     */
    void setDate(QDate date)
    {
        m_date = std::move(date);
    };
    /**
     * @brief return maximum temperature
     * @return maximum temperature, this value is initialized to the smallest value double can hold
     */
    double maxTemp() const
    {
        return m_maxTemp;
    };
    /**
     * @brief return minimum temperature
     * @return minimum temperature, this value is initialized to the largest value double can hold
     */
    double minTemp() const
    {
        return m_minTemp;
    };
    /**
     * @brief return precipitation
     * @return this value is initialized to zero
     */
    double precipitation() const
    {
        return m_precipitation;
    };
    /**
     * @brief return uvIndex
     * @return this value is initialized to zero
     */
    double uvIndex() const
    {
        return m_uvIndex;
    };
    /**
     * @brief return humidity
     * @return this value is initialized to zero
     */
    double humidity() const
    {
        return m_humidity;
    };
    /**
     * @brief return pressure
     * @return this value is initialized to zero
     */
    double pressure() const
    {
        return m_pressure;
    };
    /**
     * @brief return weather icon
     * @return weather icon, can be empty string if constructed without data
     */
    const QString &weatherIcon() const
    {
        return m_weatherIcon;
    };
    /**
     * @brief return weather description
     * @return weather description, can be empty string if constructed without data
     */
    const QString &weatherDescription() const
    {
        return m_weatherDescription;
    };
    /**
     * @brief return date this object represents
     * @return date, date can be invalid if constructed without data
     */
    const QDate &date() const
    {
        return m_date;
    };

    /**
     * @brief return sunrise data
     * @return sunrise data
     */
    const Sunrise &sunrise() const
    {
        return m_sunrise;
    };
    /**
     * @brief returns all HourlyWeathreForecast belonged to this day
     * @return all HourlyWeathreForecast belonged to this day
     */
    const std::vector<HourlyWeatherForecast> &hourlyWeatherForecast() const
    {
        return m_hourlyWeatherForecast;
    };
    /**
     * @brief overloaded version
     * @return reference to internal std::vector
     */
    std::vector<HourlyWeatherForecast> &hourlyWeatherForecast()
    {
        return m_hourlyWeatherForecast;
    };
    /**
     * @brief set sunrise
     * @param sunrise if this object and sunrise isn't on the same day, this function does nothing
     */
    void setSunrise(Sunrise sunrise)
    {
        m_sunrise = std::move(sunrise);
    };
    /**
     * @brief set the hourly forecast of the day
     * @param forecast make sure they are sorted and on the same day
     */
    void setHourlyWeatherForecast(const std::vector<HourlyWeatherForecast> &forecast)
    {
        m_hourlyWeatherForecast = forecast;
    };
    /**
     * @brief overloaded version
     * @param forecast
     */
    void setHourlyWeatherForecast(std::vector<HourlyWeatherForecast> &&forecast)
    {
        m_hourlyWeatherForecast = std::move(forecast);
    };

    /**
     * @brief merge two daily forecast
     * @param forecast make sure it is on the same day
     * @return result DailyWeatherForecast
     */
    DailyWeatherForecast &operator+(const DailyWeatherForecast &forecast);
    /**
     * @brief merge two daily forecast
     * @param forecast make sure it is on the same day
     * @return result DailyWeatherForecast
     */
    DailyWeatherForecast &operator+=(const DailyWeatherForecast &forecast);
    /**
     * @brief append hourly forecast, you can append valid hourly forecast into a null daily forecast
     * @param forecast make sure it's on the same day
     * @return result DailyWeatherForecast
     */
    DailyWeatherForecast &operator+=(const HourlyWeatherForecast &forecast);
    /**
     * @brief if on the same day and weather description/icon match
     * @param forecast
     * @return @c true if on the same day and weather description/icon match
     */
    bool operator==(const DailyWeatherForecast &forecast) const;
    /**
     * @brief if this is earlier than \param forecast
     * @param forecast
     * @return @c true if this is earlier than \param forecast
     */
    bool operator<(const DailyWeatherForecast &forecast) const;

private:
    bool m_isNull = false;

    double m_maxTemp = std::numeric_limits<double>::min();
    double m_minTemp = std::numeric_limits<double>::max();
    double m_precipitation = 0; // mm
    double m_uvIndex = 0;       // 0-1
    double m_humidity = 0;      // %
    double m_pressure = 0;      // hPa
    QString m_weatherIcon;
    QString m_weatherDescription;
    QDate m_date;

    Sunrise m_sunrise;
    std::vector<HourlyWeatherForecast> m_hourlyWeatherForecast;
};
}
