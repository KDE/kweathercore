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
     * @brief WeatherForecast construct an empty object
     */
    WeatherForecast();
    /**
     * @brief toJson convert to QJsonObject
     * @return
     */
    QJsonObject toJson() const;
    /**
     * @brief fromJson construct from json
     * @param obj
     * @return
     */
    static QExplicitlySharedDataPointer<WeatherForecast> fromJson(QJsonObject obj);
    /**
     * @brief dailyWeatherForecast return daily weather forecast
     * @return
     */
    const QVector<DailyWeatherForecast> &dailyWeatherForecast() const;
    /**
     * @brief latitude
     * @return
     */
    double latitude() const;
    /**
     * @brief longitude
     * @return
     */
    double longitude() const;
    /**
     * @brief createdTime the time this forecast was created
     * @return
     */
    const QDateTime &createdTime() const;
    /**
     * @brief dailyWeatherForecast internal QVector
     * @return
     */
    QVector<DailyWeatherForecast> &dailyWeatherForecast();
    /**
     * @brief timezone timezone
     * @return
     */
    const QString &timezone() const;
    /**
     * @brief setCoordinate
     * @param latitude
     * @param longitude
     */
    void setCoordinate(double latitude, double longitude);
    /**
     * @brief setTimezone
     * @param timezone
     */
    void setTimezone(const QString &timezone);
    /**
     * @brief setTimezone
     * @param timezone
     */
    void setTimezone(QString &&timezone);
    /**
     * @brief setDailyWeatherForecast
     * @param forecast
     */
    void setDailyWeatherForecast(const QVector<DailyWeatherForecast> &forecast);
    /**
     * @brief setDailyWeatherForecast
     * @param forecast
     */
    void setDailyWeatherForecast(QVector<DailyWeatherForecast> &&forecast);
    /**
     * @brief setSunriseForecast the vector should be sorted
     * @param sunrise
     */
    void setSunriseForecast(const QVector<Sunrise> &sunrise);
    /**
     * @brief setSunriseForecast overloaded version
     * @param sunrise
     */
    void setSunriseForecast(QVector<Sunrise> &&sunrise);

    /**
     * @brief operator += merge DailyWeatherForecast
     * @param forecast
     * @return
     */
    WeatherForecast &operator+=(const DailyWeatherForecast &forecast);
    /**
     * @brief operator += overloaded version
     * @param forecast
     * @return
     */
    WeatherForecast &operator+=(DailyWeatherForecast &&forecast);
    /**
     * @brief operator += merge HourlyWeatherForecast, a new day maybe created
     * @param forecast
     * @return
     */
    WeatherForecast &operator+=(const HourlyWeatherForecast &forecast);
    /**
     * @brief operator += overloaded version
     * @param forecast
     * @return
     */
    WeatherForecast &operator+=(HourlyWeatherForecast &&forecast);

private:
    QVector<DailyWeatherForecast> m_dailyWeatherForecast;
    // QVector<Sunrise> m_sunriseForecast;
    QString m_timezone;
    double m_latitude, m_longitude;
    QDateTime m_createdTime;
};
}
