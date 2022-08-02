#pragma once
#include "metnoparser_p.h"
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

    void getTimezone(double latitude, double longitude);
    bool isDayTime(const QDateTime &dt) const;

    MetNoParser parser;
    PendingWeatherForecast *q = nullptr;
    bool hasTimezone = false;
    QString m_timezone;

    QDateTime m_expiresTime;
    QNetworkAccessManager *m_manager = nullptr;
};
}
