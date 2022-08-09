/*
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <kweathercore/kweathercore_export.h>

#include <QXmlStreamReader>

namespace KWeatherCore
{
class AlertEntry;
class AlertInfo;

/**
 * Parser for CAP Alert Messages
 *
 * @see https://docs.oasis-open.org/emergency/cap/v1.2/CAP-v1.2.html
 */
class KWEATHERCORE_EXPORT CAPParser
{
public:
    explicit CAPParser(const QByteArray &data);
    AlertEntry parse();

private:
    AlertInfo parseInfo();
    QXmlStreamReader m_xml;
};
}
