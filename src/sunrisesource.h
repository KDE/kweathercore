/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once
#include "sunrise.h"
#include <QDateTime>
#include <QObject>
#include <kweathercore/kweathercore_export.h>
class QNetworkAccessManager;
class QNetworkReply;
namespace KWeatherCore
{
/**
 * @short The SunriseSource class can obtain the sunrise data of one location for several days
 *
 * @see Sunrise
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT SunriseSource : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief SunriseSource
     * @param latitude
     * @param longitude
     * @param offset offset from UTC, in seconds
     * @param sunrise for caching purpose
     * @param parent
     */
    SunriseSource(double latitude, double longitude, int offset, const QVector<Sunrise> &sunrise = QVector<Sunrise>(), QObject *parent = nullptr);
    /**
     * @brief requestData start downloading data
     */
    void requestData();
    /**
     * @brief setOffset change the offset in case of daylight time changing and something
     * @param offset
     */
    void setOffset(int offset);
    /**
     * @brief value get the value
     * @return
     */
    const QVector<Sunrise> &value() const;
Q_SIGNALS:
    /**
     * @brief networkError network error
     */
    void networkError();
    /**
     * @brief finished query finished
     */
    void finished();
private Q_SLOTS:
    void parseResults(QNetworkReply *reply);

private:
    double m_latitude, m_longitude;
    int m_offset;
    QVector<Sunrise> m_sunriseVec;
    QNetworkAccessManager *m_manager = nullptr;

    void popDay();
};
}
