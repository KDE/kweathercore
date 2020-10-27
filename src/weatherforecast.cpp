#include "weatherforecast.h"
namespace KWeatherCore
{
const QVector<DailyWeatherForecast> &WeatherForecast::dailyWeatherForecast() const
{
    return m_dailyWeatherForecast;
}

void WeatherForecast::setDailyWeatherForecast(const QVector<DailyWeatherForecast> &forecast)
{
    m_dailyWeatherForecast = forecast;
}
void WeatherForecast::setDailyWeatherForecast(QVector<DailyWeatherForecast> &&forecast)
{
    m_dailyWeatherForecast = std::move(forecast);
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
        if (dailyWeatherForecast().at(i).date().daysTo(forecast.date().date()) == 0) {
            dailyWeatherForecast()[i] += forecast;
            return *this;
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
