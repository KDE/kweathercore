/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include <KWeatherCore/WeatherForecastSource>
using namespace KWeatherCore;

//...

// Get the weatherforecast of Paris
WeatherForecastSource m_source;

auto m_pendingForecast = m_source.requestData(48.86, 2.34);

connect(m_pendingForecast, &PendingWeatherForecast::finished, [m_pendingForecast] {
    auto m_weatherData = m_pendingForecast->value();
    m_pendingForecast->deleteLater();
});
connect(m_pendingForecast, &PendingWeatherForecast::networkError, [] {
    qDebug() << "network error";
});

//...
