#pragma once
#include "locationqueryresult.h"
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

    void requestData(double latitude, double longitude, QString timezone = QString());
    void requestData(const LocationQueryResult &location);
Q_SIGNALS:
    void requestFinished(const WeatherForecast &forecast);
    void networkError();
private Q_SLOTS:
    void parseResults(QNetworkReply *reply);

private:
    WeatherForecastSourcePrivate *d = nullptr;

    void parseOneElement(const QJsonObject &obj, WeatherForecast &forecast);
};
}
