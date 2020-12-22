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
class HourlyWeatherForecastPrivate;
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
    Q_GADGET
    Q_PROPERTY(QDateTime date READ date WRITE setDate)
    Q_PROPERTY(QString weatherDescription READ weatherDescription WRITE setWeatherDescription)
    Q_PROPERTY(QString weatherIcon READ weatherIcon WRITE setWeatherIcon)
    Q_PROPERTY(QString neutralWeatherIcon READ neutralWeatherIcon WRITE setNeutralWeatherIcon)
    Q_PROPERTY(QString windDirection READ windDirectionStr WRITE setWindDirectionStr)
    Q_PROPERTY(qreal temperature READ temperature WRITE setTemperature)
    Q_PROPERTY(qreal pressure READ pressure WRITE setPressure)
    Q_PROPERTY(qreal windSpeed READ windSpeed WRITE setWindSpeed)
    Q_PROPERTY(qreal humidity READ humidity WRITE setHumidity)
    Q_PROPERTY(qreal fog READ fog WRITE setFog)
    Q_PROPERTY(qreal uvIndex READ uvIndex WRITE setUvIndex)
    Q_PROPERTY(qreal precipitationAmount READ precipitationAmount WRITE setPrecipitationAmount)
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
    ~HourlyWeatherForecast();
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
    const QDateTime &date() const;
    /**
     * @brief set date
     */
    void setDate(QDateTime date);
    /**
     * @brief weather description
     */
    const QString &weatherDescription() const;
    /**
     * @brief set weather description
     */
    void setWeatherDescription(QString weatherDescription);
    /**
     * @brief weather icon, breeze icon if construct by WeatherForecastSource
     */
    const QString &weatherIcon() const;
    /**
     * @brief set weather icon
     */
    void setWeatherIcon(QString weatherIcon);
    /**
     * @brief icon without "day" or "night" attached
     */
    const QString &neutralWeatherIcon() const;
    /**
     * @brief set neutral weatherIcon
     */
    void setNeutralWeatherIcon(QString neutralWeatherIcon);
    /**
     * @brief internal symbolcode from api, normally you can ignore this
     */
    const QString &symbolCode() const;
    /**
     * @brief set internal symbolcode from api, normally you can ignore this
     */
    void setSymbolCode(QString symbolCode);
    /**
     * @brief temperature in celsius
     */
    double temperature() const;
    /**
     * @brief set temperature in celsius
     */
    void setTemperature(double temperature);
    /**
     * @brief pressure in hpa
     */
    double pressure() const;
    /**
     * @brief set pressure in hpa
     */
    void setPressure(double pressure);
    /**
     * @brief scoped enum
     */
    WindDirection windDirection() const;
    KWEATHERCORE_NO_EXPORT QString windDirectionStr() const;
    /**
     * @brief set wind direction
     */
    void setWindDirection(WindDirection windDirection);
    /**
     * @brief wind speed in km/h
     */
    KWEATHERCORE_NO_EXPORT void setWindDirectionStr(QString windDirection);
    double windSpeed() const;
    /**
     * @brief set wind speed in km/h
     */
    void setWindSpeed(double windSpeed);
    /**
     * @brief humidity in percentage
     */
    double humidity() const;
    /**
     * @brief set humidity in percentage
     */
    void setHumidity(double humidity);
    /**
     * @brief fog in percentage
     */
    double fog() const;
    /**
     * @brief set fog in percentage
     */
    void setFog(double fog);
    /**
     * @brief uv index, 0-1
     */
    double uvIndex() const;
    /**
     * @brief set uv index, 0-1
     */
    void setUvIndex(double uvIndex);
    /**
     * @brief precipitation in mm
     */
    double precipitationAmount() const;
    /**
     * @brief set precipitation in mm
     */
    void setPrecipitationAmount(double precipitationAmount);
    /**
     * @return true if date, weather icon and description is same
     */
    bool operator==(const KWeatherCore::HourlyWeatherForecast &) const;

private:
    HourlyWeatherForecastPrivate *d;
};
}
