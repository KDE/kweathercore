/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "pendingweatherforecast.h"
#include "geotimezone.h"
#include "pendingweatherforecast_p.h"
#include "sunrisesource.h"
#include <KLocalizedString>
#include <QExplicitlySharedDataPointer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QTimeZone>
namespace KWeatherCore
{
PendingWeatherForecastPrivate::PendingWeatherForecastPrivate(double latitude, double longitude, const QString &timezone, QVector<Sunrise> &&sunrise)
    : forecast(QExplicitlySharedDataPointer<WeatherForecast>(new WeatherForecast))
{
    forecast->setSunriseForecast(std::move(sunrise));
    if (timezone.isEmpty()) {
        hasTimezone = false;
        getTimezone(latitude, longitude);
    } else {
        forecast->setTimezone(timezone);
    }
    if (forecast->sunriseForecast().size() <= 10) {
        hasSunrise = false;
    }
}

PendingWeatherForecastPrivate::PendingWeatherForecastPrivate(double latitude, double longitude, const QString &timezone, const QVector<Sunrise> &sunrise)
    : forecast(QExplicitlySharedDataPointer<WeatherForecast>(new WeatherForecast))
{
    forecast->setSunriseForecast(sunrise);
    if (timezone.isEmpty()) {
        hasTimezone = false;
        getTimezone(latitude, longitude);
    } else {
        forecast->setTimezone(timezone);
    }
    if (forecast->sunriseForecast().size() <= 10) {
        hasSunrise = false;
    }
}
void PendingWeatherForecastPrivate::getTimezone(double latitude, double longitude)
{
    auto timezoneSource = new GeoTimezone(latitude, longitude, this);
    connect(timezoneSource, &GeoTimezone::finished, this, &PendingWeatherForecastPrivate::parseTimezoneResult);
}
void PendingWeatherForecastPrivate::parseTimezoneResult(QString result)
{
    hasTimezone = true;
    forecast->setTimezone(result);
    if (!hasSunrise)
        getSunrise(m_latitude, m_longitude, QDateTime::currentDateTime().toTimeZone(QTimeZone(result.toUtf8())).offsetFromUtc());
}

void PendingWeatherForecastPrivate::getSunrise(double latitude, double longitude, int offset)
{
    m_sunriseSource = new SunriseSource(latitude, longitude, offset, forecast->sunriseForecast(), this);
    m_sunriseSource->requestData();
    connect(m_sunriseSource, &SunriseSource::finished, this, &PendingWeatherForecastPrivate::parseSunriseResults);
}
void PendingWeatherForecastPrivate::parseSunriseResults()
{
    if (m_sunriseSource) {
        forecast->setSunriseForecast(m_sunriseSource->value());
        hasSunrise = true;
    }

    // if this arrived later than forecast
    if (hourlyForecast.size() > 0)
        applySunriseToForecast();
}
void PendingWeatherForecastPrivate::parseWeatherForecastResults(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        qWarning() << "network error when fetching forecast:" << reply->errorString();
        Q_EMIT networkError();
        return;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    if (jsonDocument.isObject()) {
        QJsonObject obj = jsonDocument.object();
        QJsonObject prop = obj[QStringLiteral("properties")].toObject();

        if (prop.contains(QStringLiteral("timeseries"))) {
            QJsonArray timeseries = prop[QStringLiteral("timeseries")].toArray();

            // loop over all forecast data
            for (QJsonValueRef ref : timeseries) {
                QJsonObject refObj = ref.toObject();
                parseOneElement(refObj, hourlyForecast);
            }
        }
    }

    if (hasTimezone && hasSunrise)
        applySunriseToForecast();

    Q_EMIT finished();
}

void PendingWeatherForecastPrivate::parseOneElement(const QJsonObject &obj, QVector<HourlyWeatherForecast> &hourlyForecast)
{
    /*~~~~~~~~~~ lambda ~~~~~~~~~~~*/

    auto getWindDeg = [](double deg) -> WindDirection {
        if (deg < 22.5 || deg >= 337.5) {
            return WindDirection::S; // from N
        } else if (deg > 22.5 || deg <= 67.5) {
            return WindDirection::SW; // from NE
        } else if (deg > 67.5 || deg <= 112.5) {
            return WindDirection::W; // from E
        } else if (deg > 112.5 || deg <= 157.5) {
            return WindDirection::NW; // from SE
        } else if (deg > 157.5 || deg <= 202.5) {
            return WindDirection::N; // from S
        } else if (deg > 202.5 || deg <= 247.5) {
            return WindDirection::NE; // from SW
        } else if (deg > 247.5 || deg <= 292.5) {
            return WindDirection::E; // from W
        } else if (deg > 292.5 || deg <= 337.5) {
            return WindDirection::SE; // from NW
        }
        return WindDirection::N;
    };

    /*================== actual code ======================*/

    QJsonObject data = obj[QStringLiteral("data")].toObject(), instant = data[QStringLiteral("instant")].toObject()[QStringLiteral("details")].toObject();
    // ignore last forecast, which does not have enough data
    if (!data.contains(QStringLiteral("next_6_hours")) && !data.contains(QStringLiteral("next_1_hours")))
        return;

    // correct date to corresponding timezone of location if possible
    QDateTime date = QDateTime::fromString(obj.value(QStringLiteral("time")).toString(), Qt::ISODate);

    // get symbolCode and precipitation amount
    QString symbolCode;
    double precipitationAmount = 0;
    // some fields contain only "next_1_hours", and others may contain only
    // "next_6_hours"
    if (data.contains(QStringLiteral("next_1_hours"))) {
        QJsonObject nextOneHours = data[QStringLiteral("next_1_hours")].toObject();
        symbolCode = nextOneHours[QStringLiteral("summary")].toObject()[QStringLiteral("symbol_code")].toString(QStringLiteral("unknown"));
        precipitationAmount = nextOneHours[QStringLiteral("details")].toObject()[QStringLiteral("precipitation_amount")].toDouble();
    } else {
        QJsonObject nextSixHours = data[QStringLiteral("next_6_hours")].toObject();
        symbolCode = nextSixHours[QStringLiteral("summary")].toObject()[QStringLiteral("symbol_code")].toString(QStringLiteral("unknown"));
        precipitationAmount = nextSixHours[QStringLiteral("details")].toObject()[QStringLiteral("precipitation_amount")].toDouble();
    }

    symbolCode = symbolCode.split(QLatin1Char('_'))[0]; // trim _[day/night] from end -
                                                        // https://api.met.no/weatherapi/weathericon/2.0/legends
    auto hourForecast = HourlyWeatherForecast(QDateTime::fromString(obj.value(QStringLiteral("time")).toString(), Qt::ISODate),
                                              QString(),
                                              QString(),
                                              apiDescMap[symbolCode + QStringLiteral("_neutral")].icon,
                                              instant[QStringLiteral("air_temperature")].toDouble(),
                                              instant[QStringLiteral("air_pressure_at_sea_level")].toDouble(),
                                              getWindDeg(instant[QStringLiteral("wind_from_direction")].toDouble()),
                                              instant[QStringLiteral("wind_speed")].toDouble(),
                                              instant[QStringLiteral("relative_humidity")].toDouble(),
                                              instant[QStringLiteral("fog_area_fraction")].toDouble(),
                                              instant[QStringLiteral("ultraviolet_index_clear_sky")].toDouble(),
                                              precipitationAmount);

    hourlyForecast.append(std::move(hourForecast));
}

void PendingWeatherForecastPrivate::applySunriseToForecast()
{
    // ************* Lambda *************** //
    auto isDayTime = [](QDateTime date, const QVector<Sunrise> &sunrise) {
        for (auto sr : sunrise) {
            // if on the same day
            if (sr.sunRise().date().daysTo(date.date()) == 0 && sr.sunRise().date().day() == date.date().day()) {
                // 30 min threshold
                return sr.sunRise().addSecs(-1800) <= date && sr.sunSet().addSecs(1800) >= date;
            }
        }

        // not found
        return date.time().hour() >= 6 && date.time().hour() <= 18;
    };

    auto getSymbolCodeDescription = [](bool isDay, const QString &symbolCode) { return isDay ? apiDescMap[symbolCode + QStringLiteral("_day")].desc : apiDescMap[symbolCode + QStringLiteral("_night")].desc; };

    auto getSymbolCodeIcon = [](bool isDay, const QString &symbolCode) { return isDay ? apiDescMap[symbolCode + QStringLiteral("_day")].icon : apiDescMap[symbolCode + QStringLiteral("_night")].icon; };

    // ******* code ******** //
    for (auto &hourForecast : hourlyForecast) {
        hourForecast.setDate(hourForecast.date().toTimeZone(QTimeZone(m_timezone.toUtf8())));

        bool isDay;
        isDay = isDayTime(hourForecast.date(), forecast->sunriseForecast());

        hourForecast.setWeatherIcon(getSymbolCodeIcon(isDay, hourForecast.symbolCode())); // set day/night icon
        hourForecast.setWeatherDescription(getSymbolCodeDescription(isDay, hourForecast.symbolCode()));

        *forecast += hourForecast;
    }

    Q_EMIT finished();
}

PendingWeatherForecast::PendingWeatherForecast(double latitude, double longitude, QNetworkReply *reply, const QString &timezone, const QVector<Sunrise> &sunrise)
    : d(new PendingWeatherForecastPrivate(latitude, longitude, timezone, sunrise))
{
    connect(d, &PendingWeatherForecastPrivate::finished, [this] { this->m_finished = true; });
    connect(d, &PendingWeatherForecastPrivate::finished, this, &PendingWeatherForecast::finished);
    connect(d, &PendingWeatherForecastPrivate::networkError, this, &PendingWeatherForecast::networkError);
    if (reply) {
        auto d = this->d; // explicitly capture?
        connect(reply, &QNetworkReply::finished, [d, reply] { d->parseWeatherForecastResults(reply); });
    }
}

QExplicitlySharedDataPointer<WeatherForecast> PendingWeatherForecast::value() const
{
    return d->forecast;
}
PendingWeatherForecast::~PendingWeatherForecast()
{
    if (d)
        delete d;
}

const QHash<QString, int> PendingWeatherForecastPrivate::rank = { // only need neutral icons
    {QStringLiteral("weather-none-available"), -1},
    {QStringLiteral("weather-clear"), 0},
    {QStringLiteral("weather-few-clouds"), 1},
    {QStringLiteral("weather-clouds"), 2},
    {QStringLiteral("weather-fog"), 3},
    {QStringLiteral("weather-mist"), 3},
    {QStringLiteral("weather-showers-scattered"), 4},
    {QStringLiteral("weather-snow-scattered"), 4},
    {QStringLiteral("weather-showers"), 5},
    {QStringLiteral("weather-hail"), 5},
    {QStringLiteral("weather-snow"), 5},
    {QStringLiteral("weather-freezing-rain"), 6},
    {QStringLiteral("weather-freezing-storm"), 6},
    {QStringLiteral("weather-snow-rain"), 6},
    {QStringLiteral("weather-storm"), 7}};

const QMap<QString, PendingWeatherForecastPrivate::ResolvedWeatherDesc> PendingWeatherForecastPrivate::apiDescMap = {
    {QStringLiteral("heavyrainandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("heavyrainandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("heavyrainandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("heavysleetandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("heavysleetandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("heavysleetandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("heavysnowshowersandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("heavysnowshowersandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("heavysnowshowersandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("heavysnow_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Heavy Snow"))},
    {QStringLiteral("heavysnow_day"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Heavy Snow"))},
    {QStringLiteral("heavysnow_night"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Heavy Snow"))},
    {QStringLiteral("rainandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("rainandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("rainandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("heavysleetshowersandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("heavysleetshowersandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("heavysleetshowersandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("rainshowers_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-showers"), i18n("Rain"))},
    {QStringLiteral("rainshowers_day"), ResolvedWeatherDesc(QStringLiteral("weather-showers-day"), i18n("Rain"))},
    {QStringLiteral("rainshowers_night"), ResolvedWeatherDesc(QStringLiteral("weather-showers-night"), i18n("Rain"))},
    {QStringLiteral("fog_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-fog"), i18n("Fog"))},
    {QStringLiteral("fog_day"), ResolvedWeatherDesc(QStringLiteral("weather-fog"), i18n("Fog"))},
    {QStringLiteral("fog_night"), ResolvedWeatherDesc(QStringLiteral("weather-fog"), i18n("Fog"))},
    {QStringLiteral("heavysleetshowers_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-freezing-rain"), i18n("Heavy Sleet"))},
    {QStringLiteral("heavysleetshowers_day"), ResolvedWeatherDesc(QStringLiteral("weather-freezing-rain"), i18n("Heavy Sleet"))},
    {QStringLiteral("heavysleetshowers_night"), ResolvedWeatherDesc(QStringLiteral("weather-freezing-rain"), i18n("Heavy Sleet"))},
    {QStringLiteral("lightssnowshowersandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("lightssnowshowersandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("lightssnowshowersandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("cloudy_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-clouds"), i18n("Cloudy"))},
    {QStringLiteral("cloudy_day"), ResolvedWeatherDesc(QStringLiteral("weather-clouds"), i18n("Cloudy"))},
    {QStringLiteral("cloudy_night"), ResolvedWeatherDesc(QStringLiteral("weather-clouds-night"), i18n("Cloudy"))},
    {QStringLiteral("snowshowersandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("snowshowersandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("snowshowersandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("lightsnowshowers_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-snow-scattered"), i18n("Light Snow"))},
    {QStringLiteral("lightsnowshowers_day"), ResolvedWeatherDesc(QStringLiteral("weather-snow-scattered-day"), i18n("Light Snow"))},
    {QStringLiteral("lightsnowshowers_night"), ResolvedWeatherDesc(QStringLiteral("weather-snow-scattered-night"), i18n("Light Snow"))},
    {QStringLiteral("heavysleet_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-freezing-rain"), i18n("Heavy Sleet"))},
    {QStringLiteral("heavysleet_day"), ResolvedWeatherDesc(QStringLiteral("weather-freezing-rain"), i18n("Heavy Sleet"))},
    {QStringLiteral("heavysleet_night"), ResolvedWeatherDesc(QStringLiteral("weather-freezing-rain"), i18n("Heavy Sleet"))},
    {QStringLiteral("lightsnowandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("lightsnowandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("lightsnowandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("sleetshowersandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("sleetshowersandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("sleetshowersandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("rainshowersandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("rainshowersandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("rainshowersandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("lightsleet_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-showers-scattered"), i18n("Light Sleet"))},
    {QStringLiteral("lightsleet_day"), ResolvedWeatherDesc(QStringLiteral("weather-showers-scattered-day"), i18n("Light Sleet"))},
    {QStringLiteral("lightsleet_night"), ResolvedWeatherDesc(QStringLiteral("weather-showers-scattered-night"), i18n("Light Sleet"))},
    {QStringLiteral("lightssleetshowersandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("lightssleetshowersandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("lightssleetshowersandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("sleetandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("sleetandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("sleetandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("lightsnow_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-snow-scattered"), i18n("Light Snow"))},
    {QStringLiteral("lightsnow_day"), ResolvedWeatherDesc(QStringLiteral("weather-snow-scattered-day"), i18n("Light Snow"))},
    {QStringLiteral("lightsnow_night"), ResolvedWeatherDesc(QStringLiteral("weather-snow-scattered-night"), i18n("Light Snow"))},
    {QStringLiteral("sleet_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-freezing-rain"), i18n("Sleet"))},
    {QStringLiteral("sleet_day"), ResolvedWeatherDesc(QStringLiteral("weather-freezing-rain"), i18n("Sleet"))},
    {QStringLiteral("sleet_night"), ResolvedWeatherDesc(QStringLiteral("weather-freezing-rain"), i18n("Sleet"))},
    {QStringLiteral("heavyrainshowers_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-showers"), i18n("Heavy Rain"))},
    {QStringLiteral("heavyrainshowers_day"), ResolvedWeatherDesc(QStringLiteral("weather-showers-day"), i18n("Heavy Rain"))},
    {QStringLiteral("heavyrainshowers_night"), ResolvedWeatherDesc(QStringLiteral("weather-showers-night"), i18n("Heavy Rain"))},
    {QStringLiteral("lightsleetshowers_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-showers-scattered"), i18n("Light Sleet"))},
    {QStringLiteral("lightsleetshowers_day"), ResolvedWeatherDesc(QStringLiteral("weather-showers-scattered-day"), i18n("Light Sleet"))},
    {QStringLiteral("lightsleetshowers_night"), ResolvedWeatherDesc(QStringLiteral("weather-showers-scattered-night"), i18n("Light Sleet"))},
    {QStringLiteral("snowshowers_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Snow"))},
    {QStringLiteral("snowshowers_day"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Snow"))},
    {QStringLiteral("snowshowers_night"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Snow"))},
    {QStringLiteral("snowandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Snow"))},
    {QStringLiteral("snowandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Snow"))},
    {QStringLiteral("snowandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Snow"))},
    {QStringLiteral("lightsleetandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("lightsleetandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("lightsleetandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("snow_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Snow"))},
    {QStringLiteral("snow_day"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Snow"))},
    {QStringLiteral("snow_night"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Snow"))},
    {QStringLiteral("heavyrainshowersandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("heavyrainshowersandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("heavyrainshowersandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("rain_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-showers"), i18n("Rain"))},
    {QStringLiteral("rain_day"), ResolvedWeatherDesc(QStringLiteral("weather-showers-day"), i18n("Rain"))},
    {QStringLiteral("rain_night"), ResolvedWeatherDesc(QStringLiteral("weather-showers-night"), i18n("Rain"))},
    {QStringLiteral("heavysnowshowers_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Heavy Snow"))},
    {QStringLiteral("heavysnowshowers_day"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Heavy Snow"))},
    {QStringLiteral("heavysnowshowers_night"), ResolvedWeatherDesc(QStringLiteral("weather-snow"), i18n("Heavy Snow"))},
    {QStringLiteral("lightrain_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-showers-scattered"), i18n("Light Rain"))},
    {QStringLiteral("lightrain_day"), ResolvedWeatherDesc(QStringLiteral("weather-showers-scattered-day"), i18n("Light Rain"))},
    {QStringLiteral("lightrain_night"), ResolvedWeatherDesc(QStringLiteral("weather-showers-scattered-night"), i18n("Light Rain"))},
    {QStringLiteral("fair_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-few-clouds"), i18n("Light Clouds"))},
    {QStringLiteral("fair_day"), ResolvedWeatherDesc(QStringLiteral("weather-few-clouds"), i18n("Partly Sunny"))},
    {QStringLiteral("fair_night"), ResolvedWeatherDesc(QStringLiteral("weather-few-clouds-night"), i18n("Light Clouds"))},
    {QStringLiteral("partlycloudy_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-clouds"), i18n("Partly Cloudy"))},
    {QStringLiteral("partlycloudy_day"), ResolvedWeatherDesc(QStringLiteral("weather-clouds"), i18n("Partly Cloudy"))},
    {QStringLiteral("partlycloudy_night"), ResolvedWeatherDesc(QStringLiteral("weather-clouds-night"), i18n("Partly Cloudy"))},
    {QStringLiteral("clearsky_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-clear"), i18n("Clear"))},
    {QStringLiteral("clearsky_day"), ResolvedWeatherDesc(QStringLiteral("weather-clear"), i18n("Clear"))},
    {QStringLiteral("clearsky_night"), ResolvedWeatherDesc(QStringLiteral("weather-clear-night"), i18n("Clear"))},
    {QStringLiteral("lightrainshowers_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-showers-scattered"), i18n("Light Rain"))},
    {QStringLiteral("lightrainshowers_day"), ResolvedWeatherDesc(QStringLiteral("weather-showers-scattered-day"), i18n("Light Rain"))},
    {QStringLiteral("lightrainshowers_night"), ResolvedWeatherDesc(QStringLiteral("weather-showers-scattered-night"), i18n("Light Rain"))},
    {QStringLiteral("sleetshowers_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-freezing-rain"), i18n("Sleet"))},
    {QStringLiteral("sleetshowers_day"), ResolvedWeatherDesc(QStringLiteral("weather-freezing-rain"), i18n("Sleet"))},
    {QStringLiteral("sleetshowers_night"), ResolvedWeatherDesc(QStringLiteral("weather-freezing-rain"), i18n("Sleet"))},
    {QStringLiteral("lightrainandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("lightrainandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("lightrainandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("lightrainshowersandthunder_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-storm"), i18n("Storm"))},
    {QStringLiteral("lightrainshowersandthunder_day"), ResolvedWeatherDesc(QStringLiteral("weather-storm-day"), i18n("Storm"))},
    {QStringLiteral("lightrainshowersandthunder_night"), ResolvedWeatherDesc(QStringLiteral("weather-storm-night"), i18n("Storm"))},
    {QStringLiteral("heavyrain_neutral"), ResolvedWeatherDesc(QStringLiteral("weather-showers"), i18n("Heavy Rain"))},
    {QStringLiteral("heavyrain_day"), ResolvedWeatherDesc(QStringLiteral("weather-showers-day"), i18n("Heavy Rain"))},
    {QStringLiteral("heavyrain_night"), ResolvedWeatherDesc(QStringLiteral("weather-showers-night"), i18n("Heavy Rain"))},
};
}
