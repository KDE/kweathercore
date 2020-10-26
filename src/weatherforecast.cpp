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
void WeatherForecast::setDailyWeatherForecast(QVector<DailyWeatherForecast> &&forecast)
{
    m_dailyWeatherForecast = forecast;
}

void WeatherForecast::setHourlyWeatherForecast(const QVector<HourlyWeatherForecast> &forecast)
{
    m_hourlyWeatherForecast = forecast;
}

void WeatherForecast::setHourlyWeatherForecast(QVector<HourlyWeatherForecast> &&forecast)
{
    m_hourlyWeatherForecast = forecast;
}
WeatherForecast &WeatherForecast::operator+(const DailyWeatherForecast &forecast) {};
WeatherForecast &WeatherForecast::operator+(DailyWeatherForecast &&forecast) {};
WeatherForecast &WeatherForecast::operator+(const HourlyWeatherForecast &forecast) {};
WeatherForecast &WeatherForecast::operator+(HourlyWeatherForecast &&forecast) {};
WeatherForecast &WeatherForecast::operator+=(const DailyWeatherForecast &forecast) {};
WeatherForecast &WeatherForecast::operator+=(DailyWeatherForecast &&forecast) {};
WeatherForecast &WeatherForecast::operator+=(const HourlyWeatherForecast &forecast) {};
WeatherForecast &WeatherForecast::operator+=(HourlyWeatherForecast &&forecast) {};
WeatherForecast &WeatherForecast::operator+(const WeatherForecast &forecast) {};
WeatherForecast &WeatherForecast::operator+(WeatherForecast &&forecast) {};
WeatherForecast &WeatherForecast::operator+=(const WeatherForecast &forecast) {};
WeatherForecast &WeatherForecast::operator+=(WeatherForecast &&forecast) {};

}
