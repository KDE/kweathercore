/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "alertfeedentry.h"
#include "kweathercore_p.h"
#include <QUrl>
namespace KWeatherCore
{
class AlertFeedEntry::AlertFeedEntryPrivate : public QObject
{
    Q_OBJECT
public:
    AlertFeedEntryPrivate(QObject *parent)
        : QObject(parent)
    {
    }
    QString title;
    QString summary;
    QString area;
    AlertEntry::Urgency urgency;
    AlertEntry::Severity severity;
    AlertEntry::Certainty certainty;
    QDateTime date;
    QUrl CAPUrl;
    AreaCodeVec areaCodes;
    Polygon polygon;
};

AlertFeedEntry::AlertFeedEntry(QObject *parent)
    : QObject(parent)
    , d(new AlertFeedEntryPrivate(this))
{
}
const QString &AlertFeedEntry::title() const
{
    return d->title;
}
const QString &AlertFeedEntry::summary() const
{
    return d->summary;
}
const QString &AlertFeedEntry::area() const
{
    return d->area;
}
QString AlertFeedEntry::urgency() const
{
    return urgencyToString(d->urgency);
}
QString AlertFeedEntry::severity() const
{
    return severityToString(d->severity);
}
QString AlertFeedEntry::certainty() const
{
    return certaintyToString(d->certainty);
}
const QDateTime &AlertFeedEntry::date() const
{
    return d->date;
}
const QUrl &AlertFeedEntry::CAPUrl() const
{
    return d->CAPUrl;
}
const AreaCodeVec &AlertFeedEntry::areaCodes() const
{
    return d->areaCodes;
}
const Polygon &AlertFeedEntry::polygon() const
{
    return d->polygon;
}

void AlertFeedEntry::setTitle(const QString &title)
{
    d->title = title;
}
void AlertFeedEntry::setSummary(const QString &summary)
{
    d->summary = summary;
}
void AlertFeedEntry::setArea(const QString &area)
{
    d->area = area;
}
void AlertFeedEntry::setUrgency(AlertEntry::Urgency urgency)
{
    d->urgency = urgency;
}
void AlertFeedEntry::setCertainty(AlertEntry::Certainty certainty)
{
    d->certainty = certainty;
}
void AlertFeedEntry::setSeverity(AlertEntry::Severity severity)
{
    d->severity = severity;
}
void AlertFeedEntry::setDate(const QDateTime &date)
{
    d->date = date;
}
void AlertFeedEntry::setUrl(const QUrl &url)
{
    d->CAPUrl = url;
}
void AlertFeedEntry::setAreaCodes(const AreaCodeVec &areaCodes)
{
    d->areaCodes = areaCodes;
}
void AlertFeedEntry::setPolygon(const Polygon &polygon)
{
    d->polygon = polygon;
}
void AlertFeedEntry::setAreaCodes(AreaCodeVec &&areaCodes)
{
    d->areaCodes = std::move(areaCodes);
}
void AlertFeedEntry::setPolygon(Polygon &&polygon)
{
    d->polygon = std::move(polygon);
}
}
#include "alertfeedentry.moc"
