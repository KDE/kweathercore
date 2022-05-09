/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "pendingcap.h"
#include "capparser.h"
#include <QNetworkReply>
namespace KWeatherCore
{
class PendingCAPPrivate
{
public:
    QByteArray data;
};

std::unique_ptr<AlertEntry> PendingCAP::value() const
{
    if (d->data.isEmpty()) {
        return {};
    }

    CAPParser parser(d->data);
    return parser.parse();
}
PendingCAP::PendingCAP(QNetworkReply *reply)
    : d(new PendingCAPPrivate)
{
    if (reply) {
        connect(reply, &QNetworkReply::finished, this, [this, reply] {
            reply->deleteLater();
            if (reply->error()) {
                qWarning() << "network error when fetching alerts:" << reply->errorString();
                return;
            }

            d->data = reply->readAll();
            Q_EMIT finished();
        });
        connect(reply, &QNetworkReply::errorOccurred, this, &PendingCAP::networkError);
    }
}

PendingCAP::~PendingCAP() = default;
}
