#pragma once
#include "weatherforecast.h"
#include <QNetworkAccessManager>
#include <QObject>
class QNetworkReply;
namespace KWeatherCore
{
class WeatherForecastSource;
class SunriseSource;
class PendingWeatherForecast;
class PendingWeatherForecastPrivate : public QObject
{
    Q_OBJECT
public:
    PendingWeatherForecastPrivate(double latitude,
                                  double longitude,
                                  const QString &timezone,
                                  const QUrl &url,
                                  const std::vector<Sunrise> &sunrise,
                                  PendingWeatherForecast *parent = nullptr);
    PendingWeatherForecastPrivate(WeatherForecast data);
    WeatherForecast forecast;
    bool isFinished = false;

Q_SIGNALS:
    void networkError();
    void finished();
private Q_SLOTS:
    void parseWeatherForecastResults(QNetworkReply *ret);
    void parseSunriseResults();
    void parseTimezoneResult(const QString &timezone);

private:
    void parseOneElement(const QJsonObject &obj, std::vector<HourlyWeatherForecast> &hourlyForecast);
    void getTimezone(double latitude, double longitude);
    void getSunrise();
    void applySunriseToForecast();

    bool hasTimezone = false;
    bool hasSunrise = false;
    double m_latitude, m_longitude;
    QString m_timezone;

    std::vector<HourlyWeatherForecast> hourlyForecast; // tmp hourly vector

    SunriseSource *m_sunriseSource = nullptr;

    QDateTime m_expiresTime;
    QNetworkAccessManager m_manager;
};
}
