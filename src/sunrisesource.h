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
class SunriseSource : public QObject
{
    Q_OBJECT
public:
    SunriseSource(double latitude, double longitude, int offset, const QVector<Sunrise> &sunrise = QVector<Sunrise>(), QObject *parent = nullptr);
    void requestData();
    void setOffset(int offset);

    const QVector<Sunrise> &value() const;
Q_SIGNALS:
    void networkError();
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
