/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "alertentry.h"
#include "kweathercore/kweathercore_export.h"
#include <QObject>
#include <memory>
class QNetworkReply;
namespace KWeatherCore
{
class PendingCAPPrivate;
/**
 * @short The PendingAlerts class contains the reply to an asynchronous
 * CAP request.
 *
 * @see AlertEntry
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT PendingCAP : public QObject
{
    Q_OBJECT
public:
    /**
     * value pointer to the shared alerts data
     * the pointer is nullptr until finished() raised
     * @return
     */
    std::unique_ptr<AlertEntry> value() const;
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
    friend class AlertFeedEntry;
    explicit PendingCAP(QNetworkReply *reply = nullptr);

private:
    PendingCAPPrivate *d = nullptr;
};
}
