/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "weatherforecastsource.h"
#include "kweathercore_p.h"
#include "locationqueryresult.h"
#include "weatherforecast.h"

#include <KTimeZone>

#include <QFile>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QStandardPaths>

#include <algorithm>

namespace KWeatherCore
{
class WeatherForecastSourcePrivate
{
public:
    QNetworkAccessManager *m_nam = nullptr;
};

WeatherForecastSource::WeatherForecastSource(QObject *parent)
    : QObject(parent)
    , d(new WeatherForecastSourcePrivate)
{
}

WeatherForecastSource::~WeatherForecastSource() = default;

PendingWeatherForecast *WeatherForecastSource::requestData(double latitude, double longitude)
{
    QFile cache(KWeatherCorePrivate::getCacheDirectory(latitude, longitude).path() + QStringLiteral("/cache.json"));
    QString timezone;
    // valid cache
    if (cache.exists() && cache.open(QIODevice::ReadOnly)) {
        auto weatherforecast = WeatherForecast::fromJson(QJsonDocument::fromJson(cache.readAll()).object());
        timezone = weatherforecast.timezone();
        if (weatherforecast.createdTime().secsTo(QDateTime::currentDateTime()) <= 3600) {
            return new PendingWeatherForecast(weatherforecast);
        }
    }

    if (timezone.isEmpty()) {
        timezone = QString::fromUtf8(KTimeZone::fromLocation(latitude, longitude));
    }

    if (!d->m_nam) {
        d->m_nam = new QNetworkAccessManager(this);
        d->m_nam->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
        d->m_nam->setStrictTransportSecurityEnabled(true);
        d->m_nam->enableStrictTransportSecurityStore(true,
                                                     QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation)
                                                         + QLatin1String("/org.kde.kweathercore/hsts/"));
    }

    return new PendingWeatherForecast(latitude, longitude, timezone, d->m_nam);
}

PendingWeatherForecast *WeatherForecastSource::requestData(const KWeatherCore::LocationQueryResult &result)
{
    return requestData(result.latitude(), result.longitude());
}

void WeatherForecastSource::setNetworkAccessManager(QNetworkAccessManager *nam)
{
    if (d->m_nam == nam) {
        return;
    }

    if (d->m_nam->parent() == this) {
        delete d->m_nam;
    }
    d->m_nam = nam;
}
}

#include "moc_weatherforecastsource.cpp"
