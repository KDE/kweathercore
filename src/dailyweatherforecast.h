/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include <kweathercore/kweathercore_export.h>

#include "hourlyweatherforecast.h"

#include <QDate>
#include <QSharedDataPointer>

class QJsonObject;

namespace KWeatherCore
{

class DailyWeatherForecastPrivate;

/**
 * @short Class represents weatherforecast in a day
 *
 * This is a class to hold general weather conditions and hourly forecast
 * in a day All QDate/DateTime are on the location's timezone
 * @see HourlyForecast
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT DailyWeatherForecast
{
    Q_GADGET
    Q_PROPERTY(qreal maxTemp READ maxTemp)
    Q_PROPERTY(qreal minTemp READ minTemp)
    Q_PROPERTY(qreal precipitation READ precipitation)
    Q_PROPERTY(qreal uvIndex READ uvIndex)
    Q_PROPERTY(qreal humidity READ humidity)
    Q_PROPERTY(qreal pressure READ pressure)
    Q_PROPERTY(QString weatherIcon READ weatherIcon)
    Q_PROPERTY(QString weatherDescription READ weatherDescription)
    Q_PROPERTY(QDateTime date READ dateTime)
public:
    /**
     * Creates a invalid DailyWeatherForecast.
     */
    DailyWeatherForecast();
    explicit DailyWeatherForecast(const QDate &date);
    DailyWeatherForecast(const DailyWeatherForecast &other);
    ~DailyWeatherForecast();
    DailyWeatherForecast(DailyWeatherForecast &&other) noexcept;
    /**
     * Return a QJsonObject that can be converted back with
     * DailyWeatherForecast::fromJson
     */
    QJsonObject toJson() const;
    /**
     * Construct a DailyWeatherForecast from QJsonObject
     */
    static DailyWeatherForecast fromJson(const QJsonObject &obj);
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
     * returns all HourlyWeathreForecast belonged to this day
     * @return all HourlyWeathreForecast belonged to this day
     */
    const std::vector<HourlyWeatherForecast> &hourlyWeatherForecast() const;
    /**
     * overloaded version
     * @param forecast
     */
    void setHourlyWeatherForecast(std::vector<HourlyWeatherForecast> &&forecast);

    /**
     * append hourly forecast, you can append valid hourly forecast into
     * a invalid daily forecast, daily forecast becomes valid afterwards
     * @param forecast make sure it's on the same day
     * @return result DailyWeatherForecast
     */
    DailyWeatherForecast &operator+=(HourlyWeatherForecast &&forecast);
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
    DailyWeatherForecast &operator=(DailyWeatherForecast &&other) noexcept;

private:
    QSharedDataPointer<DailyWeatherForecastPrivate> d;
};
}
