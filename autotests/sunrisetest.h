/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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
