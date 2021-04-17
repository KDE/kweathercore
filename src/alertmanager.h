/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "kweathercore/kweathercore_export.h"
#include <QList>
#include <QObject>
#include <memory>
class QNetworkAccessManager;
namespace KWeatherCore
{
class PendingAlerts;
/**
 * @short The AlertManager class is intened to get pending weather
 * alerts about a location
 *
 * @see Pendingalerts
 *
 * @author Nikunj Goyal <nikunjgoyal31@gmail.com>
 */
class KWEATHERCORE_EXPORT AlertManager
{
public:
    /** inst
     * return a pointer to the global instance
     * @return
     */
    static AlertManager *inst();
    ~AlertManager();
    AlertManager(const AlertManager &other);
    /** availableCountries
     * @return names of all available countries supported
     */
    QList<QString> availableCountries() const;
    /** getAlerts
     *  @param country country for which alerts have to be received
     *  @return it is the client's responsibility to delete the
     *  PendingAlerts afterhand to avoid memory leak
     */
    PendingAlerts *getAlerts(const QString &country) const;
    AlertManager &operator=(const AlertManager &other);
    AlertManager &operator=(AlertManager &&other);

private:
    AlertManager();
    class AlertManagerPrivate;
    std::unique_ptr<AlertManagerPrivate> d;
    void loadConfigs();
};
}
