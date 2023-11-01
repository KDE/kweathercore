/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kweathercore_p.h"

#include <KLazyLocalizedString>

#include <QStandardPaths>

#include <cstring>
#include <iomanip>
#include <sstream>

namespace KWeatherCore
{
CAPPolygon KWeatherCorePrivate::stringToPolygon(const QString &str)
{
    CAPPolygon res;
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    const auto pairList = QStringView(str).split(QLatin1Char(' '), Qt::SkipEmptyParts);
#else
    const auto pairList = str.splitRef(QLatin1Char(' '), Qt::SkipEmptyParts);
#endif

    res.reserve(pairList.size());
    for (auto &pair : pairList) {
        auto coordinate = pair.split(QLatin1Char(','));
        if (coordinate.size() != 2) {
            continue;
        }
        bool latOk = false, lonOk = false;
        res.push_back({coordinate.front().toFloat(&latOk), coordinate.back().toFloat(&lonOk)});
        if (!latOk || !lonOk) {
            res.pop_back();
        }
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

CAPAlertInfo::Urgency KWeatherCorePrivate::urgencyStringToEnum(const QString &str)
{
    if (str == QLatin1String("Immediate")) {
        return CAPAlertInfo::Urgency::Immediate;
    } else if (str == QLatin1String("Expected")) {
        return CAPAlertInfo::Urgency::Expected;
    } else if (str == QLatin1String("Future")) {
        return CAPAlertInfo::Urgency::Future;
    } else if (str == QLatin1String("Past")) {
        return CAPAlertInfo::Urgency::Past;
    } else {
        return CAPAlertInfo::Urgency::UnknownUrgency;
    }
}

CAPAlertInfo::Severity KWeatherCorePrivate::severityStringToEnum(const QString &str)
{
    if (str == QLatin1String("Extreme")) {
        return CAPAlertInfo::Severity::Extreme;
    } else if (str == QLatin1String("Severe")) {
        return CAPAlertInfo::Severity::Severe;
    } else if (str == QLatin1String("Moderate")) {
        return CAPAlertInfo::Severity::Moderate;
    } else if (str == QLatin1String("Minor")) {
        return CAPAlertInfo::Severity::Minor;
    } else {
        return CAPAlertInfo::Severity::UnknownSeverity;
    }
}

CAPAlertInfo::Certainty KWeatherCorePrivate::certaintyStringToEnum(const QString &str)
{
    if (str == QLatin1String("Observed")) {
        return CAPAlertInfo::Certainty::Observed;
    } else if (str == QLatin1String("Likely")) {
        return CAPAlertInfo::Certainty::Likely;
    } else if (str == QLatin1String("Possible")) {
        return CAPAlertInfo::Certainty::Possible;
    } else if (str == QLatin1String("Unlikely")) {
        return CAPAlertInfo::Certainty::Unlikely;
    } else {
        return CAPAlertInfo::Certainty::UnknownCertainty;
    }
}

QString KWeatherCorePrivate::urgencyToString(CAPAlertInfo::Urgency urgency)
{
    QString res;
    switch (urgency) {
    case CAPAlertInfo::Urgency::Immediate:
        res = i18n("Immediate");
        break;
    case CAPAlertInfo::Urgency::Expected:
        res = i18n("Expected");
        break;
    case CAPAlertInfo::Urgency::Future:
        res = i18n("Future");
        break;
    case CAPAlertInfo::Urgency::Past:
        res = i18n("Past");
        break;
    case CAPAlertInfo::Urgency::UnknownUrgency:
        res = i18n("Unknown");
    }
    return res;
}

QString KWeatherCorePrivate::severityToString(CAPAlertInfo::Severity severity)
{
    QString res;
    switch (severity) {
    case CAPAlertInfo::Severity::Extreme:
        res = i18n("Extreme");
        break;
    case CAPAlertInfo::Severity::Severe:
        res = i18n("Severe");
        break;
    case CAPAlertInfo::Severity::Moderate:
        res = i18n("Moderate");
        break;
    case CAPAlertInfo::Severity::Minor:
        res = i18n("Minor");
        break;
    case CAPAlertInfo::Severity::UnknownSeverity:
        res = i18n("Unknown");
    }
    return res;
}

QString KWeatherCorePrivate::certaintyToString(CAPAlertInfo::Certainty certainty)
{
    QString res;
    switch (certainty) {
    case CAPAlertInfo::Certainty::Observed:
        res = i18n("Observed");
        break;
    case CAPAlertInfo::Certainty::Likely:
        res = i18n("Likely");
        break;
    case CAPAlertInfo::Certainty::Possible:
        res = i18n("Possible");
        break;
    case CAPAlertInfo::Certainty::Unlikely:
        res = i18n("Unlikely");
        break;
    case CAPAlertInfo::Certainty::UnknownCertainty:
        res = i18n("Unknown");
    }
    return res;
}

// rank weather (for what best describes the day overall)
// only need neutral icons
// ### needs to be alphabetically sorted by icon name!
struct {
    const char *iconName;
    int rank;
} static constexpr const WEATHER_ICON_PRIORITY_RANK[] = {
    {"weather-clear", 0},
    {"weather-clouds", 2},
    {"weather-few-clouds", 1},
    {"weather-fog", 3},
    {"weather-freezing-rain", 6},
    {"weather-freezing-scattered-rain", 6},
    {"weather-freezing-scattered-rain-storm", 7},
    {"weather-freezing-storm", 6},
    {"weather-hail", 5},
    {"weather-mist", 3},
    {"weather-none-available", -1},
    {"weather-showers", 5},
    {"weather-showers-scattered", 4},
    {"weather-showers-scattered-storm", 7},
    {"weather-snow", 5},
    {"weather-snow-rain", 6},
    {"weather-snow-scattered", 4},
    {"weather-snow-scattered-storm", 7},
    {"weather-snow-storm", 7},
    {"weather-storm", 7},
};

int KWeatherCorePrivate::weatherIconPriorityRank(const QString &icon)
{
    const auto it = std::lower_bound(std::begin(WEATHER_ICON_PRIORITY_RANK), std::end(WEATHER_ICON_PRIORITY_RANK), icon, [](const auto &lhs, const auto &rhs) {
        return QLatin1String(lhs.iconName) < rhs;
    });
    if (it != std::end(WEATHER_ICON_PRIORITY_RANK) && QLatin1String((*it).iconName) == icon) {
        return (*it).rank;
    }

    return 0;
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
    {"heavyrainandthunder_day", "weather-storm-day", kli18n("Thunderstorm with Heavy Rain")},
    {"heavyrainandthunder_neutral", "weather-storm", kli18n("Thunderstorm with Heavy Rain")},
    {"heavyrainandthunder_night", "weather-storm-night", kli18n("Thunderstorm with Heavy Rain")},
    {"heavyrainshowers_day", "weather-showers-day", kli18n("Heavy Rain Shower")},
    {"heavyrainshowers_neutral", "weather-showers", kli18n("Heavy Rain Shower")},
    {"heavyrainshowers_night", "weather-showers-night", kli18n("Heavy Rain Shower")},
    {"heavyrainshowersandthunder_day", "weather-storm-day", kli18n("Thunderstorm with Heavy Rain Shower")},
    {"heavyrainshowersandthunder_neutral", "weather-storm", kli18n("Thunderstorm with Heavy Rain Shower")},
    {"heavyrainshowersandthunder_night", "weather-storm-night", kli18n("Thunderstorm with Heavy Rain Shower")},

    {"heavysleet_day", "weather-freezing-rain", kli18n("Heavy Sleet")},
    {"heavysleet_neutral", "weather-freezing-rain", kli18n("Heavy Sleet")},
    {"heavysleet_night", "weather-freezing-rain", kli18n("Heavy Sleet")},
    {"heavysleetandthunder_day", "weather-freezing-storm", kli18n("Thunderstorm with Heavy Sleet")},
    {"heavysleetandthunder_neutral", "weather-freezing-storm", kli18n("Thunderstorm with Heavy Sleet")},
    {"heavysleetandthunder_night", "weather-freezing-storm", kli18n("Thunderstorm with Heavy Sleet")},
    {"heavysleetshowers_day", "weather-freezing-rain-day", kli18n("Heavy Sleet Shower")},
    {"heavysleetshowers_neutral", "weather-freezing-rain", kli18n("Heavy Sleet Shower")},
    {"heavysleetshowers_night", "weather-freezing-rain-night", kli18n("Heavy Sleet Shower")},
    {"heavysleetshowersandthunder_day", "weather-freezing-storm-day", kli18n("Thunderstorm with Heavy Sleet Shower")},
    {"heavysleetshowersandthunder_neutral", "weather-freezing-storm", kli18n("Thunderstorm with Heavy Sleet Shower")},
    {"heavysleetshowersandthunder_night", "weather-freezing-storm-night", kli18n("Thunderstorm with Heavy Sleet Shower")},

    {"heavysnow_day", "weather-snow", kli18n("Heavy Snow")},
    {"heavysnow_neutral", "weather-snow", kli18n("Heavy Snow")},
    {"heavysnow_night", "weather-snow", kli18n("Heavy Snow")},
    {"heavysnowshowers_day", "weather-snow-day", kli18n("Heavy Snow Shower")},
    {"heavysnowshowers_neutral", "weather-snow", kli18n("Heavy Snow Shower")},
    {"heavysnowshowers_night", "weather-snow-night", kli18n("Heavy Snow Shower")},
    {"heavysnowshowersandthunder_day", "weather-snow-storm-day", kli18n("Thunderstorm with Heavy Snow Shower")},
    {"heavysnowshowersandthunder_neutral", "weather-snow-storm", kli18n("Thunderstorm with Heavy Snow Shower")},
    {"heavysnowshowersandthunder_night", "weather-snow-storm-night", kli18n("Thunderstorm with Heavy Snow Shower")},

    {"lightrain_day", "weather-showers-scattered", kli18n("Light Rain")},
    {"lightrain_neutral", "weather-showers-scattered", kli18n("Light Rain")},
    {"lightrain_night", "weather-showers-scattered", kli18n("Light Rain")},
    {"lightrainandthunder_day", "weather-showers-scattered-storm", kli18n("Thunderstorm with Light Rain")},
    {"lightrainandthunder_neutral", "weather-showers-scattered-storm", kli18n("Thunderstorm with Light Rain")},
    {"lightrainandthunder_night", "weather-showers-scattered-storm", kli18n("Thunderstorm with Light Rain")},
    {"lightrainshowers_day", "weather-showers-scattered-day", kli18n("Light Rain Shower")},
    {"lightrainshowers_neutral", "weather-showers-scattered", kli18n("Light Rain Shower")},
    {"lightrainshowers_night", "weather-showers-scattered-night", kli18n("Light Rain Shower")},
    {"lightrainshowersandthunder_day", "weather-showers-scattered-storm-day", kli18n("Thunderstorm with Light Rain Shower")},
    {"lightrainshowersandthunder_neutral", "weather-showers-scattered-storm", kli18n("Thunderstorm with Light Rain Shower")},
    {"lightrainshowersandthunder_night", "weather-showers-scattered-storm-night", kli18n("Thunderstorm with Light Rain Shower")},

    {"lightsleet_day", "weather-showers-scattered", kli18n("Light Sleet")},
    {"lightsleet_neutral", "weather-showers-scattered", kli18n("Light Sleet")},
    {"lightsleet_night", "weather-showers-scattered", kli18n("Light Sleet")},
    {"lightsleetandthunder_day", "weather-freezing-scattered-rain-storm", kli18n("Thunderstorm with Light Sleet")},
    {"lightsleetandthunder_neutral", "weather-freezing-scattered-rain-storm", kli18n("Thunderstorm with Light Sleet")},
    {"lightsleetandthunder_night", "weather-freezing-scattered-rain-storm", kli18n("Thunderstorm with Light Sleet")},
    {"lightsleetshowers_day", "weather-freezing-scattered-rain-day", kli18n("Light Sleet Shower")},
    {"lightsleetshowers_neutral", "weather-freezing-scattered-rain", kli18n("Light Sleet Shower")},
    {"lightsleetshowers_night", "weather-freezing-scattered-rain-night", kli18n("Light Sleet Shower")},

    {"lightsnow_day", "weather-snow-scattered", kli18n("Light Snow")},
    {"lightsnow_neutral", "weather-snow-scattered", kli18n("Light Snow")},
    {"lightsnow_night", "weather-snow-scattered", kli18n("Light Snow")},
    {"lightsnowandthunder_day", "weather-snow-scattered-storm", kli18n("Thunderstorm with Light Snow")},
    {"lightsnowandthunder_neutral", "weather-snow-scattered-storm", kli18n("Thunderstorm with Light Snow")},
    {"lightsnowandthunder_night", "weather-snow-scattered-storm", kli18n("Thunderstorm with Light Snow")},
    {"lightsnowshowers_day", "weather-snow-scattered-day", kli18n("Light Snow Shower")},
    {"lightsnowshowers_neutral", "weather-snow-scattered", kli18n("Light Snow Shower")},
    {"lightsnowshowers_night", "weather-snow-scattered-night", kli18n("Light Snow Shower")},

    {"lightssleetshowersandthunder_day", "weather-freezing-scattered-rain-storm-day", kli18n("Thunderstorm with Light Sleet Shower")},
    {"lightssleetshowersandthunder_neutral", "weather-freezing-scattered-rain-storm", kli18n("Thunderstorm with Light Sleet Shower")},
    {"lightssleetshowersandthunder_night", "weather-freezing-scattered-rain-storm-night", kli18n("Thunderstorm with Light Sleet Shower")},
    {"lightssnowshowersandthunder_day", "weather-snow-scattered-storm-day", kli18n("Thunderstorm with Light Snow Shower")},
    {"lightssnowshowersandthunder_neutral", "weather-snow-scattered-storm", kli18n("Thunderstorm with Light Snow Shower")},
    {"lightssnowshowersandthunder_night", "weather-snow-scattered-storm-night", kli18n("Thunderstorm with Light Snow Shower")},

    {"partlycloudy_day", "weather-clouds", kli18n("Partly Cloudy")},
    {"partlycloudy_neutral", "weather-clouds", kli18n("Partly Cloudy")},
    {"partlycloudy_night", "weather-clouds-night", kli18n("Partly Cloudy")},

    {"rain_day", "weather-showers", kli18n("Rain")},
    {"rain_neutral", "weather-showers", kli18n("Rain")},
    {"rain_night", "weather-showers", kli18n("Rain")},
    {"rainandthunder_day", "weather-storm", kli18n("Thunderstorm with Rain")},
    {"rainandthunder_neutral", "weather-storm", kli18n("Thunderstorm with Rain")},
    {"rainandthunder_night", "weather-storm", kli18n("Thunderstorm with Rain")},
    {"rainshowers_day", "weather-showers-day", kli18n("Rain Shower")},
    {"rainshowers_neutral", "weather-showers", kli18n("Rain Shower")},
    {"rainshowers_night", "weather-showers-night", kli18n("Rain Shower")},
    {"rainshowersandthunder_day", "weather-storm-day", kli18n("Thunderstorm with Rain Shower")},
    {"rainshowersandthunder_neutral", "weather-storm", kli18n("Thunderstorm with Rain Shower")},
    {"rainshowersandthunder_night", "weather-storm-night", kli18n("Thunderstorm with Rain Shower")},

    {"sleet_day", "weather-freezing-rain", kli18n("Sleet")},
    {"sleet_neutral", "weather-freezing-rain", kli18n("Sleet")},
    {"sleet_night", "weather-freezing-rain", kli18n("Sleet")},
    {"sleetandthunder_day", "weather-storm", kli18n("Thunderstorm with Sleet")},
    {"sleetandthunder_neutral", "weather-storm", kli18n("Thunderstorm with Sleet")},
    {"sleetandthunder_night", "weather-storm", kli18n("Thunderstorm with Sleet")},
    {"sleetshowers_day", "weather-freezing-rain-day", kli18n("Sleet Shower")},
    {"sleetshowers_neutral", "weather-freezing-rain", kli18n("Sleet Shower")},
    {"sleetshowers_night", "weather-freezing-rain-night", kli18n("Sleet Shower")},
    {"sleetshowersandthunder_day", "weather-freezing-storm-day", kli18n("Thunderstorm with Sleet Shower")},
    {"sleetshowersandthunder_neutral", "weather-freezing-storm", kli18n("Thunderstorm with Sleet Shower")},
    {"sleetshowersandthunder_night", "weather-freezing-storm-night", kli18n("Thunderstorm with Sleet Shower")},

    {"snow_day", "weather-snow", kli18n("Snow")},
    {"snow_neutral", "weather-snow", kli18n("Snow")},
    {"snow_night", "weather-snow", kli18n("Snow")},
    {"snowandthunder_day", "weather-snow-storm", kli18n("Thunderstorm with Snow")},
    {"snowandthunder_neutral", "weather-snow-storm", kli18n("Thunderstorm with Snow")},
    {"snowandthunder_night", "weather-snow-storm", kli18n("Thunderstorm with Snow")},
    {"snowshowers_day", "weather-snow-day", kli18n("Snow Shower")},
    {"snowshowers_neutral", "weather-snow", kli18n("Snow Shower")},
    {"snowshowers_night", "weather-snow-night", kli18n("Snow Shower")},
    {"snowshowersandthunder_day", "weather-snow-storm-day", kli18n("Thunderstorm with Snow Shower")},
    {"snowshowersandthunder_neutral", "weather-snow-storm", kli18n("Thunderstorm with Snow Shower")},
    {"snowshowersandthunder_night", "weather-snow-storm-night", kli18n("Thunderstorm with Snow Shower")},

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
