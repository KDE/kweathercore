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
class PendingCAPPrivate : public QObject
{
    Q_OBJECT
public:
    PendingCAPPrivate(QNetworkReply *reply = nullptr, QObject *parent = nullptr);
    bool isFinished = false;
    QByteArray data;
Q_SIGNALS:
    void networkError();
    void finished();

private:
    void parseCAP(QNetworkReply *reply);
};
PendingCAPPrivate::PendingCAPPrivate(QNetworkReply *reply, QObject *parent)
    : QObject(parent)
{
    if (reply) {
        connect(reply, &QNetworkReply::finished, this, [this, reply] {
            this->parseCAP(reply);
        });
        connect(reply, &QNetworkReply::errorOccurred, this, &PendingCAPPrivate::networkError);
    }
}

void PendingCAPPrivate::parseCAP(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        qWarning() << "network error when fetching alerts:" << reply->errorString();
        return;
    }

    data = reply->readAll();
    isFinished = true;
    Q_EMIT finished();
}
std::unique_ptr<AlertEntry> PendingCAP::value() const
{
    if (d->data.isEmpty()) {
        return {};
    }

    CAPParser parser(d->data);
    return parser.parse();
}
PendingCAP::PendingCAP(QNetworkReply *reply)
    : d(new PendingCAPPrivate(reply, this))
{
    connect(d, &PendingCAPPrivate::finished, this, &PendingCAP::finished);
    connect(d, &PendingCAPPrivate::networkError, this, &PendingCAP::networkError);
}
bool PendingCAP::isFinished() const
{
    return d->isFinished;
}
}
#include "pendingcap.moc"
