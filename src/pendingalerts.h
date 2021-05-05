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
/**
 * @short The PendingAlerts class contains the reply to an asynchronous
 * CAP feed request.
 *
 * @see AlertFeedEntry
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT PendingAlerts : public QObject
{
    Q_OBJECT
public:
    /**
     * value pointer to the shared alerts data
     * the pointer is nullptr until finished() raised
     * @return
     */
    AlertEntries value() const;
    /**
     * isFinished if the call has finished
     * @return
     */
    bool isFinished() const;
Q_SIGNALS:
    /**
     * signals the call has finished
     */
    void finished();
    /**
     * indicate there is a network error
     */
    void networkError();

protected:
    friend class AlertManager;
    explicit PendingAlerts(const QJsonDocument &config,
                           QNetworkReply *reply = nullptr);

private:
    PendingAlertsPrivate *d = nullptr;
};
}