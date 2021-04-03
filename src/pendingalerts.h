/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "feedparser.h"
#include <QJsonDocument>
#include <QObject>
#include "kweathercore/kweathercore_export.h"
class QNetworkReply;
namespace KWeatherCore
{
class PendingAlertsPrivate;
using AlertEntries =
    std::shared_ptr<std::vector<std::unique_ptr<AlertFeedEntry>>>;
class KWEATHERCORE_EXPORT PendingAlerts : public QObject
{
    Q_OBJECT
public:
    AlertEntries value() const;
    bool isFinished() const;
Q_SIGNALS:
    void finished();
    void networkError();

protected:
    friend class AlertManager;
    explicit PendingAlerts(const QJsonDocument &config,
                           QNetworkReply *reply = nullptr);

private:
    PendingAlertsPrivate *d = nullptr;
};
}
