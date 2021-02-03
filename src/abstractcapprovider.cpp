/*
* SPDX-FileCopyrightText: 2020-2021 Anjani Kumar <anjanik012@gmail.com>
* SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <utility>
#include <QUrlQuery>

#include "abstractcapprovider.h"
#include "kweathercore_p.h"

namespace KWeatherCore {

    AbstractCAPProvider::AbstractCAPProvider(QObject *parent)
            : networkAccessManager(new QNetworkAccessManager(parent)) {
        connect(networkAccessManager, &QNetworkAccessManager::finished, this, &AbstractCAPProvider::replyFinished);
    }

    AbstractCAPProvider::AbstractCAPProvider(QObject *parent, QString countryCode)
            : networkAccessManager(new QNetworkAccessManager(parent)),
              country(std::move(countryCode)) {
        connect(networkAccessManager, &QNetworkAccessManager::finished, this, &AbstractCAPProvider::replyFinished);

        auto url = capUrls[country];
        if(url.isEmpty()) {
            qDebug() << "Country unsupported";
        }
        auto urlParams = capParams[country];
        for (auto &param : urlParams) {
            // TODO: temporary code.
            if(param == QStringLiteral("county")) {
                QUrlQuery query;
                query.addQueryItem(param, QStringLiteral("03"));
                url.setQuery(query);
            }
        }

        networkRequest = new QNetworkRequest(url);
        networkRequest->setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        networkRequest->setHeader(QNetworkRequest::UserAgentHeader,
                      QString(QStringLiteral("KWeatherCore/") +
                              VERSION_NUMBER +
                              QStringLiteral(" kde-frameworks-devel@kde.org")));
        networkReply = networkAccessManager->get(*networkRequest);
    }

    void AbstractCAPProvider::replyFinished(QNetworkReply *reply) {
        // TODO: Start parsing here
        
    }

    void AbstractCAPProvider::setCountry(const QString &countryCode) {

    }

}