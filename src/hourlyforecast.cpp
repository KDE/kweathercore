/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "hourlyforecast.h"
#include <KLocalizedString>
namespace KWeatherCore
{
HourlyWeatherForecast::HourlyWeatherForecast()
{
    m_date = QDateTime::currentDateTime();
    m_weatherDescription = i18n("Unknown");
    m_weatherIcon = QStringLiteral("weather-none-available");
    m_neutralWeatherIcon = QStringLiteral("weather-none-available");
}

HourlyWeatherForecast::HourlyWeatherForecast(QDateTime date,
                                             QString weatherDescription,
                                             QString weatherIcon,
                                             QString neutralWeatherIcon,
                                             double temperature,
                                             double pressure,
                                             WindDirection windDirection,
                                             double windSpeed,
                                             double humidity,
                                             double fog,
                                             double uvIndex,
                                             double precipitationAmount)
    : m_date(std::move(date))
    , m_weatherDescription(std::move(weatherDescription))
    , m_weatherIcon(std::move(weatherIcon))
    , m_neutralWeatherIcon(std::move(neutralWeatherIcon))
    , m_temperature(temperature)
    , m_pressure(pressure)
    , m_windDirection(windDirection)
    , m_windSpeed(windSpeed)
    , m_humidity(humidity)
    , m_fog(fog)
    , m_uvIndex(uvIndex)
    , m_precipitationAmount(precipitationAmount)

{
}
QJsonObject HourlyWeatherForecast::toJson() const
{
    QJsonObject obj;
    obj[QStringLiteral("date")] = date().toString(Qt::ISODate);
    obj[QStringLiteral("weatherDescription")] = weatherDescription();
    obj[QStringLiteral("weatherIcon")] = weatherIcon();
    obj[QStringLiteral("neutralWeatherIcon")] = neutralWeatherIcon();
    obj[QStringLiteral("temperature")] = temperature();
    obj[QStringLiteral("pressure")] = pressure();
    obj[QStringLiteral("windDirection")] = static_cast<int>(windDirection());
    obj[QStringLiteral("windSpeed")] = windSpeed();
    obj[QStringLiteral("humidity")] = humidity();
    obj[QStringLiteral("fog")] = fog();
    obj[QStringLiteral("uvIndex")] = uvIndex();
    obj[QStringLiteral("precipitationAmount")] = precipitationAmount();
    return obj;
}
HourlyWeatherForecast HourlyWeatherForecast::fromJson(QJsonObject obj)
{
    return HourlyWeatherForecast(QDateTime::fromString(obj[QStringLiteral("date")].toString(), Qt::ISODate),
                                 obj[QStringLiteral("weatherDescription")].toString(),
                                 obj[QStringLiteral("weatherIcon")].toString(),
                                 obj[QStringLiteral("neutralWeatherIcon")].toString(),
                                 obj[QStringLiteral("temperature")].toDouble(),
                                 obj[QStringLiteral("pressure")].toDouble(),
                                 static_cast<WindDirection>(obj[QStringLiteral("windDirection")].toInt()),
                                 obj[QStringLiteral("windSpeed")].toDouble(),
                                 obj[QStringLiteral("humidity")].toDouble(),
                                 obj[QStringLiteral("fog")].toDouble(),
                                 obj[QStringLiteral("uvIndex")].toDouble(),
                                 obj[QStringLiteral("precipitationAmount")].toDouble());
}

const QDateTime &HourlyWeatherForecast::date() const
{
    return m_date;
}
void HourlyWeatherForecast::setDate(QDateTime date)
{
    m_date = std::move(date);
}
const QString &HourlyWeatherForecast::weatherDescription() const
{
    return m_weatherDescription;
}
void HourlyWeatherForecast::setWeatherDescription(const QString &weatherDescription)
{
    m_weatherDescription = std::move(weatherDescription);
}
const QString &HourlyWeatherForecast::weatherIcon() const
{
    return m_weatherIcon;
}
void HourlyWeatherForecast::setWeatherIcon(const QString &weatherIcon)
{
    m_weatherIcon = std::move(weatherIcon);
}
const QString &HourlyWeatherForecast::neutralWeatherIcon() const
{
    return m_neutralWeatherIcon;
}
void HourlyWeatherForecast::setNeutralWeatherIcon(QString neutralWeatherIcon)
{
    m_neutralWeatherIcon = std::move(neutralWeatherIcon);
}
const QString &HourlyWeatherForecast::symbolCode() const
{
    return m_symbolCode;
}
void HourlyWeatherForecast::setSymbolCode(QString symbolCode)
{
    m_symbolCode = std::move(symbolCode);
}
double HourlyWeatherForecast::temperature() const
{
    return m_temperature;
}
void HourlyWeatherForecast::setTemperature(double temperature)
{
    m_temperature = temperature;
}
double HourlyWeatherForecast::pressure() const
{
    return m_pressure;
}
void HourlyWeatherForecast::setPressure(double pressure)
{
    m_pressure = pressure;
}
WindDirection HourlyWeatherForecast::windDirection() const
{
    return m_windDirection;
}
void HourlyWeatherForecast::setWindDirection(WindDirection windDirection)
{
    m_windDirection = windDirection;
}
double HourlyWeatherForecast::windSpeed() const
{
    return m_windSpeed;
}
void HourlyWeatherForecast::setWindSpeed(double windSpeed)
{
    m_windSpeed = windSpeed;
}
double HourlyWeatherForecast::humidity() const
{
    return m_humidity;
}
void HourlyWeatherForecast::setHumidity(double humidity)
{
    m_humidity = humidity;
}
double HourlyWeatherForecast::fog() const
{
    return m_fog;
}
void HourlyWeatherForecast::setFog(double fog)
{
    m_fog = fog;
}
double HourlyWeatherForecast::uvIndex() const
{
    return m_uvIndex;
}
void HourlyWeatherForecast::setUvIndex(double uvIndex)
{
    m_uvIndex = uvIndex;
}
double HourlyWeatherForecast::precipitationAmount() const
{
    return m_precipitationAmount;
}
void HourlyWeatherForecast::setPrecipitationAmount(double precipitationAmount)
{
    m_precipitationAmount = precipitationAmount;
}
bool HourlyWeatherForecast::operator==(const KWeatherCore::HourlyWeatherForecast &rhs) const
{
    return (weatherDescription() == rhs.weatherDescription() && weatherIcon() == rhs.weatherIcon() && date() == rhs.date() && qAbs(temperature() - rhs.temperature()) < 0.01);
}
}
