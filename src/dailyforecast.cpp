/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "dailyforecast.h"
#include "kweathercore_p.h"
#include "pendingweatherforecast_p.h"
#include <QJsonArray>
namespace KWeatherCore
{
DailyWeatherForecast::DailyWeatherForecast()
{
    m_weatherDescription = QStringLiteral("Unknown");
    m_weatherIcon = QStringLiteral("weather-none-available");
    m_date = QDate();
    m_isNull = true;
}
DailyWeatherForecast::DailyWeatherForecast(double maxTemp, double minTemp, double precipitation, double uvIndex, double humidity, double pressure, QString weatherIcon, QString weatherDescription, QDate date)
    : m_maxTemp(maxTemp)
    , m_minTemp(minTemp)
    , m_precipitation(precipitation)
    , m_uvIndex(uvIndex)
    , m_humidity(humidity)
    , m_pressure(pressure)
    , m_weatherIcon(std::move(weatherIcon))
    , m_weatherDescription(std::move(weatherDescription))
    , m_date(std::move(date))
{
}

QJsonObject DailyWeatherForecast::toJson()
{
    QJsonObject obj;
    QJsonArray hourlyArray;
    obj[QStringLiteral("maxTemp")] = maxTemp();
    obj[QStringLiteral("minTemp")] = minTemp();
    obj[QStringLiteral("precipitation")] = precipitation();
    obj[QStringLiteral("uvIndex")] = uvIndex();
    obj[QStringLiteral("humidity")] = humidity();
    obj[QStringLiteral("pressure")] = pressure();
    obj[QStringLiteral("weatherIcon")] = weatherIcon();
    obj[QStringLiteral("weatherDescription")] = weatherDescription();
    obj[QStringLiteral("date")] = date().toString(Qt::ISODate);
    for (auto h : hourlyWeatherForecast()) {
        hourlyArray.append(h.toJson());
    }
    obj[QStringLiteral("hourly")] = hourlyArray;
    return obj;
}
DailyWeatherForecast DailyWeatherForecast::fromJson(QJsonObject obj)
{
    auto d = DailyWeatherForecast(obj[QStringLiteral("maxTemp")].toDouble(),
                                  obj[QStringLiteral("minTemp")].toDouble(),
                                  obj[QStringLiteral("precipitation")].toDouble(),
                                  obj[QStringLiteral("uvIndex")].toDouble(),
                                  obj[QStringLiteral("humidity")].toDouble(),
                                  obj[QStringLiteral("pressure")].toDouble(),
                                  obj[QStringLiteral("weatherIcon")].toString(),
                                  obj[QStringLiteral("weatherDescription")].toString(),
                                  QDate::fromString(obj[QStringLiteral("date")].toString(), Qt::ISODate));
    std::vector<HourlyWeatherForecast> hourlyVec;
    for (auto h : obj[QStringLiteral("hourly")].toArray())
        hourlyVec.push_back(HourlyWeatherForecast::fromJson(h.toObject()));
    d.setHourlyWeatherForecast(hourlyVec);
    return d;
}

DailyWeatherForecast &DailyWeatherForecast::operator+(const DailyWeatherForecast &forecast)
{
    if (this->date().isNull())
        this->setDate(forecast.date());

    if (*this == forecast) {
        this->setPrecipitation(this->precipitation() + forecast.precipitation());
        this->setUvIndex(std::max(this->uvIndex(), forecast.uvIndex()));
        this->setHumidity(std::max(this->humidity(), forecast.humidity()));
        this->setPressure(std::max(this->pressure(), forecast.pressure()));
        this->setMaxTemp(std::max(this->maxTemp(), forecast.maxTemp()));
        this->setMinTemp(std::min(this->minTemp(), forecast.minTemp()));
    }

    return *this;
}

DailyWeatherForecast &DailyWeatherForecast::operator+=(const DailyWeatherForecast &forecast)
{
    return *this + forecast;
}

DailyWeatherForecast &DailyWeatherForecast::operator+=(const HourlyWeatherForecast &forecast)
{
    if (this->isNull()) {
        this->setDate(forecast.date().date());
        this->setWeatherDescription(forecast.weatherDescription());
        this->setWeatherIcon(forecast.weatherIcon());
        this->m_isNull = false;
    }
    if (this->date().daysTo(forecast.date().date()) == 0) {
        // set description and icon if it is higher ranked
        if (rank[forecast.neutralWeatherIcon()] >= rank[this->weatherIcon()]) {
            this->setWeatherDescription(apiDescMap[forecast.symbolCode() + QStringLiteral("_neutral")].desc);
            this->setWeatherIcon(forecast.neutralWeatherIcon());
        }
        this->setPrecipitation(this->precipitation() + forecast.precipitationAmount());
        this->setUvIndex(std::max(this->uvIndex(), forecast.uvIndex()));
        this->setHumidity(std::max(this->humidity(), forecast.humidity()));
        this->setPressure(std::max(this->pressure(), forecast.pressure()));
        this->setMaxTemp(std::max(this->maxTemp(), forecast.temperature()));
        this->setMinTemp(std::min(this->minTemp(), forecast.temperature()));
    }

    this->hourlyWeatherForecast().push_back(forecast);
    return *this;
}

bool DailyWeatherForecast::operator==(const DailyWeatherForecast &forecast) const
{
    return (this->date() == forecast.date() && this->weatherDescription() == forecast.weatherDescription() && this->weatherIcon() == forecast.weatherIcon());
}

bool DailyWeatherForecast::operator<(const DailyWeatherForecast &forecast) const
{
    return this->date() < forecast.date();
}
}
