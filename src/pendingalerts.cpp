/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "pendingalerts.h"
#include "reply_p.h"

#include <QNetworkReply>

namespace KWeatherCore
{
class PendingAlertsPrivate : public ReplyPrivate
{
public:
    AlertEntries alertEntries;
    FeedParser *parser = nullptr;
};

PendingAlerts::PendingAlerts(const QJsonDocument &config, QNetworkReply *reply, QObject *parent)
    : Reply(new PendingAlertsPrivate, parent)
{
    Q_D(PendingAlerts);
    d->parser = new FeedParser(config, this);
    if (reply) {
        connect(reply, &QNetworkReply::finished, this, [this, reply] {
            Q_D(PendingAlerts);
            reply->deleteLater();
            if (reply->error() != QNetworkReply::NoError) {
                qWarning() << "network error when fetching alerts:" << reply->errorString();
                d->setError(PendingAlerts::NetworkError, reply->errorString());
            } else {
                d->alertEntries = d->parser->parse(reply->readAll());
            }
            Q_EMIT finished();
        });
    }
}

PendingAlerts::~PendingAlerts() = default;

AlertEntries PendingAlerts::value() const
{
    Q_D(const PendingAlerts);
    return d->alertEntries;
}
}

#include "moc_pendingalerts.cpp"
