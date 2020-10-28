#pragma once
#include "weatherforecast.h"
#include <QObject>
class QNetworkReply;
namespace KWeatherCore
{
class PendingWeatherForecastPrivate;
class PendingWeatherForecast : public QObject
{
    Q_OBJECT
public:
    template<class T> PendingWeatherForecast(const QString &timezone, T &&sunrise);
    ~PendingWeatherForecast();
    WeatherForecast value();
    bool isFinished();

Q_SIGNALS:
    void finished();
    void networkError();
public Q_SLOTS:
    void parseResults(QNetworkReply *ret);

private:
    PendingWeatherForecastPrivate *d = nullptr;
    void parseOneElement(const QJsonObject &obj, QVector<HourlyWeatherForecast> &hourlyForecast);
};
}
