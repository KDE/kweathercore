#pragma once
#include "reply_p.h"
#include "weatherforecast.h"

class QNetworkReply;
class QNetworkAccessManager;
namespace KWeatherCore
{
class PendingWeatherForecast;
class PendingWeatherForecastPrivate : public ReplyPrivate
{
public:
    PendingWeatherForecastPrivate(PendingWeatherForecast *qq);

    void parseWeatherForecastResults(QNetworkReply *ret);
    void parseTimezoneResult(const QString &timezone);

    void parseOneElement(const QJsonObject &obj, std::vector<HourlyWeatherForecast> &hourlyForecast);
    void getTimezone(double latitude, double longitude);
    bool isDayTime(const QDateTime &dt) const;
    void applySunriseToForecast();

    WeatherForecast forecast;
    PendingWeatherForecast *q = nullptr;
    bool hasTimezone = false;
    QString m_timezone;

    std::vector<HourlyWeatherForecast> hourlyForecast; // tmp hourly vector

    QDateTime m_expiresTime;
    QNetworkAccessManager *m_manager = nullptr;
};
}
