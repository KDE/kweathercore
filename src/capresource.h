/*
 * SPDX-FileCopyrightText: ⓒ 2025 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef KWEATHERCORE_CAPRESOURCE_H
#define KWEATHERCORE_CAPRESOURCE_H

#include "kweathercore/kweathercore_export.h"

#include <QSharedDataPointer>
#include <qobjectdefs.h>

class QMimeType;
class QUrl;

namespace KWeatherCore
{

class CAPParser;
class CAPResourcePrivate;
;

/** An additional resource for a CAPAlertInfo.
 *  @see https://docs.oasis-open.org/emergency/cap/v1.2/CAP-v1.2.html §1.3.3 and §3.2.3
 *  @since 25.12
 */
class KWEATHERCORE_EXPORT CAPResource
{
    Q_GADGET
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QString mimeTypeName READ mimeTypeName)
    Q_PROPERTY(QMimeType mimeType READ mimeType STORED false)
    Q_PROPERTY(qsizetype size READ size)
    Q_PROPERTY(bool hasSize READ hasSize STORED false)
    Q_PROPERTY(QUrl uri READ uri)
    Q_PROPERTY(QByteArray digest READ digest)
public:
    CAPResource();
    CAPResource(const CAPResource &);
    CAPResource(CAPResource &&) noexcept;
    ~CAPResource();
    CAPResource &operator=(const CAPResource &);
    CAPResource &operator=(CAPResource &&) noexcept;

    /** Description of the type and content of the resource file. */
    [[nodiscard]] QString description() const;
    /** The identifier of the MIME content type and sub-type describing the resource file. */
    [[nodiscard]] QString mimeTypeName() const;
    /** The MIME type describing this resource file. */
    [[nodiscard]] QMimeType mimeType() const;
    /** Approximate size of the resource file in bytes.
     *  -1 indicates size is unknown/unspecified.
     */
    [[nodiscard]] qsizetype size() const;
    /** Returns whether a size for this resource is specified/known. */
    [[nodiscard]] bool hasSize() const;
    /** URI refering to the resource file. */
    [[nodiscard]] QUrl uri() const;
    /** SHA-1 hash of the resource file content as raw bytes. */
    [[nodiscard]] QByteArray digest() const;

private:
    friend class CAPParser;
    void setDescription(const QString &description);
    void setMimeTypeName(const QString &mimeType);
    void setSize(qsizetype size);
    void setUri(const QUrl &uri);
    void setDigest(const QByteArray &digest);

    QSharedDataPointer<CAPResourcePrivate> d;
};

}

#endif
