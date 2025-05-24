/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <kweathercore/kweathercore_export.h>

#include "weatherforecast.h"

class QTimeZone;

namespace KWeatherCore
{

/*! Parser for api.met.no replies.
 *  \internal only exported for unit tests
 */
class MetNoParser
{
public:
    KWEATHERCORE_EXPORT void parseLocationForecast(const QByteArray &data);
    KWEATHERCORE_EXPORT void applySunriseToForecast(const QTimeZone &timezone);

    WeatherForecast forecast;

    inline bool hasData() const
    {
        return !hourlyForecast.empty();
    }

private:
    void parseOneElement(const QJsonObject &obj);
    bool isDayTime(const QDateTime &dt) const;

    std::vector<HourlyWeatherForecast> hourlyForecast;
};
}
