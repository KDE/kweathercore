#include "pendingweatherforecast.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
namespace KWeatherCore
{
class PendingWeatherForecastPrivate
{
public:
    WeatherForecast forecast;
    QString timezone;
};

template<class T>
PendingWeatherForecast::PendingWeatherForecast(const QString &timezone, T &&sunrise)
    : d(new PendingWeatherForecastPrivate)
{
    d->timezone = timezone;
    d->forecast.setSunriseForecast(std::forward<T>(sunrise));
}
PendingWeatherForecast::~PendingWeatherForecast()
{
    delete d;
}
void PendingWeatherForecast::parseResults(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        qWarning() << "network error when fetching forecast:" << reply->errorString();
        Q_EMIT networkError();
        return;
    }

    auto hourlyForecast = QVector<HourlyWeatherForecast>();
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

    //    for (auto fc : currentData_.hourlyForecasts()) {
    //        fc.setDate(fc.date().toTimeZone(QTimeZone(timeZone_.toUtf8())));
    //    }

    //    applySunriseDataToForecast(); // applies sunrise data whether we have it or not

    //    emit updated(currentData_);
}

void PendingWeatherForecast::parseOneElement(const QJsonObject &obj, QVector<HourlyWeatherForecast> &hourlyForecast)
{
    /*~~~~~~~~~~ static variable ~~~~~~~~~~~*/
    // rank weather (for what best describes the day overall)
    static const QHash<QString, int> rank = {// only need neutral icons
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
    static const QMap<QString, ResolvedWeatherDesc> apiDescMap = {
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
                                              apiDescMap[symbolCode + "_neutral"].icon,
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
}
