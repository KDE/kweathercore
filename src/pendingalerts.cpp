/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "pendingalerts.h"
#include <QNetworkReply>
namespace KWeatherCore
{
class PendingAlertsPrivate
{
public:
    AlertEntries alertEntries;
    FeedParser *parser = nullptr;
};

PendingAlerts::PendingAlerts(const QJsonDocument &config, QNetworkReply *reply)
    : d(new PendingAlertsPrivate)
{
    d->parser = new FeedParser(config, this);
    if (reply) {
        connect(reply, &QNetworkReply::finished, this, [this, reply] {
            reply->deleteLater();
            if (reply->error()) {
                qWarning() << "network error when fetching alerts:" << reply->errorString();
                Q_EMIT networkError();
                return;
            }
            d->alertEntries = d->parser->parse(reply->readAll());
            Q_EMIT finished();
        });
    }
}

PendingAlerts::~PendingAlerts() = default;

AlertEntries PendingAlerts::value() const
{
    return d->alertEntries;
}
}
#include "pendingalerts.moc"
