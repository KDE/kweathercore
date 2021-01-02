/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "dailyweatherforecast.h"
#include "sunrise.h"
#include <QSharedData>
#include <kweathercore/kweathercore_export.h>
namespace KWeatherCore
{
class WeatherForecastPrivate;
/**
 * @short The WeatherForecast class contains the weather information of one
 * location for days
 *
 * @see WeatherForecastSource
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT WeatherForecast : public QSharedData
{
public:
    /**
     * @brief construct an empty object
     */
    WeatherForecast();
    ~WeatherForecast();
    /**
     * @brief convert to QJsonObject
     */
    QJsonObject toJson() const;
    /**
     * @brief construct from json
     */
    static QExplicitlySharedDataPointer<WeatherForecast>
    fromJson(QJsonObject obj);
    /**
     * @return daily weather forecast
     */
    const std::vector<DailyWeatherForecast> &dailyWeatherForecast() const;
    double latitude() const;
    double longitude() const;
    /**
     * @return the time this forecast object was created, this value won't
     * change once constructed
     */
    const QDateTime &createdTime() const;
    /**
     * @brief IANA Time Zone ID
     * @return
     */
    const QString &timezone() const;
    /**
     * @brief setCoordinate
     */
    void setCoordinate(double latitude, double longitude);
    /**
     * @param timezone valid IANA Time Zone ID
     */
    void setTimezone(QString timezone);
    void
    setDailyWeatherForecast(const std::vector<DailyWeatherForecast> &forecast);
    void setDailyWeatherForecast(std::vector<DailyWeatherForecast> &&forecast);
    /**
     * @brief the vector should be sorted
     */
    void setSunriseForecast(const std::vector<Sunrise> &sunrise);
    /**
     * @brief overloaded version
     */
    void setSunriseForecast(std::vector<Sunrise> &&sunrise);

    /**
     * @brief merge DailyWeatherForecast
     */
    WeatherForecast &operator+=(const DailyWeatherForecast &forecast);
    /**
     * @brief overloaded version
     */
    WeatherForecast &operator+=(DailyWeatherForecast &&forecast);
    /**
     * @brief merge HourlyWeatherForecast, new day is created when required
     */
    WeatherForecast &operator+=(const HourlyWeatherForecast &forecast);
    /**
     * @brief overloaded version
     */
    WeatherForecast &operator+=(HourlyWeatherForecast &&forecast);

private:
    WeatherForecastPrivate *d;
};
}
