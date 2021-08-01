/*
 * SPDX-FileCopyrightText: 2021 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "alertinfo.h"
#include "kweathercore/kweathercore_export.h"
#include "pendingcap.h"
#include <QObject>
#include <memory>
namespace KWeatherCore
{
// code name (FIPS6, UGC...)/code value (002050, AKZ155)
using AreaCodeVec = std::vector<std::pair<QString, QString>>;
using Polygon = std::vector<std::pair<float, float>>;
/**
 * @short Class represents single CAP
 *
 * This class contains the parsed CAP FEED entry
 *
 * @author Han Young <hanyoung@protonmail.com>
 */
class KWEATHERCORE_EXPORT AlertFeedEntry
{
    Q_GADGET
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString summary READ summary)
    Q_PROPERTY(QString area READ area)
    Q_PROPERTY(QString urgency READ urgency)
    Q_PROPERTY(QString severity READ severity)
    Q_PROPERTY(QString certainty READ certainty)
    Q_PROPERTY(QDateTime date READ date)
public:
    /**
     * default constructor
     */
    AlertFeedEntry();
    /**
     * copy constructor
     */
    AlertFeedEntry(const AlertFeedEntry &other);
    AlertFeedEntry(AlertFeedEntry &&other);
    ~AlertFeedEntry();
    /**
     * title of this entry
     */
    const QString &title() const;
    /**
     * summary of this entry
     */
    const QString &summary() const;
    /**
     * area name, for accurate location
     * use @AreaCodes if possible
     */
    const QString &area() const;
    /**
     * urgency
     * @return localized, "Unknown" by defaut
     */
    QString urgency() const;
    /**
     * severity
     * @return localized, "Unknown" by defaut
     */
    QString severity() const;
    /**
     * certainty
     * @return localized, "Unknown" by defaut
     */
    QString certainty() const;
    /**
     * could be date of the alert or the date of this message
     * refer to CAP for accurate date
     * see @CAP
     */
    const QDateTime &date() const;
    /**
     * CAP, request to download CAP file
     * @return it is the client's responsibility to delete the
     * PendingCAP afterward to avoid memory leak
     */
    PendingCAP *CAP() const;
    /**
     * areaCodes
     * @return pairs of QString, the first one is code type,
     * ie. {"UGC", "AKZ017 AKZ020 AKZ021 AKZ022 AKZ023"}
     */
    const AreaCodeVec &areaCodes() const;
    /**
     * area polygon
     * @return latitude longitude pairs
     */
    const Polygon &polygon() const;

    void setTitle(const QString &title);
    void setSummary(const QString &summary);
    void setArea(const QString &area);
    void setUrgency(AlertInfo::Urgency urgency);
    void setCertainty(AlertInfo::Certainty certainty);
    void setSeverity(AlertInfo::Severity severity);
    void setDate(const QDateTime &date);
    void setUrl(const QUrl &url);
    void setAreaCodes(const AreaCodeVec &areaCodes);
    void setPolygon(const Polygon &polygon);
    void setAreaCodes(AreaCodeVec &&areaCodes);
    void setPolygon(Polygon &&polygon);
    AlertFeedEntry &operator=(const AlertFeedEntry &other);
    AlertFeedEntry &operator=(AlertFeedEntry &&other);

private:
    class AlertFeedEntryPrivate;
    std::unique_ptr<AlertFeedEntryPrivate> d;
};
}
