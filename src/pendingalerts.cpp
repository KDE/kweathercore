/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "pendingalerts.h"
#include <QNetworkReply>
namespace KWeatherCore
{
class PendingAlertsPrivate : public QObject
{
    Q_OBJECT
public:
    PendingAlertsPrivate(const QJsonDocument &config, QNetworkReply *reply = nullptr, QObject *parent = nullptr);
    AlertEntries alertEntries;
Q_SIGNALS:
    void networkError();
    void finished();

private:
    void parseAlerts(QNetworkReply *reply);
    FeedParser *parser = nullptr;
};
PendingAlertsPrivate::PendingAlertsPrivate(const QJsonDocument &config, QNetworkReply *reply, QObject *parent)
    : QObject(parent)
    , parser(new FeedParser(config, this))
{
    if (reply) {
        connect(reply, &QNetworkReply::finished, this, [this, reply] {
            this->parseAlerts(reply);
        });
    }
}
void PendingAlertsPrivate::parseAlerts(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        qWarning() << "network error when fetching alerts:" << reply->errorString();
        Q_EMIT networkError();
        return;
    }
    alertEntries = parser->parse(reply->readAll());
    Q_EMIT finished();
}
PendingAlerts::PendingAlerts(const QJsonDocument &config, QNetworkReply *reply)
    : d(new PendingAlertsPrivate(config, reply, this))
{
    connect(d, &PendingAlertsPrivate::finished, this, &PendingAlerts::finished);
    connect(d, &PendingAlertsPrivate::networkError, this, &PendingAlerts::networkError);
}
AlertEntries PendingAlerts::value() const
{
    return d->alertEntries;
}
}
#include "pendingalerts.moc"
