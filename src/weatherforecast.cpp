/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "weatherforecast.h"
#include <QJsonArray>
namespace KWeatherCore
{
class WeatherForecast::WeatherForecastPrivate
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
WeatherForecast::WeatherForecast(const WeatherForecast &other)
    : QSharedData(QSharedData())
    , d(new WeatherForecastPrivate)
{
    *d = *other.d;
}
WeatherForecast::WeatherForecast(WeatherForecast &&other) = default;
WeatherForecast::~WeatherForecast() = default;
WeatherForecast &WeatherForecast::operator=(const WeatherForecast &other)
{
    *d = *other.d;
    return *this;
}
QJsonObject WeatherForecast::toJson() const
{
    QJsonObject obj;
    QJsonArray dayArray;
    for (auto d : dailyWeatherForecast()) {
        dayArray.append(d.toJson());
    }
    obj[QStringLiteral("day")] = dayArray;
    obj[QStringLiteral("lat")] = latitude();
    obj[QStringLiteral("lon")] = longitude();
    obj[QStringLiteral("timezone")] = timezone();
    return obj;
}
QExplicitlySharedDataPointer<WeatherForecast>
WeatherForecast::fromJson(QJsonObject obj)
{
    auto w = QExplicitlySharedDataPointer<WeatherForecast>(new WeatherForecast);
    std::vector<DailyWeatherForecast> dayVec;
    const auto &array = obj[QStringLiteral("day")].toArray();
    for (const auto &d : array) {
        dayVec.push_back(DailyWeatherForecast::fromJson(d.toObject()));
    }
    w->setDailyWeatherForecast(dayVec);
    w->setCoordinate(obj[QStringLiteral("lat")].toDouble(),
                     obj[QStringLiteral("lon")].toDouble());
    w->setTimezone(obj[QStringLiteral("timezone")].toString());
    return w;
}
const std::vector<DailyWeatherForecast> &
WeatherForecast::dailyWeatherForecast() const
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
void WeatherForecast::setDailyWeatherForecast(
    const std::vector<DailyWeatherForecast> &forecast)
{
    d->dailyWeatherForecast = forecast;
}
void WeatherForecast::setDailyWeatherForecast(
    std::vector<DailyWeatherForecast> &&forecast)
{
    d->dailyWeatherForecast = std::move(forecast);
}
void WeatherForecast::setSunriseForecast(const std::vector<Sunrise> &sunrise)
{
    int i = 0, range = sunrise.size();
    for (auto &day : d->dailyWeatherForecast) {
        if (i >= range)
            break;
        day.setSunrise(sunrise.at(i));
        ++i;
    }
}
void WeatherForecast::setSunriseForecast(std::vector<Sunrise> &&sunrise)
{
    int i = 0, range = sunrise.size();
    for (auto day : d->dailyWeatherForecast) {
        if (i >= range)
            break;
        // if on the same day, add sunrise to day
        if (day.date().daysTo(sunrise.at(i).sunRise().date()) == 0) {
            day.setSunrise(std::move(sunrise[i]));
            ++i;
        }
    }
}
WeatherForecast &
WeatherForecast::operator+=(const DailyWeatherForecast &forecast)
{
    for (int i = dailyWeatherForecast().size() - 1; i >= 0; --i) {
        if (dailyWeatherForecast().at(i) == forecast) {
            d->dailyWeatherForecast[i] += forecast;
            return *this;
        }
    }

    // if not find, append it at end
    d->dailyWeatherForecast.push_back(forecast);
    return *this;
}
WeatherForecast &WeatherForecast::operator+=(DailyWeatherForecast &&forecast)
{
    for (int i = dailyWeatherForecast().size() - 1; i >= 0; --i) {
        if (dailyWeatherForecast().at(i) == forecast) {
            d->dailyWeatherForecast[i] += forecast;
            return *this;
        }
    }

    // if not find, append it at end
    d->dailyWeatherForecast.push_back(std::move(forecast));
    return *this;
}
WeatherForecast &
WeatherForecast::operator+=(const HourlyWeatherForecast &forecast)
{
    for (int i = dailyWeatherForecast().size() - 1; i >= 0; --i) {
        if (dailyWeatherForecast().at(i).date().isValid() &&
            dailyWeatherForecast().at(i).date().daysTo(
                forecast.date().date()) == 0) {
            d->dailyWeatherForecast[i] += forecast;
            return *this;
        } else {
            break;
        }
    }

    // if not find, append it at end
    DailyWeatherForecast newDay;
    newDay += forecast;
    d->dailyWeatherForecast.push_back(std::move(newDay));
    return *this;
}
WeatherForecast &WeatherForecast::operator+=(HourlyWeatherForecast &&forecast)
{
    for (int i = dailyWeatherForecast().size() - 1; i >= 0; --i) {
        if (dailyWeatherForecast().at(i).date().daysTo(
                forecast.date().date()) == 0) {
            d->dailyWeatherForecast[i] += std::move(forecast);
            return *this;
        }
    }

    // if not find, append it at end
    auto newDay = DailyWeatherForecast();
    newDay += forecast;
    d->dailyWeatherForecast.push_back(std::move(newDay));
    return *this;
}

}
