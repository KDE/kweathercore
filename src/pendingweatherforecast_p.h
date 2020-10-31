#pragma once
#include "weatherforecast.h"
#include <KLocalizedString>
#include <QObject>
class QNetworkReply;
namespace KWeatherCore
{
class WeatherForecastSource;
class SunriseSource;
class PendingWeatherForecastPrivate : public QObject
{
    Q_OBJECT
public:
    template<class T> PendingWeatherForecastPrivate(double latitude, double longitude, const QString &timezone, T &&sunrise);
    QExplicitlySharedDataPointer<WeatherForecast> forecast;
    QString timezone;

Q_SIGNALS:
    void networkError();
    void finished();
public Q_SLOTS:
    void parseWeatherForecastResults(QNetworkReply *ret);
    void parseSunriseResults();
    void parseTimezoneResult(QString timezone);

private:
    void parseOneElement(const QJsonObject &obj, QVector<HourlyWeatherForecast> &hourlyForecast);
    void getTimezone(double latitude, double longitude);
    void getSunrise(double latitude, double longitude, int offset);
    void applySunriseToForecast();

    bool hasTimezone = false;
    bool hasSunrise = false;
    double m_latitude, m_longitude;
    QString m_timezone;

    QVector<HourlyWeatherForecast> hourlyForecast; // tmp hourly vector

    SunriseSource *m_sunriseSource = nullptr;

    // rank weather (for what best describes the day overall)
    static const QHash<QString, int> rank;
    struct ResolvedWeatherDesc {
        QString icon = QStringLiteral("weather-none-available"), desc = QStringLiteral("Unknown");
        ResolvedWeatherDesc() = default;
        ResolvedWeatherDesc(QString icon, QString desc)
        {
            this->icon = icon;
            this->desc = desc;
        }
    };

    // https://api.met.no/weatherapi/weathericon/2.0/legends
    static const QMap<QString, ResolvedWeatherDesc> apiDescMap;
};
}
