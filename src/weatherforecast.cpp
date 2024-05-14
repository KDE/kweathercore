/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "weatherforecast.h"
#include <QJsonArray>
#include <QJsonObject>

namespace KWeatherCore
{
class WeatherForecast::WeatherForecastPrivate : public QSharedData
{
public:
    std::vector<DailyWeatherForecast> dailyWeatherForecast;
    QString timezone;
    double latitude, longitude;
    QDateTime createdTime = QDateTime::currentDateTime();
};

WeatherForecast::WeatherForecast()
    : d(new WeatherForecastPrivate)
{
}

WeatherForecast::WeatherForecast(const WeatherForecast &) = default;
WeatherForecast::WeatherForecast(WeatherForecast &&) noexcept = default;

WeatherForecast &WeatherForecast::operator=(const WeatherForecast &) = default;
WeatherForecast &WeatherForecast::operator=(WeatherForecast &&) noexcept = default;

WeatherForecast::~WeatherForecast() = default;

QJsonObject WeatherForecast::toJson() const
{
    QJsonObject obj;
    QJsonArray dayArray;
    for (auto d : dailyWeatherForecast()) {
        dayArray.append(d.toJson());
    }
    obj[QLatin1String("day")] = dayArray;
    obj[QLatin1String("lat")] = latitude();
    obj[QLatin1String("lon")] = longitude();
    obj[QLatin1String("timezone")] = timezone();
    obj[QLatin1String("createdTime")] = createdTime().toString(Qt::ISODate);
    return obj;
}

WeatherForecast WeatherForecast::fromJson(const QJsonObject &obj)
{
    WeatherForecast w;
    std::vector<DailyWeatherForecast> dayVec;
    const auto &array = obj[QLatin1String("day")].toArray();
    for (const auto &d : array) {
        dayVec.push_back(DailyWeatherForecast::fromJson(d.toObject()));
    }
    w.setDailyWeatherForecast(std::move(dayVec));
    w.setCoordinate(obj[QLatin1String("lat")].toDouble(), obj[QLatin1String("lon")].toDouble());
    w.setTimezone(obj[QLatin1String("timezone")].toString());
    w.setCreatedTime(QDateTime::fromString(obj[QLatin1String("createdTime")].toString(), Qt::ISODate));
    return w;
}
const std::vector<DailyWeatherForecast> &WeatherForecast::dailyWeatherForecast() const
{
    return d->dailyWeatherForecast;
}
double WeatherForecast::latitude() const
{
    return d->latitude;
}
double WeatherForecast::longitude() const
{
    return d->longitude;
}
const QDateTime &WeatherForecast::createdTime() const
{
    return d->createdTime;
}
const QString &WeatherForecast::timezone() const
{
    return d->timezone;
}
void WeatherForecast::setCoordinate(double latitude, double longitude)
{
    d->latitude = latitude;
    d->longitude = longitude;
}
void WeatherForecast::setTimezone(QString timezone)
{
    d->timezone = std::move(timezone);
}
void WeatherForecast::setDailyWeatherForecast(std::vector<DailyWeatherForecast> &&forecast)
{
    d->dailyWeatherForecast = std::move(forecast);
}
WeatherForecast &WeatherForecast::operator+=(HourlyWeatherForecast &&forecast)
{
    for (int i = dailyWeatherForecast().size() - 1; i >= 0; --i) {
        if (dailyWeatherForecast().at(i).date() == forecast.date().date()) {
            d->dailyWeatherForecast[i] += std::move(forecast);
            return *this;
        }
    }

    // if not find, append it at end
    auto newDay = DailyWeatherForecast();
    newDay += std::move(forecast);
    d->dailyWeatherForecast.push_back(std::move(newDay));
    return *this;
}
void WeatherForecast::setCreatedTime(const QDateTime &date)
{
    d->createdTime = date;
}
}
