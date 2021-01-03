/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <QObject>
#include <QTest>
#include <kweathercore/sunrise.h>

using namespace KWeatherCore;

class SunriseTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testJson();

private:
    Sunrise d1;
    Sunrise d2;
};
