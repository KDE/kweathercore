#pragma once
#include "weatherforecast.h"
#include <QExplicitlySharedDataPointer>
#include <QObject>
class QNetworkReply;
namespace KWeatherCore
{
class PendingWeatherForecastPrivate;
class PendingWeatherForecast : public QObject
{
    Q_OBJECT
public:
    WeatherForecast value();
    bool isFinished();

Q_SIGNALS:
    void finished();
    void networkError();

protected:
    QExplicitlySharedDataPointer<PendingWeatherForecastPrivate> d;
    PendingWeatherForecast(PendingWeatherForecastPrivate *dd);
};
}
