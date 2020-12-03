#include "pendingweatherforecasttest.h"

void PendingWeatherForecastTest::initTestCase()
{
    p = d.requestData(50, 50);

    finished_spy = new QSignalSpy(p, &PendingWeatherForecast::finished);
    networkError_spy = new QSignalSpy(p, &PendingWeatherForecast::networkError);
}

void PendingWeatherForecastTest::testFetch()
{
    auto ret = finished_spy->wait(30000);

    if (ret) {
        QVERIFY(p->value()->dailyWeatherForecast().size() > 0);
    } else {
        QVERIFY(networkError_spy->size() > 0);
    }
}
