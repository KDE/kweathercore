/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once
#include <QSignalSpy>
#include <QTest>
#include <kweathercore/locationquery.h>
using namespace KWeatherCore;
class LocationQueryTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testLocate();
    void testQuery();

private:
    LocationQuery *d;
    QSignalSpy *finished_spy;
    QSignalSpy *networkError_spy;
};
