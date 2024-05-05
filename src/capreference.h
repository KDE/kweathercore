/*
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef KWEATHERCORE_CAPREFERENCE_H
#define KWEATHERCORE_CAPREFERENCE_H

#include <kweathercore/kweathercore_export.h>

#include <QMetaType>
#include <QSharedDataPointer>

namespace KWeatherCore
{

class CAPReferencePrivate;

/** A reference to a previous CAP alert message.
 *  @see https://docs.oasis-open.org/emergency/cap/v1.2/CAP-v1.2.html §3.2.1
 */
class KWEATHERCORE_EXPORT CAPReference
{
    Q_GADGET
    Q_PROPERTY(QString sender READ sender)
    Q_PROPERTY(QString identifier READ identifier)
    Q_PROPERTY(QDateTime sent READ sent)

public:
    explicit CAPReference();
    explicit CAPReference(const QString &sender, const QString &identifier, const QDateTime &sent);
    CAPReference(const CAPReference &);
    CAPReference(CAPReference &&) noexcept;
    ~CAPReference();
    CAPReference &operator=(const CAPReference &);
    CAPReference &operator=(CAPReference &&) noexcept;

    bool operator==(const CAPReference &other) const;

    QString sender() const;
    QString identifier() const;
    QDateTime sent() const;

private:
    QSharedDataPointer<CAPReferencePrivate> d;
};

}

Q_DECLARE_METATYPE(KWeatherCore::CAPReference)

#endif // KWEATHERCORE_CAPREFERENCE_H
