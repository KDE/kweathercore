/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef KWEATHERCORE_PENDINGALERTS_H
#define KWEATHERCORE_PENDINGALERTS_H

#include "feedparser.h"
#include "reply.h"

class QJsonDocument;
class QNetworkReply;

namespace KWeatherCore
{
class PendingAlertsPrivate;
using AlertEntries = std::shared_ptr<std::vector<std::unique_ptr<AlertFeedEntry>>>;
/**
 * @short The PendingAlerts class contains the reply to an asynchronous
 * CAP feed request.
 *
 * @see AlertFeedEntry
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT PendingAlerts : public Reply
{
    Q_OBJECT
public:
    ~PendingAlerts() override;

    /**
     * value pointer to the shared alerts data
     * the pointer is nullptr until finished() raised
     * @return
     */
    AlertEntries value() const;

private:
    friend class AlertManager;
    explicit PendingAlerts(const QJsonDocument &config, QNetworkReply *reply, QObject *parent = nullptr);
    Q_DECLARE_PRIVATE(PendingAlerts)
};
}

#endif
