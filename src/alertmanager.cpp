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
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QUrl>
namespace KWeatherCore
{
class AlertManager::AlertManagerPrivate
{
public:
    AlertManagerPrivate();
    AlertManagerPrivate(const AlertManagerPrivate &other);
    ~AlertManagerPrivate();
    AlertManagerPrivate &operator=(const AlertManagerPrivate &other);
    QNetworkAccessManager *manager = nullptr;
    QHash<QString, std::pair<QString, QString>> hash;
};
AlertManager::AlertManagerPrivate::AlertManagerPrivate()
    : manager(new QNetworkAccessManager())
{
}
AlertManager::AlertManagerPrivate::~AlertManagerPrivate()
{
    if (manager)
        manager->deleteLater();
}
AlertManager::AlertManagerPrivate::AlertManagerPrivate(
    const AlertManagerPrivate &other)
    : manager(new QNetworkAccessManager())
{
    hash = other.hash;
}
AlertManager::AlertManagerPrivate &
AlertManager::AlertManagerPrivate::operator=(const AlertManagerPrivate &other)
{
    hash = other.hash;
    return *this;
}
AlertManager::~AlertManager() = default;
AlertManager::AlertManager(const AlertManager &other)
    : d(std::make_unique<AlertManagerPrivate>(*other.d))
{
}
AlertManager &AlertManager::operator=(const AlertManager &other)
{
    *d = *other.d;
    return *this;
}
AlertManager &AlertManager::operator=(AlertManager &&other) = default;
AlertManager *AlertManager::inst()
{
    static AlertManager singleton;
    return &singleton;
}
AlertManager::AlertManager()
    : d(std::make_unique<AlertManagerPrivate>())
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
                d->hash[key.toString()] = std::make_pair(
                    it.filePath(), config[QStringLiteral("url")].toString());
            }
        }
    }
}
QList<QString> AlertManager::availableCountries() const
{
    return d->hash.keys();
}
PendingAlerts *AlertManager::getAlerts(const QString &country) const
{
    QFile file(d->hash.value(country).first);
    file.open(QIODevice::ReadOnly);
    QByteArray val = file.readAll();
    QJsonDocument config = QJsonDocument::fromJson(val);
    QUrl url(((d->hash.value(country)).second));
    auto reply = d->manager->get(QNetworkRequest(url));
    return new PendingAlerts(config, reply);
}
}
