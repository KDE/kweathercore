/*
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "reply.h"
#include "reply_p.h"

using namespace KWeatherCore;

void ReplyPrivate::setError(Reply::Error error, const QString &msg)
{
    m_error = error;
    m_errorMessage = msg;
}

Reply::Reply(ReplyPrivate *dd, QObject *parent)
    : QObject(parent)
    , d_ptr(dd)
{
}

Reply::~Reply() = default;

Reply::Error Reply::error() const
{
    return d_ptr->m_error;
}

QString Reply::errorMessage() const
{
    return d_ptr->m_errorMessage;
}

#include "moc_reply.cpp"
