/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "dailyweatherforecast.h"
#include "kweathercore_p.h"
#include "pendingweatherforecast_p.h"
#include <QJsonArray>
namespace KWeatherCore
{
class DailyWeatherForecast::DailyWeatherForecastPrivate
{
public:
    bool isValid = true;

    double maxTemp = std::numeric_limits<double>::lowest();
    double minTemp = std::numeric_limits<double>::max();
    double precipitation = 0; // mm
    double uvIndex = 0; // 0-1
    double humidity = 0; // %
    double pressure = 0; // hPa
    QString weatherIcon = QStringLiteral("weather-none-available");
    QString weatherDescription = QStringLiteral("Unknown");
    QDate date;

    std::vector<HourlyWeatherForecast> hourlyWeatherForecast;
};
DailyWeatherForecast::~DailyWeatherForecast() = default;
DailyWeatherForecast::DailyWeatherForecast(DailyWeatherForecast &&other) = default;
DailyWeatherForecast &DailyWeatherForecast::operator=(DailyWeatherForecast &&other) = default;
DailyWeatherForecast::DailyWeatherForecast()
    : d(std::make_unique<DailyWeatherForecastPrivate>())
{
}
DailyWeatherForecast::DailyWeatherForecast(const QDate &date)
    : d(std::make_unique<DailyWeatherForecastPrivate>())
{
    d->date = date;
    d->isValid = false;
}
DailyWeatherForecast::DailyWeatherForecast(const DailyWeatherForecast &other)
    : d(std::make_unique<DailyWeatherForecastPrivate>())
{
    *d = *other.d;
}

QJsonObject DailyWeatherForecast::toJson()
{
    QJsonObject obj;
    QJsonArray hourlyArray;
    obj[QLatin1String("maxTemp")] = maxTemp();
    obj[QLatin1String("minTemp")] = minTemp();
    obj[QLatin1String("precipitation")] = precipitation();
    obj[QLatin1String("uvIndex")] = uvIndex();
    obj[QLatin1String("humidity")] = humidity();
    obj[QLatin1String("pressure")] = pressure();
    obj[QLatin1String("weatherIcon")] = weatherIcon();
    obj[QLatin1String("weatherDescription")] = weatherDescription();
    obj[QLatin1String("date")] = date().toString(Qt::ISODate);
    for (const auto &h : hourlyWeatherForecast()) {
        hourlyArray.append(h.toJson());
    }
    obj[QLatin1String("hourly")] = hourlyArray;
    return obj;
}
DailyWeatherForecast DailyWeatherForecast::fromJson(const QJsonObject &obj)
{
    DailyWeatherForecast ret(QDate::fromString(obj[QLatin1String("date")].toString(), Qt::ISODate));
    ret.setMaxTemp(obj[QLatin1String("maxTemp")].toDouble());
    ret.setMinTemp(obj[QLatin1String("minTemp")].toDouble());
    ret.setPrecipitation(obj[QLatin1String("precipitation")].toDouble());
    ret.setUvIndex(obj[QLatin1String("uvIndex")].toDouble());
    ret.setHumidity(obj[QLatin1String("humidity")].toDouble());
    ret.setPressure(obj[QLatin1String("pressure")].toDouble());
    ret.setWeatherIcon(obj[QLatin1String("weatherIcon")].toString());
    ret.setWeatherDescription(obj[QLatin1String("weatherDescription")].toString());
    std::vector<HourlyWeatherForecast> hourlyVec;
    auto array = obj[QLatin1String("hourly")].toArray();
    for (int i = 0; i < array.size(); i++) {
        hourlyVec.push_back(HourlyWeatherForecast::fromJson(array.at(i).toObject()));
    }
    ret.setHourlyWeatherForecast(std::move(hourlyVec));
    return ret;
}
bool DailyWeatherForecast::isValid() const
{
    return d->isValid;
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
void DailyWeatherForecast::setWeatherIcon(const QString &icon)
{
    d->weatherIcon = icon;
}
void DailyWeatherForecast::setWeatherDescription(const QString &description)
{
    d->weatherDescription = std::move(description);
}
void DailyWeatherForecast::setDate(const QDate &date)
{
    d->date = date;
}
void DailyWeatherForecast::setDate(const QDateTime &date)
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
QDateTime DailyWeatherForecast::dateTime() const
{
    return d->date.startOfDay();
}
const std::vector<HourlyWeatherForecast> &DailyWeatherForecast::hourlyWeatherForecast() const
{
    return d->hourlyWeatherForecast;
}
void DailyWeatherForecast::setHourlyWeatherForecast(std::vector<HourlyWeatherForecast> &&forecast)
{
    d->hourlyWeatherForecast = std::move(forecast);
}

DailyWeatherForecast &DailyWeatherForecast::operator+=(const HourlyWeatherForecast &forecast)
{
    if (isValid()) {
        setDate(forecast.date().date());
        setWeatherDescription(forecast.weatherDescription());
        setWeatherIcon(forecast.weatherIcon());
        d->isValid = false;
    }

    if (date().daysTo(forecast.date().date()) == 0) {
        // set description and icon if it is higher ranked
        if (KWeatherCorePrivate::weatherIconPriorityRank(forecast.neutralWeatherIcon()) >= KWeatherCorePrivate::weatherIconPriorityRank(weatherIcon())) {
            setWeatherDescription(KWeatherCorePrivate::resolveAPIWeatherDesc(forecast.symbolCode() + QStringLiteral("_neutral")).desc);
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

bool DailyWeatherForecast::operator==(const DailyWeatherForecast &forecast) const
{
    return (date() == forecast.date());
}

bool DailyWeatherForecast::operator<(const DailyWeatherForecast &forecast) const
{
    return date() < forecast.date();
}
DailyWeatherForecast &DailyWeatherForecast::operator=(const DailyWeatherForecast &other)
{
    *d = *other.d;
    return *this;
}
}

#include "moc_dailyweatherforecast.cpp"
