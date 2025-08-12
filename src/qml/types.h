/*
    SPDX-FileCopyrightText: 2025 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KWEATHERCORE_QML_TYPES_H
#define KWEATHERCORE_QML_TYPES_H

#include <KWeatherCore/CAPAlertInfo>
#include <KWeatherCore/CAPAlertMessage>
#include <KWeatherCore/CAPArea>
#include <KWeatherCore/CAPReference>
#include <KWeatherCore/CAPResource>

#include <QQmlEngine>

#define FOREIGN_VALUE_TYPE(Class, Name) \
struct Class ## Foreign { \
    Q_GADGET \
    QML_FOREIGN(KWeatherCore::Class) \
    QML_VALUE_TYPE(Name) \
};

#define FOREIGN_ENUM_GADGET(Class) \
class Class ## Derived: public KWeatherCore::Class \
{ \
    Q_GADGET \
}; \
namespace Class ## DerivedForeign \
{ \
    Q_NAMESPACE \
    QML_NAMED_ELEMENT(Class) \
    QML_FOREIGN_NAMESPACE(Class ## Derived) \
} \


FOREIGN_VALUE_TYPE(CAPAlertInfo, capAlertInfo)
FOREIGN_ENUM_GADGET(CAPAlertInfo)
FOREIGN_VALUE_TYPE(CAPAlertMessage, capAlertMessage)
FOREIGN_ENUM_GADGET(CAPAlertMessage)
FOREIGN_VALUE_TYPE(CAPArea, capArea)
FOREIGN_VALUE_TYPE(CAPCircle, capCircle)
FOREIGN_VALUE_TYPE(CAPNamedValue, capNamedValue)
FOREIGN_VALUE_TYPE(CAPReference, capReference)
FOREIGN_VALUE_TYPE(CAPResource, capResource)

#endif
