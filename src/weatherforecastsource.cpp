#include "weatherforecastsource.h"
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
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
    connect(d->manager, &QNetworkAccessManager::finished, this, &WeatherForecastSource::parseResults);
}
WeatherForecastSource::~WeatherForecastSource()
{
    delete d;
}

void WeatherForecastSource::requestData(double latitude, double longitude)
{
    // query weather api
    QUrl url(QStringLiteral("https://api.met.no/weatherapi/locationforecast/2.0/complete"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), QString::number(latitude));
    query.addQueryItem(QStringLiteral("lon"), QString::number(longitude));

    url.setQuery(query);

    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    // see §Identification on https://api.met.no/conditions_service.html
    req.setHeader(QNetworkRequest::UserAgentHeader, QString(QCoreApplication::applicationName() + QLatin1Char(' ') + QCoreApplication::applicationVersion() + QLatin1String(" (kde-pim@kde.org)")));

    d->manager->get(req);
}

void WeatherForecastSource::parseResults(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        qWarning() << "network error when fetching forecast:" << reply->errorString();
        emit networkError();
        return;
    }
}
}
