#pragma once
#include <QDate>
#include <QJsonObject>
namespace KWeatherCore
{
class DailyWeatherForecast
{
public:
    DailyWeatherForecast();
    DailyWeatherForecast(double maxTemp, double minTemp, double precipitation, double uvIndex, double humidity, double pressure, QString weatherIcon, QString weatherDescription, QDate date);

    QJsonObject toJson();
    static DailyWeatherForecast fromJson(QJsonObject obj);

    void setMaxTemp(double maxTemp);
    void setMinTemp(double minTemp);
    void setPrecipitation(double precipitation);
    void setUvIndex(double uvIndex);
    void setHumidity(double humidity);
    void setPressure(double pressure);
    void setWeatherIcon(QString icon);
    void setWeatherDescription(QString description);
    void setDate(QDate date);
    double maxTemp() const;
    double minTemp() const;
    double precipitation() const;
    double uvIndex() const;
    double humidity() const;
    double pressure() const;
    QString weatherIcon() const;
    QString weatherDescription() const;
    QDate date() const;

private:
    double m_maxTemp = 0;
    double m_minTemp = 0;
    double m_precipitation = 0; // mm
    double m_uvIndex = 0;       // 0-1
    double m_humidity = 0;      // %
    double m_pressure = 0;      // hPa
    QString m_weatherIcon;
    QString m_weatherDescription;
    QDate m_date;
};
}
