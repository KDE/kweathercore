/*
* SPDX-FileCopyrightText: 2020-2021 Anjani Kumar <anjanik012@gmail.com>
* SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QObject>

class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

namespace KWeatherCore {
    class AbstractCAPProvider : public QObject {
        Q_OBJECT

    public:
        explicit AbstractCAPProvider(QObject *);
        AbstractCAPProvider(QObject *, QString);
        void setCountry(const QString &);

    public Q_SLOTS:
        void replyFinished(QNetworkReply *reply);

    private:
        QNetworkAccessManager *m_manager;
        QNetworkRequest *m_networkRequest;
        QNetworkReply *m_networkReply;
        const QString m_country;    // Short hand of a m_country name. Ex- NO for Norway

    };

}