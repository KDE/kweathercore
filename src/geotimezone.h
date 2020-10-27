/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QObject>
class QNetworkReply;
class QNetworkAccessManager;
namespace KWeatherCore
{
class GeoTimezone : public QObject
{
    Q_OBJECT
public:
    GeoTimezone(double lat, double lon, QObject *parent = nullptr);
    QString getTimeZone();

Q_SIGNALS:
    void finished(QString timezone);
    void networkError();
private Q_SLOTS:
    void downloadFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager = nullptr;
};
}
