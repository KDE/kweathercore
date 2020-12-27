/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include <KWeatherCore/LocationQuery>
using namespace KWeatherCore;

//...
    
    LocationQuery m_locationSource;
    
    // find places called "Oslo"
    m_locationSource.query("Oslo")
    connect(m_locationSource, &LocationQuery::queryFinished, 
            [](const std::vector<LocationQueryResult> &result)
            {
                for(auto location : result)
                {
                    qDebug() << location.toponymName();
                }
            });
    connect(m_locationSource, &LocationQuery::queryError, 
            []{qDebug() << "can't find this place";});
    
    m_locationSource.locate();
    connect(m_locationSource, &LocationQuery::located, 
            [](const LocationQueryResult &result){qDebug() << "your coordinate: " << result.latitude() << ", " << longitude();}
//...
