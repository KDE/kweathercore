/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
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
#include <memory>
namespace KWeatherCore
{
class DailyWeatherForecastPrivate
{
public:
    bool isValid = true;

    double maxTemp = std::numeric_limits<double>::lowest();
    double minTemp = std::numeric_limits<double>::max();
    double precipitation = 0; // mm
    double uvIndex = 0; // 0-1
    double humidity = 0; // %
    double pressure = 0; // hPa
    QString weatherIcon = QStringLiteral("weather-none-available");
    QString weatherDescription = QStringLiteral("Unknown");
    QDate date;

    Sunrise sunrise;
    std::vector<HourlyWeatherForecast> hourlyWeatherForecast;
};

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
    Q_PROPERTY(bool valid READ isValid)
    Q_PROPERTY(qreal maxTemp READ maxTemp WRITE setMaxTemp)
    Q_PROPERTY(qreal minTemp READ minTemp WRITE setMinTemp)
    Q_PROPERTY(qreal precipitation READ precipitation WRITE setPrecipitation)
    Q_PROPERTY(qreal uvIndex READ uvIndex WRITE setUvIndex)
    Q_PROPERTY(qreal humidity READ humidity WRITE setHumidity)
    Q_PROPERTY(qreal pressure READ pressure WRITE setPressure)
    Q_PROPERTY(QString weatherIcon READ weatherIcon WRITE setWeatherIcon)
    Q_PROPERTY(QString weatherDescription READ weatherDescription WRITE
                   setWeatherDescription)
    Q_PROPERTY(QDateTime date READ dateTime WRITE setDate)
public:
    /**
     * Creates a invalid DailyWeatherForecast.
     */
    DailyWeatherForecast();
    explicit DailyWeatherForecast(const QDate &date);
    DailyWeatherForecast(const DailyWeatherForecast &other);
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
    bool isValid() const;
    /**
     * set the maximum temperature of the day
     * @param maxTemp maximum temperature of the day, in celsius
     */
    void setMaxTemp(double maxTemp);
    /**
     * set the minimum temperature of the day
     * @param minTemp minimum temperature of the day, in celsius
     */
    void setMinTemp(double minTemp);
    /**
     * set the precipitation of the day
     * @param precipitation precipitation of the day, in mm
     */
    void setPrecipitation(double precipitation);
    /**
     * set the UvIndex of the day
     * @param uvIndex 0-1
     */
    void setUvIndex(double uvIndex);
    /**
     * set the humidity of the day
     * @param humidity humidity of the day, in percentage
     */
    void setHumidity(double humidity);
    /**
     * set the pressure of the day
     * @param pressure pressure of the day, in hpa
     */
    void setPressure(double pressure);
    /**
     * set the weather icon of the day
     * @param icon
     */
    void setWeatherIcon(const QString &icon);
    /**
     * set the weather description of the day
     * @param description
     */
    void setWeatherDescription(const QString &description);
    /**
     * set the date this object represents
     * @param date
     */
    void setDate(const QDate &date);
    void setDate(const QDateTime &date);
    /**
     * return maximum temperature
     * @return maximum temperature, this value is initialized to the smallest
     * value double can hold
     */
    double maxTemp() const;
    /**
     * return minimum temperature
     * @return minimum temperature, this value is initialized to the largest
     * value double can hold
     */
    double minTemp() const;
    /**
     * return precipitation
     * @return this value is initialized to zero
     */
    double precipitation() const;
    /**
     * return uvIndex
     * @return this value is initialized to zero
     */
    double uvIndex() const;
    /**
     * return humidity
     * @return this value is initialized to zero
     */
    double humidity() const;
    /**
     * return pressure
     * @return this value is initialized to zero
     */
    double pressure() const;
    /**
     * return weather icon
     * @return weather icon, can be empty string if constructed without data
     */
    const QString &weatherIcon() const;
    /**
     * return weather description
     * @return weather description, can be empty string if constructed without
     * data
     */
    const QString &weatherDescription() const;
    /**
     * return date this object represents
     * @return date, date can be invalid if constructed without data
     */
    const QDate &date() const;
    QDateTime dateTime() const;
    /**
     * return sunrise data
     * @return sunrise data
     */
    const Sunrise &sunrise() const;
    /**
     * returns all HourlyWeathreForecast belonged to this day
     * @return all HourlyWeathreForecast belonged to this day
     */
    const std::vector<HourlyWeatherForecast> &hourlyWeatherForecast() const;
    /**
     * set sunrise
     * @param sunrise if this object and sunrise isn't on the same day, this
     * function does nothing
     */
    void setSunrise(Sunrise sunrise);
    /**
     * set the hourly forecast of the day
     * @param forecast make sure they are sorted and on the same day
     */
    void setHourlyWeatherForecast(
        const std::vector<HourlyWeatherForecast> &forecast);
    /**
     * overloaded version
     * @param forecast
     */
    void
    setHourlyWeatherForecast(std::vector<HourlyWeatherForecast> &&forecast);

    /**
     * merge two daily forecast, note the hourly forecast is unchanged
     * @param forecast make sure it is on the same day
     * @return result DailyWeatherForecast
     */
    DailyWeatherForecast &operator+(const DailyWeatherForecast &forecast);
    /**
     * merge two daily forecast, note the hourly forecast is unchanged,
     * daily forecast becomes valid afterwards
     * @param forecast make sure it is on the same day
     * @return result DailyWeatherForecast
     */
    DailyWeatherForecast &operator+=(const DailyWeatherForecast &forecast);
    /**
     * append hourly forecast, you can append valid hourly forecast into
     * a invalid daily forecast, daily forecast becomes valid afterwards
     * @param forecast make sure it's on the same day
     * @return result DailyWeatherForecast
     */
    DailyWeatherForecast &operator+=(const HourlyWeatherForecast &forecast);
    /**
     * if on the same day
     * @param forecast
     * @return @c true if on the same day
     */
    bool operator==(const DailyWeatherForecast &forecast) const;
    /**
     * if this is earlier than \param forecast
     * @param forecast
     * @return @c true if this is earlier than \param forecast
     */
    bool operator<(const DailyWeatherForecast &forecast) const;

    DailyWeatherForecast &operator=(const DailyWeatherForecast &other);

private:
    std::unique_ptr<DailyWeatherForecastPrivate> d;
};
}
