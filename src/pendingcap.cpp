/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "pendingcap.h"
#include "capalertmessage.h"
#include "capparser.h"
#include "reply_p.h"

#include <QNetworkReply>

namespace KWeatherCore
{
class PendingCAPPrivate : public ReplyPrivate
{
public:
    QByteArray data;
};

CAPAlertMessage PendingCAP::value() const
{
    Q_D(const PendingCAP);
    if (d->data.isEmpty()) {
        return {};
    }

    CAPParser parser(d->data);
    return parser.parse();
}
PendingCAP::PendingCAP(QNetworkReply *reply, QObject *parent)
    : Reply(new PendingCAPPrivate, parent)
{
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        Q_D(PendingCAP);
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "network error when fetching alerts:" << reply->errorString();
            d->setError(PendingCAP::NetworkError, reply->errorString());
        } else {
            d->data = reply->readAll();
        }
        Q_EMIT finished();
    });
}

PendingCAP::~PendingCAP() = default;
}
