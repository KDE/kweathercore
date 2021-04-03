/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "feedparser.h"
#include <QObject>
#include "kweathercore/kweathercore_export.h"
namespace KWeatherCore
{
class PendingAlerts;
class KWEATHERCORE_EXPORT AlertManager : public QObject
{
    Q_OBJECT
public:
    static AlertManager *inst();
    QStringList availableCountries() const; // return supported countries
    PendingAlerts *
    getAlerts(const QString &country) const; // return PendingAlerts for
                                             // corresponding country
private:
    AlertManager();
    void loadConfigs(); // load configs on computer
};
}
