/*
 * SPDX-FileCopyrightText: ⓒ 2025 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "capresource.h"

#include <QMimeDatabase>
#include <QMimeType>
#include <QUrl>

namespace KWeatherCore
{
class CAPResourcePrivate : public QSharedData
{
public:
    QString description;
    QString mimeType;
    qsizetype size = -1;
    QUrl uri;
    QByteArray digest;
};
}

using namespace KWeatherCore;

CAPResource::CAPResource()
    : d(new CAPResourcePrivate)
{
}

CAPResource::CAPResource(const CAPResource &) = default;
CAPResource::CAPResource(CAPResource &&) noexcept = default;

CAPResource::~CAPResource() = default;

CAPResource &CAPResource::operator=(const CAPResource &) = default;
CAPResource &CAPResource::operator=(CAPResource &&) noexcept = default;

QString CAPResource::description() const
{
    return d->description;
}

void CAPResource::setDescription(const QString &description)
{
    d->description = description;
}

QString CAPResource::mimeTypeName() const
{
    return d->mimeType;
}

QMimeType CAPResource::mimeType() const
{
    QMimeDatabase db;
    return db.mimeTypeForName(d->mimeType);
}

void CAPResource::setMimeTypeName(const QString &mimeType)
{
    d->mimeType = mimeType;
}

qsizetype CAPResource::size() const
{
    return d->size;
}

bool CAPResource::hasSize() const
{
    return d->size > 0;
}

void CAPResource::setSize(qsizetype size)
{
    d->size = size;
}

QUrl CAPResource::uri() const
{
    return d->uri;
}

void CAPResource::setUri(const QUrl &uri)
{
    d->uri = uri;
}

QByteArray CAPResource::digest() const
{
    return d->digest;
}

void CAPResource::setDigest(const QByteArray &digest)
{
    d->digest = digest;
}

#include "moc_capresource.cpp"
