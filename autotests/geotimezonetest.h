/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include <QSignalSpy>
#include <QTest>
class GeoTimezoneTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testTimezone();
};
