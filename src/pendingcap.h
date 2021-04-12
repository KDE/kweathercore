/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include <QObject>
#include <memory>
#include "alertentry.h"
#include "kweathercore/kweathercore_export.h"
class QNetworkReply;
namespace KWeatherCore {
class PendingCAPPrivate;
class KWEATHERCORE_EXPORT PendingCAP : public QObject
{
    Q_OBJECT
public:
    std::unique_ptr<AlertEntry> value() const;
    bool isFinished() const;
Q_SIGNALS:
    void finished();
    void networkError();
protected:
    friend class AlertFeedEntry;
    explicit PendingCAP(QNetworkReply *reply = nullptr);
private:
    PendingCAPPrivate *d = nullptr;
};
}
