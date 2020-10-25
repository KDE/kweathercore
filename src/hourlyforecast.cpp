#include "hourlyforecast.h"
#include <QJsonObject>
namespace KWeatherCore
{
HourlyWeatherForecast::HourlyWeatherForecast()
{
    m_date = QDateTime::currentDateTime();
    m_weatherDescription = QStringLiteral("Unknown");
    m_weatherIcon = QStringLiteral("weather-none-available");
    m_neutralWeatherIcon = QStringLiteral("weather-none-available");
}

HourlyWeatherForecast::HourlyWeatherForecast(QDateTime date,
                                             QString weatherDescription,
                                             QString weatherIcon,
                                             QString neutralWeatherIcon,
                                             float temperature,
                                             float pressure,
                                             WindDirection windDirection,
                                             float windSpeed,
                                             float humidity,
                                             float fog,
                                             float uvIndex,
                                             float precipitationAmount)
    : m_date(std::move(date))
    , m_weatherDescription(std::move(weatherDescription))
    , m_weatherIcon(std::move(weatherIcon))
    , m_neutralWeatherIcon(std::move(neutralWeatherIcon))
    , m_temperature(temperature)
    , m_pressure(pressure)
    , m_windDirection(windDirection)
    , m_windSpeed(windSpeed)
    , m_humidity(humidity)
    , m_fog(fog)
    , m_uvIndex(uvIndex)
    , m_precipitationAmount(precipitationAmount)

{
}
QJsonObject HourlyWeatherForecast::toJson()
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
    return HourlyWeatherForecast(QDateTime::fromString(obj[QStringLiteral("date")].toString(), Qt::ISODate),
                                 obj[QStringLiteral("weatherDescription")].toString(),
                                 obj[QStringLiteral("weatherIcon")].toString(),
                                 obj[QStringLiteral("neutralWeatherIcon")].toString(),
                                 obj[QStringLiteral("temperature")].toDouble(),
                                 obj[QStringLiteral("pressure")].toDouble(),
                                 static_cast<WindDirection>(obj[QStringLiteral("windDirection")].toInt()),
                                 obj[QStringLiteral("windSpeed")].toDouble(),
                                 obj[QStringLiteral("humidity")].toDouble(),
                                 obj[QStringLiteral("fog")].toDouble(),
                                 obj[QStringLiteral("uvIndex")].toDouble(),
                                 obj[QStringLiteral("precipitationAmount")].toDouble());
}

QDateTime HourlyWeatherForecast::date() const
{
    return m_date;
};
void HourlyWeatherForecast::setDate(QDateTime date)
{
    m_date = std::move(date);
};
QString HourlyWeatherForecast::weatherDescription() const
{
    return m_weatherDescription;
};
void HourlyWeatherForecast::setWeatherDescription(const QString &weatherDescription)
{
    m_weatherDescription = std::move(weatherDescription);
};
QString HourlyWeatherForecast::weatherIcon() const
{
    return m_weatherIcon;
};
void HourlyWeatherForecast::setWeatherIcon(const QString &weatherIcon)
{
    m_weatherIcon = std::move(weatherIcon);
};
QString HourlyWeatherForecast::neutralWeatherIcon() const
{
    return m_neutralWeatherIcon;
};
void HourlyWeatherForecast::setNeutralWeatherIcon(QString neutralWeatherIcon)
{
    m_neutralWeatherIcon = std::move(neutralWeatherIcon);
};
QString HourlyWeatherForecast::symbolCode() const
{
    return m_symbolCode;
};
void HourlyWeatherForecast::setSymbolCode(QString symbolCode)
{
    m_symbolCode = std::move(symbolCode);
};
float HourlyWeatherForecast::temperature() const
{
    return m_temperature;
};
void HourlyWeatherForecast::setTemperature(float temperature)
{
    m_temperature = temperature;
};
float HourlyWeatherForecast::pressure() const
{
    return m_pressure;
};
void HourlyWeatherForecast::setPressure(float pressure)
{
    m_pressure = pressure;
};
WindDirection HourlyWeatherForecast::windDirection() const
{
    return m_windDirection;
};
void HourlyWeatherForecast::setWindDirection(WindDirection windDirection)
{
    m_windDirection = windDirection;
};
float HourlyWeatherForecast::windSpeed() const
{
    return m_windSpeed;
};
void HourlyWeatherForecast::setWindSpeed(float windSpeed)
{
    m_windSpeed = windSpeed;
};
float HourlyWeatherForecast::humidity() const
{
    return m_humidity;
};
void HourlyWeatherForecast::setHumidity(float humidity)
{
    m_humidity = humidity;
};
float HourlyWeatherForecast::fog() const
{
    return m_fog;
};
void HourlyWeatherForecast::setFog(float fog)
{
    m_fog = fog;
};
float HourlyWeatherForecast::uvIndex() const
{
    return m_uvIndex;
};
void HourlyWeatherForecast::setUvIndex(float uvIndex)
{
    m_uvIndex = uvIndex;
};
float HourlyWeatherForecast::precipitationAmount() const
{
    return m_precipitationAmount;
};
void HourlyWeatherForecast::setPrecipitationAmount(float precipitationAmount)
{
    m_precipitationAmount = precipitationAmount;
};
}
