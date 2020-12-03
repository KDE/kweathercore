#pragma once

#include <QSignalSpy>
#include <QTest>
#include <kweathercore/weatherforecastsource.h>
using namespace KWeatherCore;
class PendingWeatherForecastTest : public QObject
{
private Q_SLOTS:
    void initTestCase();
    void testFetch();

private:
    WeatherForecastSource d;
    PendingWeatherForecast *p;
    QSignalSpy *finished_spy;
    QSignalSpy *networkError_spy;
};
