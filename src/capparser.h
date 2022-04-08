/*
 * SPDX-FileCopyrightText: 2021 Anjani Kumar <anjanik012@gmail.com>
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "alertentry.h"
#include "alertinfo.h"

#include <QObject>
#include <QXmlStreamReader>

#include <memory>

namespace KWeatherCore
{
/**
 * @internal exported only for unit tests.
 */
class KWEATHERCORE_EXPORT CAPParser
{
public:
    explicit CAPParser(const QByteArray &data = {});
    void setData(const QByteArray &data);
    std::unique_ptr<AlertEntry> parse();

private:
    AlertInfo parseInfo();
    QXmlStreamReader m_xml;
};
}
