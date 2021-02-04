/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "alertentry.h"
#include <QObject>
#include <memory>
namespace KWeatherCore
{
// code name (FIPS6, UGC...)/code value (002050, AKZ155)
using AreaCodeVec = std::vector<std::pair<QString, QString>>;
using Polygon = std::vector<std::pair<float, float>>;
class AlertFeedEntry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString summary READ summary)
    Q_PROPERTY(QString area READ area)
    Q_PROPERTY(QString urgency READ urgency)
    Q_PROPERTY(QString severity READ sevirity)
    Q_PROPERTY(QString certainty READ certainty)
    Q_PROPERTY(QDateTime date READ date)
public:
    AlertFeedEntry(QObject *parent = nullptr);
    const QString &title() const;
    const QString &summary() const;
    const QString &area() const;
    QString urgency() const;
    QString severity() const;
    QString certainty() const;
    const QDateTime &date() const;
    const QUrl &CAPUrl() const;
    const AreaCodeVec &areaCodes() const;
    const Polygon &polygon() const;

    void setTitle(const QString &title);
    void setSummary(const QString &summary);
    void setArea(const QString &area);
    void setUrgency(AlertEntry::Urgency urgency);
    void setCertainty(AlertEntry::Certainty certainty);
    void setSeverity(AlertEntry::Severity severity);
    void setDate(const QDateTime &date);
    void setUrl(const QUrl &url);
    void setAreaCodes(const AreaCodeVec &areaCodes);
    void setPolygon(const Polygon &polygon);
    void setAreaCodes(AreaCodeVec &&areaCodes);
    void setPolygon(Polygon &&polygon);

private:
    class AlertFeedEntryPrivate;
    AlertFeedEntryPrivate *d;
};
}
