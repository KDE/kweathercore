/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "alertmanager.h"
namespace KWeatherCore
{
AlertManager *AlertManager::inst()
{
    static AlertManager singleton;
    return &singleton;
}
AlertManager::AlertManager()
{
}
}
