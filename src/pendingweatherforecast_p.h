#pragma once
#include "weatherforecast.h"
#include <QObject>
class QNetworkReply;
namespace KWeatherCore
{
class WeatherForecastSource;
class SunriseSource;
class PendingWeatherForecastPrivate : public QObject
{
    Q_OBJECT
public:
    PendingWeatherForecastPrivate(double latitude, double longitude, const QString &timezone, const QVector<Sunrise> &sunrise);
    PendingWeatherForecastPrivate(double latitude, double longitude, const QString &timezone, QVector<Sunrise> &&sunrise);
    QExplicitlySharedDataPointer<WeatherForecast> forecast;
    QString timezone;

Q_SIGNALS:
    void networkError();
    void finished();
public Q_SLOTS:
    void parseWeatherForecastResults(QNetworkReply *ret);
    void parseSunriseResults();
    void parseTimezoneResult(QString timezone);

private:
    void parseOneElement(const QJsonObject &obj, QVector<HourlyWeatherForecast> &hourlyForecast);
    void getTimezone(double latitude, double longitude);
    void getSunrise(double latitude, double longitude, int offset);
    void applySunriseToForecast();

    bool hasTimezone = false;
    bool hasSunrise = false;
    double m_latitude, m_longitude;
    QString m_timezone;

    QVector<HourlyWeatherForecast> hourlyForecast; // tmp hourly vector

    SunriseSource *m_sunriseSource = nullptr;
};
}
