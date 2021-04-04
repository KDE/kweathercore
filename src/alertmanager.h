/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "feedparser.h"
#include "kweathercore/kweathercore_export.h"
#include <QHash>
#include <QList>
#include <QNetworkAccessManager>
#include <QObject>
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
class KWEATHERCORE_EXPORT AlertManager : public QObject
{
    Q_OBJECT
public:
    /** inst
     * return a pointer to the global instance
     * @return 
     */
    static AlertManager *inst();
    /** availableCountries
     * @return names of all available countries supported 
     */    
    QList<QString> availableCountries() const;
    /** getAlerts
     *  @param country country for which alerts have to be received
     *  @return it is the client's responsibility to delete the
     *  PendingAlerts afterhand to avoid memory leak
     */    
    PendingAlerts *
    getAlerts(const QString &country) const; 
                                             
private:
    AlertManager();
    QNetworkAccessManager *m_manager = nullptr;
    QHash<QString, std::pair<QString, QString>> m_hash;
    void loadConfigs(); 
};
}
