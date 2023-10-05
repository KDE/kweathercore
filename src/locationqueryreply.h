/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef KWEATHERCORE_LOCATIONQUERYREPLY_H
#define KWEATHERCORE_LOCATIONQUERYREPLY_H

#include "reply.h"

class QGeoPositionInfoSource;
class QNetworkAccessManager;

namespace KWeatherCore
{

class LocationQueryResult;
class LocationQueryReplyPrivate;

/** Asynchronous reply for a location query.
 *  @see LocationQuery
 *  @since 0.6
 */
class KWEATHERCORE_EXPORT LocationQueryReply : public Reply
{
public:
    ~LocationQueryReply() override;

    /** Result of the location query.
     *  Can be none, one or multiple elements.
     */
    const std::vector<LocationQueryResult> &result() const;

private:
    friend class LocationQuery;
    explicit LocationQueryReply(const QString &name, int number, QNetworkAccessManager *nam, QObject *parent = nullptr);
    explicit LocationQueryReply(QGeoPositionInfoSource *source, QNetworkAccessManager *nam, QObject *parent = nullptr);
    Q_DECLARE_PRIVATE(LocationQueryReply)
};

}

#endif // KWEATHERCORE_LOCATIONQUERYREPLY_H
