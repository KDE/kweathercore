/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
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
 * @short The SunriseSource class can obtain the sunrise data of one location
 * for several days
 *
 * @see Sunrise
 *
 * @author Han Young <hanyoung@protonmail.com>
 * @deprecated since 0.6 Use KHolidays::SunRiseSet and/or KHolidays::LunarPhase instead.
 */
class KWEATHERCORE_DEPRECATED_EXPORT SunriseSource : public QObject
{
    Q_OBJECT
public:
    /**
     * SunriseSource
     * @param timezone timezone ID
     * @param sunrise for caching purpose
     */
    SunriseSource(double latitude,
                  double longitude,
                  const QString &timezone = QString(),
                  const std::vector<Sunrise> &sunrise = std::vector<Sunrise>(),
                  QObject *parent = nullptr);
    /**
     * start downloading data
     */
    void requestData();
    /**
     * change the timezone
     */
    void setTimezone(const QString &timezone);
    /**
     * get the value
     * @return
     */
    const std::vector<Sunrise> &value() const;
Q_SIGNALS:
    /**
     * network error
     */
    void networkError();
    /**
     * query finished
     */
    void finished();
private Q_SLOTS:
    void parseResults(QNetworkReply *reply);

private:
    double m_latitude, m_longitude;
    QString m_timezone;
    std::vector<Sunrise> m_sunriseVec;
    QNetworkAccessManager *m_manager = nullptr;

    void popDay();
};
}
