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
    CAPAlertInfo::Urgency urgency = CAPAlertInfo::Urgency::UnknownUrgency;
    CAPAlertInfo::Severity severity = CAPAlertInfo::Severity::UnknownSeverity;
    CAPAlertInfo::Certainty certainty = CAPAlertInfo::Certainty::UnknownCertainty;
    QDateTime date;
    QUrl CAPUrl;
    AreaCodeVec areaCodes;
    CAPPolygon polygon;
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
CAPAlertInfo::Urgency AlertFeedEntry::urgency() const
{
    return d->urgency;
}
CAPAlertInfo::Severity AlertFeedEntry::severity() const
{
    return d->severity;
}
CAPAlertInfo::Certainty AlertFeedEntry::certainty() const
{
    return d->certainty;
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
const CAPPolygon &AlertFeedEntry::polygon() const
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
void AlertFeedEntry::setUrgency(CAPAlertInfo::Urgency urgency)
{
    d->urgency = urgency;
}
void AlertFeedEntry::setCertainty(CAPAlertInfo::Certainty certainty)
{
    d->certainty = certainty;
}
void AlertFeedEntry::setSeverity(CAPAlertInfo::Severity severity)
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
void AlertFeedEntry::setAreaCodes(AreaCodeVec &&areaCodes)
{
    d->areaCodes = std::move(areaCodes);
}
void AlertFeedEntry::setPolygon(CAPPolygon &&polygon)
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

#include "moc_alertfeedentry.cpp"
