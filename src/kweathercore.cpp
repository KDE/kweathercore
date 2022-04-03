/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kweathercore_p.h"

#include <KLazyLocalizedString>

#include <cstring>

namespace KWeatherCore
{
KWeatherCorePrivate::KWeatherCorePrivate(QObject *parent)
    : QObject(parent)
{
}

Polygon KWeatherCorePrivate::stringToPolygon(const QString &str)
{
    Polygon res;
    const auto pairList = str.split(QLatin1Char(' '));
    for (auto &pair : pairList) {
        auto coordinate = pair.split(QLatin1Char(','));
        res.push_back({coordinate.front().toFloat(), coordinate.back().toFloat()});
    }
    return res;
}

QString KWeatherCorePrivate::toFixedString(double num)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << num;
    return QString::fromStdString(oss.str());
}

QDir KWeatherCorePrivate::getCacheDirectory(double latitude, double longitude)
{
#ifdef Q_OS_ANDROID
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QStringLiteral("/cache/") + toFixedString(latitude) + QStringLiteral("/")
             + toFixedString(longitude));
    if (!dir.exists())
        dir.mkpath(QStringLiteral("."));
    return dir;
#else
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation) + QStringLiteral("/kweather/cache/") + toFixedString(latitude)
             + QStringLiteral("/") + toFixedString(longitude));
    if (!dir.exists()) {
        dir.mkpath(QStringLiteral("."));
    }
    return dir;
#endif
}

AlertInfo::Urgency KWeatherCorePrivate::urgencyStringToEnum(const QString &str)
{
    if (str == QStringLiteral("Immediate")) {
        return AlertInfo::Urgency::Immediate;
    } else if (str == QStringLiteral("Expected")) {
        return AlertInfo::Urgency::Expected;
    } else if (str == QStringLiteral("Future")) {
        return AlertInfo::Urgency::Future;
    } else if (str == QStringLiteral("Past")) {
        return AlertInfo::Urgency::Past;
    } else {
        return AlertInfo::Urgency::Unknown;
    }
}

AlertInfo::Severity KWeatherCorePrivate::severityStringToEnum(const QString &str)
{
    if (str == QStringLiteral("Extreme")) {
        return AlertInfo::Severity::Extreme;
    } else if (str == QStringLiteral("Severe")) {
        return AlertInfo::Severity::Severe;
    } else if (str == QStringLiteral("Moderate")) {
        return AlertInfo::Severity::Moderate;
    } else if (str == QStringLiteral("Minor")) {
        return AlertInfo::Severity::Minor;
    } else {
        return AlertInfo::Severity::Unknown;
    }
}

AlertInfo::Certainty KWeatherCorePrivate::certaintyStringToEnum(const QString &str)
{
    if (str == QStringLiteral("Observed")) {
        return AlertInfo::Certainty::Observed;
    } else if (str == QStringLiteral("Likely")) {
        return AlertInfo::Certainty::Likely;
    } else if (str == QStringLiteral("Possible")) {
        return AlertInfo::Certainty::Possible;
    } else if (str == QStringLiteral("Unlikely")) {
        return AlertInfo::Certainty::Unlikely;
    } else {
        return AlertInfo::Certainty::Unknown;
    }
}

QString KWeatherCorePrivate::urgencyToString(AlertInfo::Urgency urgency)
{
    QString res;
    switch (urgency) {
    case AlertInfo::Urgency::Immediate:
        res = i18n("Immediate");
        break;
    case AlertInfo::Urgency::Expected:
        res = i18n("Expected");
        break;
    case AlertInfo::Urgency::Future:
        res = i18n("Future");
        break;
    case AlertInfo::Urgency::Past:
        res = i18n("Past");
        break;
    case AlertInfo::Urgency::Unknown:
        res = i18n("Unknown");
    }
    return res;
}

QString KWeatherCorePrivate::severityToString(AlertInfo::Severity severity)
{
    QString res;
    switch (severity) {
    case AlertInfo::Severity::Extreme:
        res = i18n("Extreme");
        break;
    case AlertInfo::Severity::Severe:
        res = i18n("Severe");
        break;
    case AlertInfo::Severity::Moderate:
        res = i18n("Moderate");
        break;
    case AlertInfo::Severity::Minor:
        res = i18n("Minor");
        break;
    case AlertInfo::Severity::Unknown:
        res = i18n("Unknown");
    }
    return res;
}

QString KWeatherCorePrivate::certaintyToString(AlertInfo::Certainty certainty)
{
    QString res;
    switch (certainty) {
    case AlertInfo::Certainty::Observed:
        res = i18n("Observed");
        break;
    case AlertInfo::Certainty::Likely:
        res = i18n("Likely");
        break;
    case AlertInfo::Certainty::Possible:
        res = i18n("Possible");
        break;
    case AlertInfo::Certainty::Unlikely:
        res = i18n("Unlikely");
        break;
    case AlertInfo::Certainty::Unknown:
        res = i18n("Unknown");
    }
    return res;
}

int KWeatherCorePrivate::weatherIconPriorityRank(const QString &icon)
{
    return WEATHER_ICON_PRIORITY_RANK[icon];
}

// https://api.met.no/weatherapi/weathericon/2.0/legends
// ### needs to be alphabetically sorted by key!
struct {
    const char *key;
    const char *icon;
    const KLazyLocalizedString desc;
} static constexpr const WEATHER_API_DESC_MAP[] = {
    {"clearsky_day", "weather-clear", kli18n("Clear")},
    {"clearsky_neutral", "weather-clear", kli18n("Clear")},
    {"clearsky_night", "weather-clear-night", kli18n("Clear")},
    {"cloudy_day", "weather-clouds", kli18n("Cloudy")},
    {"cloudy_neutral", "weather-clouds", kli18n("Cloudy")},
    {"cloudy_night", "weather-clouds-night", kli18n("Cloudy")},
    {"fair_day", "weather-few-clouds", kli18n("Partly Sunny")},
    {"fair_neutral", "weather-few-clouds", kli18n("Light Clouds")},
    {"fair_night", "weather-few-clouds-night", kli18n("Light Clouds")},
    {"fog_day", "weather-fog", kli18n("Fog")},
    {"fog_neutral", "weather-fog", kli18n("Fog")},
    {"fog_night", "weather-fog", kli18n("Fog")},

    {"heavyrain_day", "weather-showers-day", kli18n("Heavy Rain")},
    {"heavyrain_neutral", "weather-showers", kli18n("Heavy Rain")},
    {"heavyrain_night", "weather-showers-night", kli18n("Heavy Rain")},
    {"heavyrainandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"heavyrainandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"heavyrainandthunder_night", "weather-storm-night", kli18n("Storm")},
    {"heavyrainshowers_day", "weather-showers-day", kli18n("Heavy Rain")},
    {"heavyrainshowers_neutral", "weather-showers", kli18n("Heavy Rain")},
    {"heavyrainshowers_night", "weather-showers-night", kli18n("Heavy Rain")},
    {"heavyrainshowersandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"heavyrainshowersandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"heavyrainshowersandthunder_night", "weather-storm-night", kli18n("Storm")},

    {"heavysleet_day", "weather-freezing-rain", kli18n("Heavy Sleet")},
    {"heavysleet_neutral", "weather-freezing-rain", kli18n("Heavy Sleet")},
    {"heavysleet_night", "weather-freezing-rain", kli18n("Heavy Sleet")},
    {"heavysleetandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"heavysleetandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"heavysleetandthunder_night", "weather-storm-night", kli18n("Storm")},
    {"heavysleetshowers_day", "weather-freezing-rain", kli18n("Heavy Sleet")},
    {"heavysleetshowers_neutral", "weather-freezing-rain", kli18n("Heavy Sleet")},
    {"heavysleetshowers_night", "weather-freezing-rain", kli18n("Heavy Sleet")},
    {"heavysleetshowersandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"heavysleetshowersandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"heavysleetshowersandthunder_night", "weather-storm-night", kli18n("Storm")},

    {"heavysnow_day", "weather-snow", kli18n("Heavy Snow")},
    {"heavysnow_neutral", "weather-snow", kli18n("Heavy Snow")},
    {"heavysnow_night", "weather-snow", kli18n("Heavy Snow")},
    {"heavysnowshowers_day", "weather-snow", kli18n("Heavy Snow")},
    {"heavysnowshowers_neutral", "weather-snow", kli18n("Heavy Snow")},
    {"heavysnowshowers_night", "weather-snow", kli18n("Heavy Snow")},
    {"heavysnowshowersandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"heavysnowshowersandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"heavysnowshowersandthunder_night", "weather-storm-night", kli18n("Storm")},

    {"lightrain_day", "weather-showers-scattered-day", kli18n("Light Rain")},
    {"lightrain_neutral", "weather-showers-scattered", kli18n("Light Rain")},
    {"lightrain_night", "weather-showers-scattered-night", kli18n("Light Rain")},
    {"lightrainandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"lightrainandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"lightrainandthunder_night", "weather-storm-night", kli18n("Storm")},
    {"lightrainshowers_day", "weather-showers-scattered-day", kli18n("Light Rain")},
    {"lightrainshowers_neutral", "weather-showers-scattered", kli18n("Light Rain")},
    {"lightrainshowers_night", "weather-showers-scattered-night", kli18n("Light Rain")},
    {"lightrainshowersandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"lightrainshowersandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"lightrainshowersandthunder_night", "weather-storm-night", kli18n("Storm")},

    {"lightsleet_day", "weather-showers-scattered-day", kli18n("Light Sleet")},
    {"lightsleet_neutral", "weather-showers-scattered", kli18n("Light Sleet")},
    {"lightsleet_night", "weather-showers-scattered-night", kli18n("Light Sleet")},
    {"lightsleetandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"lightsleetandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"lightsleetandthunder_night", "weather-storm-night", kli18n("Storm")},
    {"lightsleetshowers_day", "weather-showers-scattered-day", kli18n("Light Sleet")},
    {"lightsleetshowers_neutral", "weather-showers-scattered", kli18n("Light Sleet")},
    {"lightsleetshowers_night", "weather-showers-scattered-night", kli18n("Light Sleet")},

    {"lightsnow_day", "weather-snow-scattered-day", kli18n("Light Snow")},
    {"lightsnow_neutral", "weather-snow-scattered", kli18n("Light Snow")},
    {"lightsnow_night", "weather-snow-scattered-night", kli18n("Light Snow")},
    {"lightsnowandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"lightsnowandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"lightsnowandthunder_night", "weather-storm-night", kli18n("Storm")},
    {"lightsnowshowers_day", "weather-snow-scattered-day", kli18n("Light Snow")},
    {"lightsnowshowers_neutral", "weather-snow-scattered", kli18n("Light Snow")},
    {"lightsnowshowers_night", "weather-snow-scattered-night", kli18n("Light Snow")},

    {"lightssleetshowersandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"lightssleetshowersandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"lightssleetshowersandthunder_night", "weather-storm-night", kli18n("Storm")},
    {"lightssnowshowersandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"lightssnowshowersandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"lightssnowshowersandthunder_night", "weather-storm-night", kli18n("Storm")},

    {"partlycloudy_day", "weather-clouds", kli18n("Partly Cloudy")},
    {"partlycloudy_neutral", "weather-clouds", kli18n("Partly Cloudy")},
    {"partlycloudy_night", "weather-clouds-night", kli18n("Partly Cloudy")},

    {"rain_day", "weather-showers-day", kli18n("Rain")},
    {"rain_neutral", "weather-showers", kli18n("Rain")},
    {"rain_night", "weather-showers-night", kli18n("Rain")},
    {"rainandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"rainandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"rainandthunder_night", "weather-storm-night", kli18n("Storm")},
    {"rainshowers_day", "weather-showers-day", kli18n("Rain")},
    {"rainshowers_neutral", "weather-showers", kli18n("Rain")},
    {"rainshowers_night", "weather-showers-night", kli18n("Rain")},
    {"rainshowersandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"rainshowersandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"rainshowersandthunder_night", "weather-storm-night", kli18n("Storm")},

    {"sleet_day", "weather-freezing-rain", kli18n("Sleet")},
    {"sleet_neutral", "weather-freezing-rain", kli18n("Sleet")},
    {"sleet_night", "weather-freezing-rain", kli18n("Sleet")},
    {"sleetandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"sleetandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"sleetandthunder_night", "weather-storm-night", kli18n("Storm")},
    {"sleetshowers_day", "weather-freezing-rain", kli18n("Sleet")},
    {"sleetshowers_neutral", "weather-freezing-rain", kli18n("Sleet")},
    {"sleetshowers_night", "weather-freezing-rain", kli18n("Sleet")},
    {"sleetshowersandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"sleetshowersandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"sleetshowersandthunder_night", "weather-storm-night", kli18n("Storm")},

    {"snow_day", "weather-snow", kli18n("Snow")},
    {"snow_neutral", "weather-snow", kli18n("Snow")},
    {"snow_night", "weather-snow", kli18n("Snow")},
    {"snowandthunder_day", "weather-snow", kli18n("Snow")},
    {"snowandthunder_neutral", "weather-snow", kli18n("Snow")},
    {"snowandthunder_night", "weather-snow", kli18n("Snow")},
    {"snowshowers_day", "weather-snow", kli18n("Snow")},
    {"snowshowers_neutral", "weather-snow", kli18n("Snow")},
    {"snowshowers_night", "weather-snow", kli18n("Snow")},
    {"snowshowersandthunder_day", "weather-storm-day", kli18n("Storm")},
    {"snowshowersandthunder_neutral", "weather-storm", kli18n("Storm")},
    {"snowshowersandthunder_night", "weather-storm-night", kli18n("Storm")},
};

ResolvedWeatherDesc KWeatherCorePrivate::resolveAPIWeatherDesc(const QString &desc)
{
    const auto key = desc.toUtf8();
    const auto it = std::lower_bound(std::begin(WEATHER_API_DESC_MAP), std::end(WEATHER_API_DESC_MAP), key, [](const auto &lhs, const auto &rhs) {
        return std::strcmp(lhs.key, rhs.constData()) < 0;
    });
    if (it != std::end(WEATHER_API_DESC_MAP) && std::strcmp((*it).key, key.constData()) == 0) {
        return ResolvedWeatherDesc(QLatin1String((*it).icon), (*it).desc.toString());
    }

    return {};
}

}
