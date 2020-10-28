#pragma once
#include "locationqueryresult.h"
#include "pendingweatherforecast.h"
#include "weatherforecast.h"
#include <QObject>
class QNetworkReply;
namespace KWeatherCore
{
class WeatherForecastSourcePrivate;
class WeatherForecastSource : public QObject
{
    Q_OBJECT
public:
    WeatherForecastSource(QObject *parent = nullptr);
    ~WeatherForecastSource();

    PendingWeatherForecast *requestData(double latitude, double longitude, QString timezone = QString());
    PendingWeatherForecast *requestData(const LocationQueryResult &location);
Q_SIGNALS:
    void networkError();

private:
    WeatherForecastSourcePrivate *d = nullptr;
};
}
