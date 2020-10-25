#pragma once
#include "locationqueryresult.h"
#include <QObject>
class QNetworkReply;
namespace KWeatherCore
{
class WeatherForecastPrivate;
class WeatherForecast : public QObject
{
    Q_OBJECT
public:
    WeatherForecast(QObject *parent = nullptr);
    ~WeatherForecast();

    void requestData(double latitude, double longitude);
    void requestData(const LocationQueryResult &location);
Q_SIGNALS:
    void requestFinished(const QJsonObject &obj);
    void networkError();
private Q_SLOTS:
    void parseResults(QNetworkReply *reply);

private:
    WeatherForecastPrivate *d = nullptr;
};
}
