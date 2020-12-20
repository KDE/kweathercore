/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "sunrisesource.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

namespace KWeatherCore
{
SunriseSource::SunriseSource(double latitude, double longitude, int offset, const std::vector<Sunrise> &sunrise, QObject *parent)
    : QObject(parent)
    , m_latitude(latitude)
    , m_longitude(longitude)
    , m_offset(offset)
    , m_sunriseVec(sunrise)
    , m_manager(new QNetworkAccessManager(this))
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &SunriseSource::parseResults);
}
void SunriseSource::requestData()
{
    // pop older data
    popDay();

    if (m_sunriseVec.size() >= 10) // don't update if we have enough data
    {
        Q_EMIT finished();
        return;
    }

    QUrl url(QStringLiteral("https://api.met.no/weatherapi/sunrise/2.0/.json"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), QString::number(m_latitude));
    query.addQueryItem(QStringLiteral("lon"), QString::number(m_longitude));
    // if we already have data, request data beyond the last day
    query.addQueryItem(QStringLiteral("date"), !m_sunriseVec.size() ? QDate::currentDate().toString(QStringLiteral("yyyy-MM-dd")) : QDate::currentDate().addDays(m_sunriseVec.size()).toString(QStringLiteral("yyyy-MM-dd")));
    query.addQueryItem(QStringLiteral("days"), !m_sunriseVec.size() ? QString::number(10) : QString::number(11 - m_sunriseVec.size()));

    // calculate offset (form example: -04:00)
    QString offset = m_offset < 0 ? QStringLiteral("-") : QStringLiteral("+");
    int hour = std::abs(m_offset) / 3600;
    if (hour >= 10)
        offset.append(QString::number(hour) + QStringLiteral(":"));
    else {
        offset.append(QStringLiteral("0") + QString::number(hour) + QStringLiteral(":"));
    }
    int min = (std::abs(m_offset) - hour * 3600) / 60;
    if (min >= 10) {
        offset.append(QString::number(min));
    } else {
        offset.append(QStringLiteral("0") + QString::number(min));
    }
    query.addQueryItem(QStringLiteral("offset"), offset);

    url.setQuery(query);
    QNetworkRequest req(url);

    m_manager->get(req);
}
void SunriseSource::parseResults(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        qWarning() << "nmisunriseapi network error:" << reply->errorString();
        Q_EMIT networkError();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonArray array = doc[QStringLiteral("location")].toObject()[QStringLiteral("time")].toArray();

    m_sunriseVec.reserve(array.size());

    for (int i = 0; i <= array.count() - 2; i++) // we don't want last one
    {
        Sunrise sr;
        sr.setSunSet(QDateTime::fromString(array.at(i).toObject()[QStringLiteral("sunset")].toObject()[QStringLiteral("time")].toString().left(19), QStringLiteral("yyyy-MM-ddThh:mm:ss")));
        sr.setSunRise(QDateTime::fromString(array.at(i).toObject()[QStringLiteral("sunrise")].toObject()[QStringLiteral("time")].toString().left(19), QStringLiteral("yyyy-MM-ddThh:mm:ss")));
        sr.setMoonSet(QDateTime::fromString(array.at(i).toObject()[QStringLiteral("moonset")].toObject()[QStringLiteral("time")].toString().left(19), QStringLiteral("yyyy-MM-ddThh:mm:ss")));
        sr.setMoonRise(QDateTime::fromString(array.at(i).toObject()[QStringLiteral("moonrise")].toObject()[QStringLiteral("time")].toString().left(19), QStringLiteral("yyyy-MM-ddThh:mm:ss")));
        sr.setSolarMidnight(QPair<QDateTime, double>(QDateTime::fromString(array.at(i).toObject()[QStringLiteral("solarmidnight")].toObject()[QStringLiteral("time")].toString().left(19), QStringLiteral("yyyy-MM-ddThh:mm:ss")),
                                                     array.at(i).toObject()[QStringLiteral("solarmidnight")].toObject()[QStringLiteral("elevation")].toString().toDouble()));
        sr.setSolarNoon(QPair<QDateTime, double>(QDateTime::fromString(array.at(i).toObject()[QStringLiteral("solarnoon")].toObject()[QStringLiteral("time")].toString().left(19), QStringLiteral("yyyy-MM-ddThh:mm:ss")),
                                                 array.at(i).toObject()[QStringLiteral("solarnoon")].toObject()[QStringLiteral("elevation")].toString().toDouble()));
        sr.setHighMoon(QPair<QDateTime, double>(QDateTime::fromString(array.at(i).toObject()[QStringLiteral("high_moon")].toObject()[QStringLiteral("time")].toString().left(19), QStringLiteral("yyyy-MM-ddThh:mm:ss")),
                                                array.at(i).toObject()[QStringLiteral("high_moon")].toObject()[QStringLiteral("elevation")].toString().toDouble()));
        sr.setLowMoon(QPair<QDateTime, double>(QDateTime::fromString(array.at(i).toObject()[QStringLiteral("low_moon")].toObject()[QStringLiteral("time")].toString().left(19), QStringLiteral("yyyy-MM-ddThh:mm:ss")),
                                               array.at(i).toObject()[QStringLiteral("low_moon")].toObject()[QStringLiteral("elevation")].toString().toDouble()));
        sr.setMoonPhase(array.at(i).toObject()[QStringLiteral("moonposition")].toObject()[QStringLiteral("phase")].toString().toDouble());

        m_sunriseVec.push_back(sr);
    }

    Q_EMIT finished();
}
inline void SunriseSource::setOffset(int offset)
{
    m_offset = offset;
}

void SunriseSource::popDay()
{
    auto today = QDateTime::currentDateTime();
    for (auto day : m_sunriseVec) {
        if (day.sunRise().daysTo(today) > 0) {
            m_sunriseVec.erase(m_sunriseVec.begin());
        } else {
            // since vector is always sorted
            break;
        }
    }
}
}
