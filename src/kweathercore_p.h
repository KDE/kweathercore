/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <iomanip>
#include <sstream>

#include <KLocalizedString>

#include <QDir>
#include <QHash>
#include <QStandardPaths>
#include <QString>
#include <QUrl>

#include "alertinfo.h"

namespace KWeatherCore
{
using Polygon = std::vector<std::pair<float, float>>;

// weather description
struct ResolvedWeatherDesc {
    QString icon = QStringLiteral("weather-none-available"), desc = i18n("Unknown");
    ResolvedWeatherDesc() = default;
    ResolvedWeatherDesc(QString icon, QString desc)
    {
        this->icon = icon;
        this->desc = desc;
    }
};

static const QString VERSION_NUMBER = QStringLiteral("0.1.0");

class KWeatherCorePrivate : public QObject
{
    Q_OBJECT

public:
    KWeatherCorePrivate(QObject *parent = nullptr);

    Polygon stringToPolygon(const QString &str);
    QString toFixedString(double num);
    QDir getCacheDirectory(double latitude, double longitude);

    AlertInfo::Urgency urgencyStringToEnum(const QString &str);
    AlertInfo::Severity severityStringToEnum(const QString &str);
    AlertInfo::Certainty certaintyStringToEnum(const QString &str);
    QString urgencyToString(AlertInfo::Urgency urgency);
    QString severityToString(AlertInfo::Severity severity);
    QString certaintyToString(AlertInfo::Certainty certainty);

    int weatherIconPriorityRank(const QString &icon);
    static ResolvedWeatherDesc resolveAPIWeatherDesc(const QString &desc);

    // rank weather (for what best describes the day overall)
    const QHash<QString, int> WEATHER_ICON_PRIORITY_RANK = {// only need neutral icons
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

    // Parameters supported by different CAP providers. Key is the country shorthand
    const QMap<QString, QVector<QString>> CAP_PARAMS = {{QStringLiteral("NOR"),
                                                         {
                                                             QStringLiteral("county"),
                                                             QStringLiteral("cap"),
                                                             QStringLiteral("lang"),
                                                             QStringLiteral("event"),
                                                             QStringLiteral("incidentName"),
                                                             QStringLiteral("geographicDomain"),
                                                             QStringLiteral("municipality"),
                                                             QStringLiteral("lat"),
                                                             QStringLiteral("long"),
                                                             QStringLiteral("show"),
                                                         }}};

    // URLs for CAP alerts for different countries.
    // Country codes according to https://en.wikipedia.org/wiki/ISO_3166-1_alpha-3
    const QMap<QString, QUrl> CAP_URLS = {{QStringLiteral("NOR"), QUrl(QStringLiteral("https://api.met.no/weatherapi/metalerts/1.1/"))}};
};

static KWeatherCorePrivate *self()
{
    static KWeatherCorePrivate singleton;
    return &singleton;
}

}
