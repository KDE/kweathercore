/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "alertmanager.h"
#include "pendingalerts.h"
#include <QDirIterator>
#include <QFile>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QUrl>
namespace KWeatherCore
{
AlertManager *AlertManager::inst()
{
    static AlertManager singleton;
    return &singleton;
}
AlertManager::AlertManager()
    : m_manager(new QNetworkAccessManager(this))
{
}
void AlertManager::loadConfigs()
{
    QString config =
        QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    QDir dir(config + QStringLiteral("/kweathercore"));
    if (dir.exists()) {
        QDirIterator it(config + QStringLiteral("kweathercore"));
        while (it.hasNext()) {
            QFile file(it.next());
            if ((it.fileName()).right(4) == QStringLiteral("json")) {
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                auto config = QJsonDocument::fromJson(file.readAll()).object();
                QJsonValue key = config.value(QStringLiteral("country"));
                m_hash[key.toString()] = std::make_pair(
                    it.filePath(), config[QStringLiteral("url")].toString());
            }
        }
    }
}
QList<QString> AlertManager::availableCountries() const
{
    return m_hash.keys();
}
PendingAlerts *AlertManager::getAlerts(const QString &country) const
{
    QFile file(m_hash.value(country).first);
    file.open(QIODevice::ReadOnly);
    QByteArray val = file.readAll();
    QJsonDocument config = QJsonDocument::fromJson(val);
    QUrl url(((m_hash.value(country)).second));
    auto reply = m_manager->get(QNetworkRequest(url));
    return new PendingAlerts(config, reply);
}
}
