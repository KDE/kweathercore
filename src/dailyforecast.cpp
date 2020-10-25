#include "dailyforecast.h"

namespace KWeatherCore
{
DailyWeatherForecast::DailyWeatherForecast()
{
    m_weatherDescription = QStringLiteral("Unknown");
    m_weatherIcon = QStringLiteral("weather-none-available");
    m_date = QDate::currentDate();
}
DailyWeatherForecast::DailyWeatherForecast(double maxTemp, double minTemp, double precipitation, double uvIndex, double humidity, double pressure, QString weatherIcon, QString weatherDescription, QDate date)
    : m_maxTemp(maxTemp)
    , m_minTemp(minTemp)
    , m_precipitation(precipitation)
    , m_uvIndex(uvIndex)
    , m_humidity(humidity)
    , m_pressure(pressure)
    , m_weatherIcon(std::move(weatherIcon))
    , m_weatherDescription(std::move(weatherDescription)) {};

QJsonObject DailyWeatherForecast::toJson()
{
    QJsonObject obj;
    obj[QStringLiteral("maxTemp")] = maxTemp();
    obj[QStringLiteral("minTemp")] = minTemp();
    obj[QStringLiteral("precipitation")] = precipitation();
    obj[QStringLiteral("uvIndex")] = uvIndex();
    obj[QStringLiteral("humidity")] = humidity();
    obj[QStringLiteral("pressure")] = pressure();
    obj[QStringLiteral("weatherIcon")] = weatherIcon();
    obj[QStringLiteral("weatherDescription")] = weatherDescription();
    obj[QStringLiteral("date")] = date().toString(Qt::ISODate);
    return obj;
}

void DailyWeatherForecast::setMaxTemp(double maxTemp)
{
    m_maxTemp = maxTemp;
};
void DailyWeatherForecast::setMinTemp(double minTemp)
{
    m_minTemp = minTemp;
};
void DailyWeatherForecast::setPrecipitation(double precipitation)
{
    m_precipitation = precipitation;
};
void DailyWeatherForecast::setUvIndex(double uvIndex)
{
    m_uvIndex = uvIndex;
};
void DailyWeatherForecast::setHumidity(double humidity)
{
    m_humidity = humidity;
};
void DailyWeatherForecast::setPressure(double pressure)
{
    m_pressure = pressure;
};
void DailyWeatherForecast::setWeatherIcon(QString icon)
{
    m_weatherIcon = std::move(icon);
};
void DailyWeatherForecast::setWeatherDescription(QString description)
{
    m_weatherDescription = std::move(description);
};
void DailyWeatherForecast::setDate(QDate date)
{
    m_date = std::move(date);
};
double DailyWeatherForecast::maxTemp() const
{
    return m_maxTemp;
};
double DailyWeatherForecast::minTemp() const
{
    return m_minTemp;
};
double DailyWeatherForecast::precipitation() const
{
    return m_precipitation;
    ;
};
double DailyWeatherForecast::uvIndex() const
{
    return m_uvIndex;
};
double DailyWeatherForecast::humidity() const
{
    return m_humidity;
};
double DailyWeatherForecast::pressure() const
{
    return m_pressure;
};
QString DailyWeatherForecast::weatherIcon() const
{
    return m_weatherIcon;
};
QString DailyWeatherForecast::weatherDescription() const
{
    return m_weatherDescription;
};
QDate DailyWeatherForecast::date() const
{
    return m_date;
};
}
