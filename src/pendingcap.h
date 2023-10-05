/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "reply.h"

class QNetworkReply;

namespace KWeatherCore
{
class CAPAlertMessage;
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
    ~PendingCAP() override;

    /**
     * value pointer to the shared alerts data
     * the pointer is nullptr until finished() raised
     * @return
     */
    CAPAlertMessage value() const;

private:
    friend class AlertFeedEntry;
    explicit PendingCAP(QNetworkReply *reply, QObject *parent = nullptr);
    Q_DECLARE_PRIVATE(PendingCAP)
};
}
