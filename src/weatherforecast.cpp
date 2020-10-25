#include "weatherforecast.h"
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
namespace KWeatherCore
{
class WeatherForecastPrivate
{
public:
    QNetworkAccessManager *manager = nullptr;
};

WeatherForecast::WeatherForecast(QObject *parent)
    : QObject(parent)
    , d(new WeatherForecastPrivate)
{
    d->manager = new QNetworkAccessManager(this);
    connect(d->manager, &QNetworkAccessManager::finished, this, &WeatherForecast::parseResults);
}
WeatherForecast::~WeatherForecast()
{
    delete d;
}

void WeatherForecast::requestData(double latitude, double longitude)
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

void WeatherForecast::parseResults(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        qWarning() << "network error when fetching forecast:" << reply->errorString();
        emit networkError();
        return;
    }
}
}
