/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
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
    void testLocate();
    void testQuery();

private:
    LocationQuery *d = new LocationQuery();
    QSignalSpy *finished_spy;
    QSignalSpy *networkError_spy;
};
