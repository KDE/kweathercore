/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "hourlyweatherforecast.h"
namespace KWeatherCore
{
class HourlyWeatherForecast::HourlyWeatherForecastPrivate
{
public:
    QDateTime date = QDateTime::currentDateTime();
    QString weatherDescription = QStringLiteral("Unknown");
    QString weatherIcon = QStringLiteral("weather-none-available");
    // weather icon without time of day
    QString neutralWeatherIcon = QStringLiteral("weather-none-available");
    QString symbolCode;
    double temperature = 0; // celsius
    double pressure = 0; // hPa
    WindDirection windDirection = WindDirection::E;
    double windSpeed = 0; // m/s
    double humidity = 0; // %
    double fog = 0; // %
    double uvIndex = 0; // 0-1
    double precipitationAmount = 0; // mm
};
HourlyWeatherForecast::HourlyWeatherForecast(const QDateTime &date)
    : d(std::make_unique<HourlyWeatherForecastPrivate>())
{
    d->date = date;
}
HourlyWeatherForecast::HourlyWeatherForecast(HourlyWeatherForecast &&other) = default;
HourlyWeatherForecast::~HourlyWeatherForecast() = default;

HourlyWeatherForecast::HourlyWeatherForecast()
    : d(std::make_unique<HourlyWeatherForecastPrivate>())
{
}

HourlyWeatherForecast::HourlyWeatherForecast(const HourlyWeatherForecast &other)
    : d(std::make_unique<HourlyWeatherForecastPrivate>())
{
    *d = *other.d;
}
HourlyWeatherForecast &HourlyWeatherForecast::operator=(HourlyWeatherForecast &&other) = default;
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
    HourlyWeatherForecast ret(QDateTime::fromString(obj[QStringLiteral("date")].toString(), Qt::ISODate));
    ret.setWeatherDescription(obj[QStringLiteral("weatherDescription")].toString());
    ret.setWeatherIcon(obj[QStringLiteral("weatherIcon")].toString());
    ret.setNeutralWeatherIcon(obj[QStringLiteral("neutralWeatherIcon")].toString());
    ret.setTemperature(obj[QStringLiteral("temperature")].toDouble());
    ret.setPressure(obj[QStringLiteral("pressure")].toDouble());
    ret.setWindDirection(static_cast<WindDirection>(obj[QStringLiteral("windDirection")].toInt()));
    ret.setWindSpeed(obj[QStringLiteral("windSpeed")].toDouble());
    ret.setHumidity(obj[QStringLiteral("humidity")].toDouble());
    ret.setFog(obj[QStringLiteral("fog")].toDouble());
    ret.setUvIndex(obj[QStringLiteral("uvIndex")].toDouble());
    ret.setPrecipitationAmount(obj[QStringLiteral("precipitationAmount")].toDouble());
    return ret;
}
const QDateTime &HourlyWeatherForecast::date() const
{
    return d->date;
}
void HourlyWeatherForecast::setDate(const QDateTime &date)
{
    d->date = std::move(date);
}
const QString &HourlyWeatherForecast::weatherDescription() const
{
    return d->weatherDescription;
}
void HourlyWeatherForecast::setWeatherDescription(const QString &weatherDescription)
{
    d->weatherDescription = weatherDescription;
}
const QString &HourlyWeatherForecast::weatherIcon() const
{
    return d->weatherIcon;
}
void HourlyWeatherForecast::setWeatherIcon(const QString &weatherIcon)
{
    d->weatherIcon = weatherIcon;
}
const QString &HourlyWeatherForecast::neutralWeatherIcon() const
{
    return d->neutralWeatherIcon;
}
void HourlyWeatherForecast::setNeutralWeatherIcon(const QString &neutralWeatherIcon)
{
    d->neutralWeatherIcon = neutralWeatherIcon;
}
const QString &HourlyWeatherForecast::symbolCode() const
{
    return d->symbolCode;
}
void HourlyWeatherForecast::setSymbolCode(const QString &symbolCode)
{
    d->symbolCode = symbolCode;
}
double HourlyWeatherForecast::temperature() const
{
    return d->temperature;
}
void HourlyWeatherForecast::setTemperature(double temperature)
{
    d->temperature = temperature;
}
double HourlyWeatherForecast::pressure() const
{
    return d->pressure;
}
void HourlyWeatherForecast::setPressure(double pressure)
{
    d->pressure = pressure;
}
WindDirection HourlyWeatherForecast::windDirection() const
{
    return d->windDirection;
}
QString HourlyWeatherForecast::windDirectionStr() const
{
    switch (windDirection()) {
    case WindDirection::E:
        return QStringLiteral("E");
    case WindDirection::N:
        return QStringLiteral("N");
    case WindDirection::NE:
        return QStringLiteral("NE");
    case WindDirection::NW:
        return QStringLiteral("NW");
    case WindDirection::S:
        return QStringLiteral("S");
    case WindDirection::SE:
        return QStringLiteral("SE");
    case WindDirection::SW:
        return QStringLiteral("SW");
    case WindDirection::W:
        return QStringLiteral("W");
    }
    return QStringLiteral("E");
}
void HourlyWeatherForecast::setWindDirection(WindDirection windDirection)
{
    d->windDirection = windDirection;
}
void HourlyWeatherForecast::setWindDirectionStr(const QString &windDirection)
{
    if (windDirection == QStringLiteral("E"))
        setWindDirection(WindDirection::E);
    else if (windDirection == QStringLiteral("N"))
        setWindDirection(WindDirection::N);
    else if (windDirection == QStringLiteral("NE"))
        setWindDirection(WindDirection::NE);
    else if (windDirection == QStringLiteral("NW"))
        setWindDirection(WindDirection::NW);
    else if (windDirection == QStringLiteral("S"))
        setWindDirection(WindDirection::S);
    else if (windDirection == QStringLiteral("SE"))
        setWindDirection(WindDirection::SE);
    else if (windDirection == QStringLiteral("SW"))
        setWindDirection(WindDirection::SW);
    else if (windDirection == QStringLiteral("W"))
        setWindDirection(WindDirection::W);
}
double HourlyWeatherForecast::windSpeed() const
{
    return d->windSpeed;
}
void HourlyWeatherForecast::setWindSpeed(double windSpeed)
{
    d->windSpeed = windSpeed;
}
double HourlyWeatherForecast::humidity() const
{
    return d->humidity;
}
void HourlyWeatherForecast::setHumidity(double humidity)
{
    d->humidity = humidity;
}
double HourlyWeatherForecast::fog() const
{
    return d->fog;
}
void HourlyWeatherForecast::setFog(double fog)
{
    d->fog = fog;
}
double HourlyWeatherForecast::uvIndex() const
{
    return d->uvIndex;
}
void HourlyWeatherForecast::setUvIndex(double uvIndex)
{
    d->uvIndex = uvIndex;
}
double HourlyWeatherForecast::precipitationAmount() const
{
    return d->precipitationAmount;
}
void HourlyWeatherForecast::setPrecipitationAmount(double precipitationAmount)
{
    d->precipitationAmount = precipitationAmount;
}
bool HourlyWeatherForecast::operator==(const KWeatherCore::HourlyWeatherForecast &rhs) const
{
    return (weatherDescription() == rhs.weatherDescription() && weatherIcon() == rhs.weatherIcon() && date() == rhs.date());
}

HourlyWeatherForecast &HourlyWeatherForecast::operator=(const HourlyWeatherForecast &other)
{
    *d = *other.d;
    return *this;
}
}
