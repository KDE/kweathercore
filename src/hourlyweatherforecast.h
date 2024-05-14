/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <kweathercore/kweathercore_export.h>

#include <QDateTime>
#include <QSharedDataPointer>

class QJsonObject;

namespace KWeatherCore
{
Q_NAMESPACE_EXPORT(KWEATHERCORE_EXPORT)
enum class WindDirection { N, NW, W, SW, S, SE, E, NE };
Q_ENUM_NS(WindDirection)

class HourlyWeatherForecastPrivate;

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
    Q_PROPERTY(QDateTime date READ date)
    Q_PROPERTY(QString weatherDescription READ weatherDescription)
    Q_PROPERTY(QString weatherIcon READ weatherIcon)
    Q_PROPERTY(QString neutralWeatherIcon READ neutralWeatherIcon)
    Q_PROPERTY(double windDirectionDegree READ windDirectionDegree)
    Q_PROPERTY(KWeatherCore::WindDirection windDirectionCardinal READ windDirectionCardinal STORED false)
    Q_PROPERTY(qreal temperature READ temperature)
    Q_PROPERTY(qreal pressure READ pressure)
    Q_PROPERTY(qreal windSpeed READ windSpeed)
    Q_PROPERTY(qreal humidity READ humidity)
    Q_PROPERTY(qreal fog READ fog)
    Q_PROPERTY(qreal uvIndex READ uvIndex)
    Q_PROPERTY(qreal precipitationAmount READ precipitationAmount)
public:
    /**
     * HourlyWeatherForecast construct a null forecast
     */
    HourlyWeatherForecast();
    explicit HourlyWeatherForecast(const QDateTime &date);
    HourlyWeatherForecast(const HourlyWeatherForecast &other);
    HourlyWeatherForecast(HourlyWeatherForecast &&other) noexcept;
    ~HourlyWeatherForecast();

    /**
     * convert this to QJsonObject
     */
    QJsonObject toJson() const;
    /**
     * construct from QJsonObject
     */
    static HourlyWeatherForecast fromJson(const QJsonObject &obj);
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
     * Wind direction in degree.
     * That is, the direction the wind is coming from.
     * @see https://en.wikipedia.org/wiki/Wind_direction
     * @see windDirectionCardinal
     */
    double windDirectionDegree() const;
    /**
     * Sets the wind direction in degree.
     * @see windDirectionDegree
     */
    void setWindDirectionDegree(double windDirection);
    /**
     * Cardinal wind direction.
     * That is, the cardinal direction the wind is coming from.
     * @see windDirectionDegree
     */
    WindDirection windDirectionCardinal() const;
    /**
     * wind speed in km/h
     */
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
    HourlyWeatherForecast &operator=(const HourlyWeatherForecast &other);
    HourlyWeatherForecast &operator=(HourlyWeatherForecast &&other) noexcept;

private:
    QSharedDataPointer<HourlyWeatherForecastPrivate> d;
};
}
