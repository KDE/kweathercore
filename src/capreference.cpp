/*
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "capreference.h"

#include <QDateTime>

using namespace KWeatherCore;

namespace KWeatherCore
{
class CAPReferencePrivate : public QSharedData
{
public:
    QString sender;
    QString identifier;
    QDateTime sent;
};
}

CAPReference::CAPReference()
    : d(new CAPReferencePrivate)
{
}

CAPReference::CAPReference(const QString &sender, const QString &identifier, const QDateTime &sent)
    : d(new CAPReferencePrivate)
{
    d->sender = sender;
    d->identifier = identifier;
    d->sent = sent;
}

CAPReference::CAPReference(const CAPReference &) = default;
CAPReference::CAPReference(CAPReference &&) = default;
CAPReference::~CAPReference() = default;

CAPReference &CAPReference::operator=(const CAPReference &) = default;
CAPReference &CAPReference::operator=(CAPReference &&) = default;

bool CAPReference::operator==(const CAPReference &other) const
{
    return d->sender == other.d->sender && d->identifier == other.d->identifier && d->sent == other.d->sent;
}

QString CAPReference::sender() const
{
    return d->sender;
}

QString CAPReference::identifier() const
{
    return d->identifier;
}

QDateTime CAPReference::sent() const
{
    return d->sent;
}

#include "moc_capreference.cpp"
