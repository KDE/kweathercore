/*
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef KWEATHERCORE_REPLY_P_H
#define KWEATHERCORE_REPLY_P_H

#include "reply.h"

#include <QString>

namespace KWeatherCore
{

class ReplyPrivate
{
public:
    virtual ~ReplyPrivate() = default;
    void setError(Reply::Error error, const QString &msg = {});

    Reply::Error m_error = Reply::NoError;
    QString m_errorMessage;
};

}

#endif // KWEATHERCORE_REPLY_P_H
