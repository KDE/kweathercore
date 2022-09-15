/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <KLocalizedString>

#include <QDir>
#include <QString>

#include "alertinfo.h"
#include "caparea.h"

namespace KWeatherCore
{

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

class KWeatherCorePrivate
{
public:
    static CAPPolygon stringToPolygon(const QString &str);
    static QString toFixedString(double num);
    static QDir getCacheDirectory(double latitude, double longitude);

    static AlertInfo::Urgency urgencyStringToEnum(const QString &str);
    static AlertInfo::Severity severityStringToEnum(const QString &str);
    static AlertInfo::Certainty certaintyStringToEnum(const QString &str);
    static QString urgencyToString(AlertInfo::Urgency urgency);
    static QString severityToString(AlertInfo::Severity severity);
    static QString certaintyToString(AlertInfo::Certainty certainty);

    static int weatherIconPriorityRank(const QString &icon);
    static ResolvedWeatherDesc resolveAPIWeatherDesc(const QString &desc);
};
}
