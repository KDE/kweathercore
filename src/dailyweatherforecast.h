/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "hourlyweatherforecast.h"
#include "sunrise.h"
#include <QDate>
#include <QJsonObject>
#include <kweathercore/kweathercore_export.h>
namespace KWeatherCore
{
class DailyWeatherForecastPrivate;
/**
 * @short Class represents weatherforecast in a day
 *
 * This is a class to hold general weather conditions, hourly forecast and
 * sunrise in a day All QDate/DateTime are on the location's timezone
 * @see HourlyForecast, Sunrise
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT DailyWeatherForecast
{
    Q_GADGET
    Q_PROPERTY(bool null READ isNull)
    Q_PROPERTY(qreal maxTemp READ maxTemp WRITE setMaxTemp)
    Q_PROPERTY(qreal minTemp READ minTemp WRITE setMinTemp)
    Q_PROPERTY(qreal precipitation READ precipitation WRITE setPrecipitation)
    Q_PROPERTY(qreal uvIndex READ uvIndex WRITE setUvIndex)
    Q_PROPERTY(qreal humidity READ humidity WRITE setHumidity)
    Q_PROPERTY(qreal pressure READ pressure WRITE setPressure)
    Q_PROPERTY(QString weatherIcon READ weatherIcon WRITE setWeatherIcon)
    Q_PROPERTY(QString weatherDescription READ weatherDescription WRITE
                   setWeatherDescription)
    Q_PROPERTY(QDateTime date READ jsDate WRITE setJsDate)
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
     * @param weatherDescription text describes the general weather condition of
     * the day
     * @param date the date this object represents
     */
    DailyWeatherForecast(double maxTemp,
                         double minTemp,
                         double precipitation,
                         double uvIndex,
                         double humidity,
                         double pressure,
                         QString weatherIcon,
                         QString weatherDescription,
                         QDate date);
    ~DailyWeatherForecast();
    /**
     * Return a QJsonObject that can be converted back with
     * DailyWeatherForecast::fromJson
     */
    QJsonObject toJson();
    /**
     * Construct a DailyWeatherForecast from QJsonObject
     */
    static DailyWeatherForecast fromJson(QJsonObject obj);
    /**
     * @return @c true if the object is created without data
     * this value won't change once the class is created with the exceptions of
     * Day/Hour merge
     */
    bool isNull() const;
    /**
     * @brief set the maximum temperature of the day
     * @param maxTemp maximum temperature of the day, in celsius
     */
    void setMaxTemp(double maxTemp);
    /**
     * @brief set the minimum temperature of the day
     * @param minTemp minimum temperature of the day, in celsius
     */
    void setMinTemp(double minTemp);
    /**
     * @brief set the precipitation of the day
     * @param precipitation precipitation of the day, in mm
     */
    void setPrecipitation(double precipitation);
    /**
     * @brief set the UvIndex of the day
     * @param uvIndex 0-1
     */
    void setUvIndex(double uvIndex);
    /**
     * @brief set the humidity of the day
     * @param humidity humidity of the day, in percentage
     */
    void setHumidity(double humidity);
    /**
     * @brief set the pressure of the day
     * @param pressure pressure of the day, in hpa
     */
    void setPressure(double pressure);
    /**
     * @brief set the weather icon of the day
     * @param icon
     */
    void setWeatherIcon(QString icon);
    /**
     * @brief set the weather description of the day
     * @param description
     */
    void setWeatherDescription(QString description);
    /**
     * @brief set the date this object represents
     * @param date
     */
    void setDate(QDate date);
    KWEATHERCORE_NO_EXPORT void setJsDate(const QDateTime &date);
    /**
     * @brief return maximum temperature
     * @return maximum temperature, this value is initialized to the smallest
     * value double can hold
     */
    double maxTemp() const;
    /**
     * @brief return minimum temperature
     * @return minimum temperature, this value is initialized to the largest
     * value double can hold
     */
    double minTemp() const;
    /**
     * @brief return precipitation
     * @return this value is initialized to zero
     */
    double precipitation() const;
    /**
     * @brief return uvIndex
     * @return this value is initialized to zero
     */
    double uvIndex() const;
    /**
     * @brief return humidity
     * @return this value is initialized to zero
     */
    double humidity() const;
    /**
     * @brief return pressure
     * @return this value is initialized to zero
     */
    double pressure() const;
    /**
     * @brief return weather icon
     * @return weather icon, can be empty string if constructed without data
     */
    const QString &weatherIcon() const;
    /**
     * @brief return weather description
     * @return weather description, can be empty string if constructed without
     * data
     */
    const QString &weatherDescription() const;
    /**
     * @brief return date this object represents
     * @return date, date can be invalid if constructed without data
     */
    const QDate &date() const;
    KWEATHERCORE_NO_EXPORT QDateTime jsDate() const;
    /**
     * @brief return sunrise data
     * @return sunrise data
     */
    const Sunrise &sunrise() const;
    /**
     * @brief returns all HourlyWeathreForecast belonged to this day
     * @return all HourlyWeathreForecast belonged to this day
     */
    const std::vector<HourlyWeatherForecast> &hourlyWeatherForecast() const;
    /**
     * @brief set sunrise
     * @param sunrise if this object and sunrise isn't on the same day, this
     * function does nothing
     */
    void setSunrise(Sunrise sunrise);
    /**
     * @brief set the hourly forecast of the day
     * @param forecast make sure they are sorted and on the same day
     */
    void setHourlyWeatherForecast(
        const std::vector<HourlyWeatherForecast> &forecast);
    /**
     * @brief overloaded version
     * @param forecast
     */
    void
    setHourlyWeatherForecast(std::vector<HourlyWeatherForecast> &&forecast);

    /**
     * @brief merge two daily forecast, note the hourly forecast is unchanged
     * @param forecast make sure it is on the same day
     * @return result DailyWeatherForecast
     */
    DailyWeatherForecast &operator+(const DailyWeatherForecast &forecast);
    /**
     * @brief merge two daily forecast, note the hourly forecast is unchanged,
     * daily forecast becomes valid afterwards
     * @param forecast make sure it is on the same day
     * @return result DailyWeatherForecast
     */
    DailyWeatherForecast &operator+=(const DailyWeatherForecast &forecast);
    /**
     * @brief append hourly forecast, you can append valid hourly forecast into
     * a null daily forecast, daily forecast becomes valid afterwards
     * @param forecast make sure it's on the same day
     * @return result DailyWeatherForecast
     */
    DailyWeatherForecast &operator+=(const HourlyWeatherForecast &forecast);
    /**
     * @brief if on the same day
     * @param forecast
     * @return @c true if on the same day
     */
    bool operator==(const DailyWeatherForecast &forecast) const;
    /**
     * @brief if this is earlier than \param forecast
     * @param forecast
     * @return @c true if this is earlier than \param forecast
     */
    bool operator<(const DailyWeatherForecast &forecast) const;

private:
    DailyWeatherForecastPrivate *d;
};
}
