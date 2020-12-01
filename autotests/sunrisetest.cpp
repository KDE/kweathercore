#include "sunrisetest.h"

void SunriseTest::testJson()
{
    auto date = QDateTime::currentDateTime();
    d1 = Sunrise(date, date.addSecs(3600 * 12), date.addSecs(3600 * 13), date.addSecs(3600 * 23), {date, 10}, {date, 10}, {date, 10}, {date, 10}, 0.5);
    d2 = Sunrise::fromJson(d1.toJson());
    QCOMPARE(d2.toJson(), d1.toJson());
}
QTEST_MAIN(SunriseTest)
