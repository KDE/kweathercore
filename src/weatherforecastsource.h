#pragma once
#include "dailyforecast.h"
#include "hourlyforecast.h"
#include "locationqueryresult.h"
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

    void requestData(double latitude, double longitude);
    void requestData(const LocationQueryResult &location);
Q_SIGNALS:
    void requestFinished(const QVector<>);
    void networkError();
private Q_SLOTS:
    void parseResults(QNetworkReply *reply);

private:
    WeatherForecastSourcePrivate *d = nullptr;
};
}
