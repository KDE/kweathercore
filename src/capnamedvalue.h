/*
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef KWEATHERCORE_CAPNAMEDVALUE_H
#define KWEATHERCORE_CAPNAMEDVALUE_H

#include <kweathercore/kweathercore_export.h>

#include <QMetaType>
#include <QString>

namespace KWeatherCore
{

/** CAP key/value pairs as found in area geo codes, alert event codes or alert parameters. */
class KWEATHERCORE_EXPORT CAPNamedValue
{
    Q_GADGET
    Q_PROPERTY(QString name)
    Q_PROPERTY(QString value)
    Q_PROPERTY(float radius MEMBER radius)

public:
    QString name;
    QString value;
};
}

Q_DECLARE_METATYPE(KWeatherCore::CAPNamedValue)

#endif // KWEATHERCORE_CAPNAMEDVALUE_H
