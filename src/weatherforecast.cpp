#include "weatherforecast.h"
namespace KWeatherCore
{
const QVector<DailyWeatherForecast> &WeatherForecast::dailyWeatherForecast() const
{
    return m_dailyWeatherForecast;
}
const QVector<HourlyWeatherForecast> &WeatherForecast::houlyWeatherForecast() const
{
    return m_hourlyWeatherForecast;
}

void WeatherForecast::setDailyWeatherForecast(const QVector<DailyWeatherForecast> &forecast)
{
    m_dailyWeatherForecast = forecast;
}

void WeatherForecast::setHourlyWeatherForecast(const QVector<HourlyWeatherForecast> &forecast)
{
    m_hourlyWeatherForecast = forecast;
}
}
