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
class KWEATHERCORE_EXPORT DailyWeatherForecast
{
public:
    DailyWeatherForecast();
    DailyWeatherForecast(double maxTemp, double minTemp, double precipitation, double uvIndex, double humidity, double pressure, QString weatherIcon, QString weatherDescription, QDate date);

    QJsonObject toJson();
    static DailyWeatherForecast fromJson(QJsonObject obj);

    bool isNull() const;
    void setMaxTemp(double maxTemp);
    void setMinTemp(double minTemp);
    void setPrecipitation(double precipitation);
    void setUvIndex(double uvIndex);
    void setHumidity(double humidity);
    void setPressure(double pressure);
    void setWeatherIcon(QString icon);
    void setWeatherDescription(QString description);
    void setDate(QDate date);
    double maxTemp() const;
    double minTemp() const;
    double precipitation() const;
    double uvIndex() const;
    double humidity() const;
    double pressure() const;
    QString weatherIcon() const;
    QString weatherDescription() const;
    QDate date() const;

    const Sunrise &sunrise() const;
    const QVector<HourlyWeatherForecast> &hourlyWeatherForecast() const;
    QVector<HourlyWeatherForecast> &hourlyWeatherForecast();
    void setSunrise(const Sunrise &sunrise);
    void setSunrise(Sunrise &&sunrise);
    void setHourlyWeatherForecast(const QVector<HourlyWeatherForecast> &forecast);
    void setHourlyWeatherForecast(QVector<HourlyWeatherForecast> &&forecast);

    DailyWeatherForecast &operator+(const DailyWeatherForecast &forecast);
    DailyWeatherForecast &operator+=(const DailyWeatherForecast &forecast);
    DailyWeatherForecast &operator+=(const HourlyWeatherForecast &forecast);
    DailyWeatherForecast &operator<<(const DailyWeatherForecast &forecast);
    DailyWeatherForecast &operator<<(const HourlyWeatherForecast &forecast);
    bool operator==(const DailyWeatherForecast &forecast) const;
    bool operator<(const DailyWeatherForecast &forecast) const;
    bool operator>(const DailyWeatherForecast &forecast) const;
    bool operator>=(const DailyWeatherForecast &forecast) const;
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
