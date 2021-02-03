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
            : m_manager(new QNetworkAccessManager(parent)) {
        connect(m_manager, &QNetworkAccessManager::finished, this, &AbstractCAPProvider::replyFinished);
    }

    AbstractCAPProvider::AbstractCAPProvider(QObject *parent, QString countryCode)
            : m_manager(new QNetworkAccessManager(parent)),
              m_country(std::move(countryCode)) {
        connect(m_manager, &QNetworkAccessManager::finished, this, &AbstractCAPProvider::replyFinished);

        auto url = capUrls[m_country];
        if(url.isEmpty()) {
            qDebug() << "Country unsupported";
        }
        auto urlParams = capParams[m_country];
        for (auto &param : urlParams) {
            // TODO: temporary code.
            if(param == QStringLiteral("county")) {
                QUrlQuery query;
                query.addQueryItem(param, QStringLiteral("03"));
                url.setQuery(query);
            }
        }

        m_networkRequest = new QNetworkRequest(url);
        m_networkRequest->setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        m_networkRequest->setHeader(QNetworkRequest::UserAgentHeader,
                                    QString(QStringLiteral("KWeatherCore/") +
                              VERSION_NUMBER +
                              QStringLiteral(" kde-frameworks-devel@kde.org")));
        m_networkReply = m_manager->get(*m_networkRequest);
    }

    void AbstractCAPProvider::replyFinished(QNetworkReply *reply) {
        // TODO: Start parsing here
        
    }

    void AbstractCAPProvider::setCountry(const QString &countryCode) {

    }

}