#include "weatherforecastsource.h"
#include "geotimezone.h"
#include "weatherforecast.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
namespace KWeatherCore
{
class WeatherForecastSourcePrivate
{
public:
    QNetworkAccessManager *manager = nullptr;
};

WeatherForecastSource::WeatherForecastSource(QObject *parent)
    : QObject(parent)
    , d(new WeatherForecastSourcePrivate)
{
    d->manager = new QNetworkAccessManager(this);
}
WeatherForecastSource::~WeatherForecastSource()
{
    delete d;
}

PendingWeatherForecast *WeatherForecastSource::requestData(double latitude, double longitude, QString timezone)
{
    auto pf = new PendingWeatherForecast(timezone);
    // query weather api
    QUrl url(QStringLiteral("https://api.met.no/weatherapi/locationforecast/2.0/complete"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), QString::number(latitude));
    query.addQueryItem(QStringLiteral("lon"), QString::number(longitude));

    url.setQuery(query);

    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    // see §Identification on https://api.met.no/conditions_service.html
    req.setHeader(QNetworkRequest::UserAgentHeader, QString(QCoreApplication::applicationName() + QLatin1Char(' ') + QCoreApplication::applicationVersion() + QStringLiteral(" (kde-pim@kde.org)")));

    auto reply = d->manager->get(req);
    connect(reply, &QNetworkReply::finished, pf, &PendingWeatherForecast::parseResults);

    return pf;
}
}
