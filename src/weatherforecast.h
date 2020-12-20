/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once
#include "dailyforecast.h"
#include "sunrise.h"
#include <QSharedData>
#include <kweathercore/kweathercore_export.h>
namespace KWeatherCore
{
/**
 * @short The WeatherForecast class contains the weather information of one location for days
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
    /**
     * @brief convert to QJsonObject
     */
    QJsonObject toJson() const;
    /**
     * @brief construct from json
     */
    static QExplicitlySharedDataPointer<WeatherForecast> fromJson(QJsonObject obj);
    /**
     * @return daily weather forecast
     */
    const std::vector<DailyWeatherForecast> &dailyWeatherForecast() const
    {
        return m_dailyWeatherForecast;
    };
    double latitude() const
    {
        return m_latitude;
    };
    double longitude() const
    {
        return m_longitude;
    };
    /**
     * @return the time this forecast object was created, this value won't change once constructed
     */
    const QDateTime &createdTime() const
    {
        return m_createdTime;
    };
    /**
     * @brief internal std::vector
     * @return
     */
    std::vector<DailyWeatherForecast> &dailyWeatherForecast()
    {
        return m_dailyWeatherForecast;
    };
    /**
     * @brief IANA Time Zone ID
     * @return
     */
    const QString &timezone() const
    {
        return m_timezone;
    };
    /**
     * @brief setCoordinate
     */
    void setCoordinate(double latitude, double longitude)
    {
        m_latitude = latitude;
        m_longitude = longitude;
    };
    /**
     * @param timezone valid IANA Time Zone ID
     */
    void setTimezone(QString timezone)
    {
        m_timezone = std::move(timezone);
    };
    void setDailyWeatherForecast(const std::vector<DailyWeatherForecast> &forecast)
    {
        m_dailyWeatherForecast = forecast;
    };
    void setDailyWeatherForecast(std::vector<DailyWeatherForecast> &&forecast)
    {
        m_dailyWeatherForecast = std::move(forecast);
    };
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
    std::vector<DailyWeatherForecast> m_dailyWeatherForecast;
    // std::vector<Sunrise> m_sunriseForecast;
    QString m_timezone;
    double m_latitude, m_longitude;
    QDateTime m_createdTime;
};
}
