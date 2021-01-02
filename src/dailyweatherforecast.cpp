/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "dailyweatherforecast.h"
#include "kweathercore_p.h"
#include "pendingweatherforecast_p.h"
#include <QJsonArray>
namespace KWeatherCore
{
class DailyWeatherForecastPrivate
{
public:
    bool isNull = true;

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

DailyWeatherForecast::DailyWeatherForecast()
    : d(new DailyWeatherForecastPrivate)
{
}
DailyWeatherForecast::DailyWeatherForecast(double maxTemp,
                                           double minTemp,
                                           double precipitation,
                                           double uvIndex,
                                           double humidity,
                                           double pressure,
                                           QString weatherIcon,
                                           QString weatherDescription,
                                           QDate date)
    : d(new DailyWeatherForecastPrivate)
{
    d->maxTemp = maxTemp;
    d->minTemp = minTemp;
    d->precipitation = precipitation;
    d->uvIndex = uvIndex;
    d->humidity = humidity;
    d->pressure = pressure;
    d->weatherIcon = std::move(weatherIcon);
    d->weatherDescription = std::move(weatherDescription);
    d->date = std::move(date);
    d->isNull = false;
}
DailyWeatherForecast::~DailyWeatherForecast()
{
    delete d;
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
    auto d = DailyWeatherForecast(
        obj[QStringLiteral("maxTemp")].toDouble(),
        obj[QStringLiteral("minTemp")].toDouble(),
        obj[QStringLiteral("precipitation")].toDouble(),
        obj[QStringLiteral("uvIndex")].toDouble(),
        obj[QStringLiteral("humidity")].toDouble(),
        obj[QStringLiteral("pressure")].toDouble(),
        obj[QStringLiteral("weatherIcon")].toString(),
        obj[QStringLiteral("weatherDescription")].toString(),
        QDate::fromString(obj[QStringLiteral("date")].toString(), Qt::ISODate));
    std::vector<HourlyWeatherForecast> hourlyVec;
    auto array = obj[QStringLiteral("hourly")].toArray();
    for (int i = 0; i < array.size(); i++) {
        hourlyVec.push_back(
            HourlyWeatherForecast::fromJson(array.at(i).toObject()));
    }
    d.setHourlyWeatherForecast(hourlyVec);
    return d;
}
bool DailyWeatherForecast::isNull() const
{
    return d->isNull;
}
void DailyWeatherForecast::setMaxTemp(double maxTemp)
{
    d->maxTemp = maxTemp;
}
void DailyWeatherForecast::setMinTemp(double minTemp)
{
    d->minTemp = minTemp;
}
void DailyWeatherForecast::setPrecipitation(double precipitation)
{
    d->precipitation = precipitation;
}
void DailyWeatherForecast::setUvIndex(double uvIndex)
{
    d->uvIndex = uvIndex;
}
void DailyWeatherForecast::setHumidity(double humidity)
{
    d->humidity = humidity;
}
void DailyWeatherForecast::setPressure(double pressure)
{
    d->pressure = pressure;
}
void DailyWeatherForecast::setWeatherIcon(QString icon)
{
    d->weatherIcon = std::move(icon);
}
void DailyWeatherForecast::setWeatherDescription(QString description)
{
    d->weatherDescription = std::move(description);
}
void DailyWeatherForecast::setDate(QDate date)
{
    d->date = std::move(date);
}
void DailyWeatherForecast::setJsDate(const QDateTime &date)
{
    d->date = date.date();
}
double DailyWeatherForecast::maxTemp() const
{
    return d->maxTemp;
}
double DailyWeatherForecast::minTemp() const
{
    return d->minTemp;
}
double DailyWeatherForecast::precipitation() const
{
    return d->precipitation;
}
double DailyWeatherForecast::uvIndex() const
{
    return d->uvIndex;
}
double DailyWeatherForecast::humidity() const
{
    return d->humidity;
}
double DailyWeatherForecast::pressure() const
{
    return d->pressure;
}
const QString &DailyWeatherForecast::weatherIcon() const
{
    return d->weatherIcon;
}
const QString &DailyWeatherForecast::weatherDescription() const
{
    return d->weatherDescription;
}
const QDate &DailyWeatherForecast::date() const
{
    return d->date;
}
QDateTime DailyWeatherForecast::jsDate() const
{
    return d->date.startOfDay();
}
const Sunrise &DailyWeatherForecast::sunrise() const
{
    return d->sunrise;
}
const std::vector<HourlyWeatherForecast> &
DailyWeatherForecast::hourlyWeatherForecast() const
{
    return d->hourlyWeatherForecast;
}
void DailyWeatherForecast::setSunrise(Sunrise sunrise)
{
    d->sunrise = std::move(sunrise);
}
void DailyWeatherForecast::setHourlyWeatherForecast(
    const std::vector<HourlyWeatherForecast> &forecast)
{
    d->hourlyWeatherForecast = forecast;
}
void DailyWeatherForecast::setHourlyWeatherForecast(
    std::vector<HourlyWeatherForecast> &&forecast)
{
    d->hourlyWeatherForecast = std::move(forecast);
}
DailyWeatherForecast &
DailyWeatherForecast::operator+(const DailyWeatherForecast &forecast)
{
    if (date().isNull()) {
        setDate(forecast.date());
        setWeatherDescription(forecast.weatherDescription());
        setWeatherIcon(forecast.weatherIcon());
        d->isNull = false;
    }

    if (*this == forecast) {
        setPrecipitation(precipitation() + forecast.precipitation());
        setUvIndex(std::max(uvIndex(), forecast.uvIndex()));
        setHumidity(std::max(humidity(), forecast.humidity()));
        setPressure(std::max(pressure(), forecast.pressure()));
        setMaxTemp(std::max(maxTemp(), forecast.maxTemp()));
        setMinTemp(std::min(minTemp(), forecast.minTemp()));
    }

    return *this;
}

DailyWeatherForecast &
DailyWeatherForecast::operator+=(const DailyWeatherForecast &forecast)
{
    return *this + forecast;
}

DailyWeatherForecast &
DailyWeatherForecast::operator+=(const HourlyWeatherForecast &forecast)
{
    if (isNull()) {
        setDate(forecast.date().date());
        setWeatherDescription(forecast.weatherDescription());
        setWeatherIcon(forecast.weatherIcon());
        d->isNull = false;
    }
    if (date().daysTo(forecast.date().date()) == 0) {
        // set description and icon if it is higher ranked
        if (rank[forecast.neutralWeatherIcon()] >= rank[weatherIcon()]) {
            setWeatherDescription(
                apiDescMap[forecast.symbolCode() + QStringLiteral("_neutral")]
                    .desc);
            setWeatherIcon(forecast.neutralWeatherIcon());
        }
        setPrecipitation(precipitation() + forecast.precipitationAmount());
        setUvIndex(std::max(uvIndex(), forecast.uvIndex()));
        setHumidity(std::max(humidity(), forecast.humidity()));
        setPressure(std::max(pressure(), forecast.pressure()));
        setMaxTemp(std::max(maxTemp(), forecast.temperature()));
        setMinTemp(std::min(minTemp(), forecast.temperature()));
    }

    d->hourlyWeatherForecast.push_back(forecast);
    return *this;
}

bool DailyWeatherForecast::operator==(
    const DailyWeatherForecast &forecast) const
{
    return (date() == forecast.date());
}

bool DailyWeatherForecast::operator<(const DailyWeatherForecast &forecast) const
{
    return date() < forecast.date();
}
}
