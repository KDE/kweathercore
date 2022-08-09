/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "alertfeedentry.h"
#include "kweathercore_p.h"
#include <QNetworkAccessManager>
#include <QUrl>
namespace KWeatherCore
{
class AlertFeedEntry::AlertFeedEntryPrivate
{
public:
    QString title;
    QString summary;
    QString area;
    AlertInfo::Urgency urgency = AlertInfo::Urgency::UnknownUrgency;
    AlertInfo::Severity severity = AlertInfo::Severity::UnknownSeverity;
    AlertInfo::Certainty certainty = AlertInfo::Certainty::UnknownCertainty;
    QDateTime date;
    QUrl CAPUrl;
    AreaCodeVec areaCodes;
    Polygon polygon;
};

AlertFeedEntry::AlertFeedEntry()
    : d(std::make_unique<AlertFeedEntryPrivate>())
{
}
AlertFeedEntry::AlertFeedEntry(const AlertFeedEntry &other)
    : d(std::make_unique<AlertFeedEntryPrivate>(*other.d))
{
}
AlertFeedEntry::AlertFeedEntry(AlertFeedEntry &&other) = default;
AlertFeedEntry::~AlertFeedEntry() = default;
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
    return KWeatherCorePrivate::urgencyToString(d->urgency);
}
QString AlertFeedEntry::severity() const
{
    return KWeatherCorePrivate::severityToString(d->severity);
}
QString AlertFeedEntry::certainty() const
{
    return KWeatherCorePrivate::certaintyToString(d->certainty);
}
const QDateTime &AlertFeedEntry::date() const
{
    return d->date;
}
PendingCAP *AlertFeedEntry::CAP() const
{
    QNetworkAccessManager manager;
    auto reply = manager.get(QNetworkRequest(d->CAPUrl));
    return new PendingCAP(reply);
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
void AlertFeedEntry::setUrgency(AlertInfo::Urgency urgency)
{
    d->urgency = urgency;
}
void AlertFeedEntry::setCertainty(AlertInfo::Certainty certainty)
{
    d->certainty = certainty;
}
void AlertFeedEntry::setSeverity(AlertInfo::Severity severity)
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
AlertFeedEntry &AlertFeedEntry::operator=(const AlertFeedEntry &other)
{
    *this->d = *other.d;
    return *this;
}
AlertFeedEntry &AlertFeedEntry::operator=(AlertFeedEntry &&other) = default;
}
