/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include <QObject>
#include <kweathercore/kweathercore_export.h>
class QNetworkReply;
class QNetworkAccessManager;
namespace KWeatherCore
{
/**
 * @short Class to obtain timezone via coordinate
 *
 * This is a class to obtain timezone via coordinate and signals finished() with
 * a QString
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT GeoTimezone : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief GeoTimezone
     * @param lat latitude
     * @param lon longitude
     * @param parent
     */
    GeoTimezone(double lat, double lon, QObject *parent = nullptr);

Q_SIGNALS:
    /**
     * @brief finished
     * @param timezone
     */
    void finished(const QString &timezone);
    /**
     * @brief networkError encounted network error
     */
    void networkError();
private Q_SLOTS:
    void downloadFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager = nullptr;
};
}
