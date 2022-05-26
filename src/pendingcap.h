/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "alertentry.h"
#include "reply.h"

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
class KWEATHERCORE_EXPORT PendingCAP : public Reply
{
    Q_OBJECT
public:
    ~PendingCAP();

    /**
     * value pointer to the shared alerts data
     * the pointer is nullptr until finished() raised
     * @return
     */
    std::unique_ptr<AlertEntry> value() const;

private:
    friend class AlertFeedEntry;
    explicit PendingCAP(QNetworkReply *reply, QObject *parent = nullptr);
    Q_DECLARE_PRIVATE(PendingCAP)
};
}
