/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
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
     * construct an empty object
     */
    WeatherForecast();
    WeatherForecast(const WeatherForecast &other);
    WeatherForecast(WeatherForecast &&other);
    ~WeatherForecast();
    WeatherForecast &operator=(const WeatherForecast &other);
    /**
     * convert to QJsonObject
     */
    QJsonObject toJson() const;
    /**
     * construct from json
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
     * IANA Time Zone ID
     * @return
     */
    const QString &timezone() const;
    /**
     * setCoordinate
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
     * the vector should be sorted
     */
    void setSunriseForecast(const std::vector<Sunrise> &sunrise);
    /**
     * overloaded version
     */
    void setSunriseForecast(std::vector<Sunrise> &&sunrise);

    /**
     * merge DailyWeatherForecast
     */
    WeatherForecast &operator+=(const DailyWeatherForecast &forecast);
    /**
     * overloaded version
     */
    WeatherForecast &operator+=(DailyWeatherForecast &&forecast);
    /**
     * merge HourlyWeatherForecast, new day is created when required
     */
    WeatherForecast &operator+=(const HourlyWeatherForecast &forecast);
    /**
     * overloaded version
     */
    WeatherForecast &operator+=(HourlyWeatherForecast &&forecast);

private:
    class WeatherForecastPrivate;
    std::unique_ptr<WeatherForecastPrivate> d;
};
}
