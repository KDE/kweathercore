/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
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
    Q_PROPERTY(QString weatherDescription READ weatherDescription WRITE
                   setWeatherDescription)
    Q_PROPERTY(QString weatherIcon READ weatherIcon WRITE setWeatherIcon)
    Q_PROPERTY(QString neutralWeatherIcon READ neutralWeatherIcon WRITE
                   setNeutralWeatherIcon)
    Q_PROPERTY(
        QString windDirection READ windDirectionStr WRITE setWindDirectionStr)
    Q_PROPERTY(qreal temperature READ temperature WRITE setTemperature)
    Q_PROPERTY(qreal pressure READ pressure WRITE setPressure)
    Q_PROPERTY(qreal windSpeed READ windSpeed WRITE setWindSpeed)
    Q_PROPERTY(qreal humidity READ humidity WRITE setHumidity)
    Q_PROPERTY(qreal fog READ fog WRITE setFog)
    Q_PROPERTY(qreal uvIndex READ uvIndex WRITE setUvIndex)
    Q_PROPERTY(qreal precipitationAmount READ precipitationAmount WRITE
                   setPrecipitationAmount)
public:
    /**
     * HourlyWeatherForecast construct a null forecast
     */
    HourlyWeatherForecast();
    /**
     * HourlyWeatherForecast construct a forecast with given data
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
     * convert this to QJsonObject
     */
    QJsonObject toJson() const;
    /**
     * construct from QJsonObject
     */
    static HourlyWeatherForecast fromJson(QJsonObject obj);
    /**
     * date of the forecast
     * @return
     */
    const QDateTime &date() const;
    /**
     * set date
     */
    void setDate(const QDateTime &date);
    /**
     * weather description
     */
    const QString &weatherDescription() const;
    /**
     * set weather description
     */
    void setWeatherDescription(const QString &weatherDescription);
    /**
     * weather icon, breeze icon if construct by WeatherForecastSource
     */
    const QString &weatherIcon() const;
    /**
     * set weather icon
     */
    void setWeatherIcon(const QString &weatherIcon);
    /**
     * icon without "day" or "night" attached
     */
    const QString &neutralWeatherIcon() const;
    /**
     * set neutral weatherIcon
     */
    void setNeutralWeatherIcon(const QString &neutralWeatherIcon);
    /**
     * internal symbolcode from api, normally you can ignore this
     */
    const QString &symbolCode() const;
    /**
     * set internal symbolcode from api, normally you can ignore this
     */
    void setSymbolCode(const QString &symbolCode);
    /**
     * temperature in celsius
     */
    double temperature() const;
    /**
     * set temperature in celsius
     */
    void setTemperature(double temperature);
    /**
     * pressure in hpa
     */
    double pressure() const;
    /**
     * set pressure in hpa
     */
    void setPressure(double pressure);
    /**
     * scoped enum
     */
    WindDirection windDirection() const;
    KWEATHERCORE_NO_EXPORT QString windDirectionStr() const;
    /**
     * set wind direction
     */
    void setWindDirection(WindDirection windDirection);
    /**
     * wind speed in km/h
     */
    KWEATHERCORE_NO_EXPORT void
    setWindDirectionStr(const QString &windDirection);
    double windSpeed() const;
    /**
     * set wind speed in km/h
     */
    void setWindSpeed(double windSpeed);
    /**
     * humidity in percentage
     */
    double humidity() const;
    /**
     * set humidity in percentage
     */
    void setHumidity(double humidity);
    /**
     * fog in percentage
     */
    double fog() const;
    /**
     * set fog in percentage
     */
    void setFog(double fog);
    /**
     * uv index, 0-1
     */
    double uvIndex() const;
    /**
     * set uv index, 0-1
     */
    void setUvIndex(double uvIndex);
    /**
     * precipitation in mm
     */
    double precipitationAmount() const;
    /**
     * set precipitation in mm
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
