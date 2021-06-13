/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kweathercore_p.h"

namespace KWeatherCore
{

KWeatherCorePrivate::KWeatherCorePrivate(QObject *parent) : QObject(parent) 
{
}

Polygon KWeatherCorePrivate::stringToPolygon(const QString &str) 
{
    Polygon res;
    const auto pairList = str.split(QLatin1Char(' '));
    for (auto &pair : pairList) {
        auto coordinate = pair.split(QLatin1Char(','));
        res.push_back(
            {coordinate.front().toFloat(), coordinate.back().toFloat()});
    }
    return res;
}

QString KWeatherCorePrivate::toFixedString(double num) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << num;
    return QString::fromStdString(oss.str());
}

QDir KWeatherCorePrivate::getCacheDirectory(double latitude, double longitude)
{
#ifdef __ANDROID__
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
             QStringLiteral("/cache/") + toFixedString(latitude) +
             QStringLiteral("/") + toFixedString(longitude));
    if (!dir.exists())
        dir.mkpath(QStringLiteral("."));
    return dir;
#endif
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation)
             + QStringLiteral("/kweather/cache/")
             + toFixedString(latitude)
             + QStringLiteral("/") + toFixedString(longitude));
    if (!dir.exists())
        dir.mkpath(QStringLiteral("."));
    return dir;
}

AlertInfo::Urgency KWeatherCorePrivate::urgencyStringToEnum(const QString &str) 
{
    if (str == QStringLiteral("Immediate"))
        return AlertInfo::Urgency::Immediate;
    else if (str == QStringLiteral("Expected"))
        return AlertInfo::Urgency::Expected;
    else if (str == QStringLiteral("Future"))
        return AlertInfo::Urgency::Future;
    else if (str == QStringLiteral("Past"))
        return AlertInfo::Urgency::Past;
    else
        return AlertInfo::Urgency::Unknown;
}

AlertInfo::Severity KWeatherCorePrivate::severityStringToEnum(const QString &str) 
{
    if (str == QStringLiteral("Extreme"))
        return AlertInfo::Severity::Extreme;
    else if (str == QStringLiteral("Severe"))
        return AlertInfo::Severity::Severe;
    else if (str == QStringLiteral("Moderate"))
        return AlertInfo::Severity::Moderate;
    else if (str == QStringLiteral("Minor"))
        return AlertInfo::Severity::Minor;
    else
        return AlertInfo::Severity::Unknown;
}

AlertInfo::Certainty KWeatherCorePrivate::certaintyStringToEnum(const QString &str) 
{
    if (str == QStringLiteral("Observed"))
        return AlertInfo::Certainty::Observed;
    else if (str == QStringLiteral("Likely"))
        return AlertInfo::Certainty::Likely;
    else if (str == QStringLiteral("Possible"))
        return AlertInfo::Certainty::Possible;
    else if (str == QStringLiteral("Unlikely"))
        return AlertInfo::Certainty::Unlikely;
    else
        return AlertInfo::Certainty::Unknown;
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
            res  = i18n("Severe");
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

ResolvedWeatherDesc KWeatherCorePrivate::resolveAPIWeatherDesc(const QString &desc) 
{
    return WEATHER_API_DESC_MAP[desc];
}

}
