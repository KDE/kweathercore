/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "weatherforecast.h"
#include <QJsonArray>
namespace KWeatherCore
{
WeatherForecast::WeatherForecast()
{
    m_createdTime = QDateTime::currentDateTime();
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
QExplicitlySharedDataPointer<WeatherForecast> WeatherForecast::fromJson(QJsonObject obj)
{
    auto w = QExplicitlySharedDataPointer<WeatherForecast>(new WeatherForecast);
    QVector<DailyWeatherForecast> dayVec;
    for (auto d : obj[QStringLiteral("day")].toArray()) {
        dayVec.append(DailyWeatherForecast::fromJson(d.toObject()));
    }
    w->setDailyWeatherForecast(dayVec);
    w->setCoordinate(obj[QStringLiteral("lat")].toDouble(), obj[QStringLiteral("lon")].toDouble());
    w->setTimezone(obj[QStringLiteral("timezone")].toString());
    return w;
}
const QDateTime &WeatherForecast::createdTime() const
{
    return m_createdTime;
}
double WeatherForecast::latitude() const
{
    return m_latitude;
}
double WeatherForecast::longitude() const
{
    return m_longitude;
}
const QVector<DailyWeatherForecast> &WeatherForecast::dailyWeatherForecast() const
{
    return m_dailyWeatherForecast;
}
QVector<DailyWeatherForecast> &WeatherForecast::dailyWeatherForecast()
{
    return m_dailyWeatherForecast;
}
const QString &WeatherForecast::timezone() const
{
    return m_timezone;
}
void WeatherForecast::setCoordinate(double latitude, double longitude)
{
    m_latitude = latitude;
    m_longitude = longitude;
}
void WeatherForecast::setTimezone(const QString &timezone)
{
    m_timezone = timezone;
}
void WeatherForecast::setTimezone(QString &&timezone)
{
    m_timezone = std::move(timezone);
}
void WeatherForecast::setDailyWeatherForecast(const QVector<DailyWeatherForecast> &forecast)
{
    m_dailyWeatherForecast = forecast;
}
void WeatherForecast::setDailyWeatherForecast(QVector<DailyWeatherForecast> &&forecast)
{
    m_dailyWeatherForecast = std::move(forecast);
}
void WeatherForecast::setSunriseForecast(const QVector<Sunrise> &sunrise)
{
    int i = 0, range = sunrise.size();
    for (auto day : m_dailyWeatherForecast) {
        if (i >= range)
            break;
        // if on the same day, add sunrise to day
        if (day.date().daysTo(sunrise.at(i).sunRise().date()) == 0) {
            day.setSunrise(sunrise.at(i));
            ++i;
        }
    }
}
void WeatherForecast::setSunriseForecast(QVector<Sunrise> &&sunrise)
{
    int i = 0, range = sunrise.size();
    for (auto day : m_dailyWeatherForecast) {
        if (i >= range)
            break;
        // if on the same day, add sunrise to day
        if (day.date().daysTo(sunrise.at(i).sunRise().date()) == 0) {
            day.setSunrise(std::move(sunrise[i]));
            ++i;
        }
    }
}
WeatherForecast &WeatherForecast::operator+=(const DailyWeatherForecast &forecast)
{
    for (int i = dailyWeatherForecast().size() - 1; i >= 0; --i) {
        if (dailyWeatherForecast().at(i) == forecast) {
            dailyWeatherForecast()[i] += forecast;
            return *this;
        }
    }

    // if not find, append it at end
    dailyWeatherForecast().append(forecast);
    return *this;
};
WeatherForecast &WeatherForecast::operator+=(DailyWeatherForecast &&forecast)
{
    for (int i = dailyWeatherForecast().size() - 1; i >= 0; --i) {
        if (dailyWeatherForecast().at(i) == forecast) {
            dailyWeatherForecast()[i] += forecast;
            return *this;
        }
    }

    // if not find, append it at end
    dailyWeatherForecast().append(std::move(forecast));
    return *this;
};
WeatherForecast &WeatherForecast::operator+=(const HourlyWeatherForecast &forecast)
{
    for (int i = dailyWeatherForecast().size() - 1; i >= 0; --i) {
        if (dailyWeatherForecast().at(i).date().isValid() && dailyWeatherForecast().at(i).date().daysTo(forecast.date().date()) == 0) {
            dailyWeatherForecast()[i] += forecast;
            return *this;
        } else {
            break;
        }
    }

    // if not find, append it at end
    auto newDay = DailyWeatherForecast();
    newDay += forecast;
    dailyWeatherForecast().append(std::move(newDay));
    return *this;
};
WeatherForecast &WeatherForecast::operator+=(HourlyWeatherForecast &&forecast)
{
    for (int i = dailyWeatherForecast().size() - 1; i >= 0; --i) {
        if (dailyWeatherForecast().at(i).date().daysTo(forecast.date().date()) == 0) {
            dailyWeatherForecast()[i] += std::move(forecast);
            return *this;
        }
    }

    // if not find, append it at end
    auto newDay = DailyWeatherForecast();
    newDay += forecast;
    dailyWeatherForecast().append(std::move(newDay));
    return *this;
};

}
