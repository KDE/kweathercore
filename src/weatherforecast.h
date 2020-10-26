#pragma once
#include "dailyforecast.h"
#include "hourlyforecast.h"
namespace KWeatherCore
{
class WeatherForecast
{
public:
    WeatherForecast();
    WeatherForecast(QVector<DailyWeatherForecast> daily, QVector<HourlyWeatherForecast> hourly);
    const QVector<DailyWeatherForecast> &dailyWeatherForecast() const;
    QVector<DailyWeatherForecast> &dailyWeatherForecast();
    void setDailyWeatherForecast(const QVector<DailyWeatherForecast> &forecast);
    void setDailyWeatherForecast(QVector<DailyWeatherForecast> &&forecast);
    void setHourlyWeatherForecast(const QVector<HourlyWeatherForecast> &forecast);
    void setHourlyWeatherForecast(QVector<HourlyWeatherForecast> &&forecast);

    WeatherForecast &operator+=(const DailyWeatherForecast &forecast);
    WeatherForecast &operator+=(DailyWeatherForecast &&forecast);
    WeatherForecast &operator+=(const HourlyWeatherForecast &forecast);
    WeatherForecast &operator+=(HourlyWeatherForecast &&forecast);

private:
    QVector<DailyWeatherForecast> m_dailyWeatherForecast;
};
}
