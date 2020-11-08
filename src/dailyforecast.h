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
 *
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
    bool isNull() const;
    /**
     * @brief setMaxTemp set the maximum temperature of the day
     * @param maxTemp maximum temperature of the day, in celsius
     */
    void setMaxTemp(double maxTemp);
    /**
     * @brief setMinTemp set the minimum temperature of the day
     * @param minTemp minimum temperature of the day, in celsius
     */
    void setMinTemp(double minTemp);
    /**
     * @brief setPrecipitation set the precipitation of the day
     * @param precipitation precipitation of the day, in mm
     */
    void setPrecipitation(double precipitation);
    /**
     * @brief setUvIndex set the UvIndex of the day
     * @param uvIndex 0-1
     */
    void setUvIndex(double uvIndex);
    /**
     * @brief setHumidity set the humidity of the day
     * @param humidity humidity of the day, in percentage
     */
    void setHumidity(double humidity);
    /**
     * @brief setPressure set the pressure of the day
     * @param pressure pressure of the day, in hpa
     */
    void setPressure(double pressure);
    /**
     * @brief setWeatherIcon set the weather icon of the day
     * @param icon
     */
    void setWeatherIcon(QString icon);
    /**
     * @brief setWeatherDescription set the weather description of the day
     * @param description
     */
    void setWeatherDescription(QString description);
    /**
     * @brief setDate set the date this object represents
     * @param date
     */
    void setDate(QDate date);
    /**
     * @brief maxTemp return maximum temperature
     * @return maximum temperature, this value is initialized to the smallest value double can hold
     */
    double maxTemp() const;
    /**
     * @brief minTemp return minimum temperature
     * @return minimum temperature, this value is initialized to the largest value double can hold
     */
    double minTemp() const;
    /**
     * @brief precipitation return precipitation
     * @return this value is initialized to zero
     */
    double precipitation() const;
    /**
     * @brief uvIndex return uvIndex
     * @return this value is initialized to zero
     */
    double uvIndex() const;
    /**
     * @brief humidity return humidity
     * @return this value is initialized to zero
     */
    double humidity() const;
    /**
     * @brief pressure return pressure
     * @return this value is initialized to zero
     */
    double pressure() const;
    /**
     * @brief weatherIcon return weather icon
     * @return weather icon, can be empty string if constructed without data
     */
    const QString &weatherIcon() const;
    /**
     * @brief weatherDescription return weather description
     * @return weather description, can be empty string if constructed without data
     */
    const QString &weatherDescription() const;
    /**
     * @brief date return date this object represents
     * @return date, date can be invalid if constructed without data
     */
    const QDate &date() const;

    /**
     * @brief sunrise return sunrise data
     * @return sunrise data
     */
    const Sunrise &sunrise() const;
    /**
     * @brief hourlyWeatherForecast returns all HourlyWeathreForecast belonged to this day
     * @return all HourlyWeathreForecast belonged to this day
     */
    const QVector<HourlyWeatherForecast> &hourlyWeatherForecast() const;
    /**
     * @brief hourlyWeatherForecast overloaded version
     * @return reference to internal QVector
     */
    QVector<HourlyWeatherForecast> &hourlyWeatherForecast();
    /**
     * @brief setSunrise set sunrise
     * @param sunrise if this object and sunrise isn't on the same day, this function does nothing
     */
    void setSunrise(const Sunrise &sunrise);
    /**
     * @brief setSunrise overloaded version
     * @param sunrise
     */
    void setSunrise(Sunrise &&sunrise);
    /**
     * @brief setHourlyWeatherForecast set the hourly forecast of the day
     * @param forecast make sure they are sorted and on the same day
     */
    void setHourlyWeatherForecast(const QVector<HourlyWeatherForecast> &forecast);
    /**
     * @brief setHourlyWeatherForecast overloaded version
     * @param forecast
     */
    void setHourlyWeatherForecast(QVector<HourlyWeatherForecast> &&forecast);

    /**
     * @brief operator + merge two daily forecast
     * @param forecast make sure it is on the same day
     * @return result DailyWeatherForecast
     */
    DailyWeatherForecast &operator+(const DailyWeatherForecast &forecast);
    /**
     * @brief operator += merge two daily forecast
     * @param forecast make sure it is on the same day
     * @return result DailyWeatherForecast
     */
    DailyWeatherForecast &operator+=(const DailyWeatherForecast &forecast);
    /**
     * @brief operator += append hourly forecast, you can append valid hourly forecast into a null daily forecast
     * @param forecast make sure it's on the same day
     * @return result DailyWeatherForecast
     */
    DailyWeatherForecast &operator+=(const HourlyWeatherForecast &forecast);
    /**
     * @brief operator << overloaded version
     * @param forecast
     * @return
     */
    DailyWeatherForecast &operator<<(const DailyWeatherForecast &forecast);
    /**
     * @brief operator << overloaded version
     * @param forecast
     * @return
     */
    DailyWeatherForecast &operator<<(const HourlyWeatherForecast &forecast);
    /**
     * @brief operator == if on the same day
     * @param forecast
     * @return @c true if on the same day
     */
    bool operator==(const DailyWeatherForecast &forecast) const;
    /**
     * @brief operator < if this is earlier than \param forecast
     * @param forecast
     * @return @c true if this is earlier than \param forecast
     */
    bool operator<(const DailyWeatherForecast &forecast) const;
    /**
     * @brief operator > if this is later than \param forecast
     * @param forecast
     * @return @c true if this is later than \param forecast
     */
    bool operator>(const DailyWeatherForecast &forecast) const;
    /**
     * @brief operator >= this does exactly what you think it does
     * @param forecast
     * @return
     */
    bool operator>=(const DailyWeatherForecast &forecast) const;
    /**
     * @brief operator <= this does exactly what you think it does
     * @param forecast
     * @return
     */
    bool operator<=(const DailyWeatherForecast &forecast) const;

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
    QVector<HourlyWeatherForecast> m_hourlyWeatherForecast;
};
}
