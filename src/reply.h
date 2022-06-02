/*
 * SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef KWEATHERCORE_REPLY_H
#define KWEATHERCORE_REPLY_H

#include <kweathercore/kweathercore_export.h>

#include <QObject>

#include <memory>

namespace KWeatherCore
{

class ReplyPrivate;

/** Base class for all asynchronous jobs.
 *  @since 0.6
 */
class KWEATHERCORE_EXPORT Reply : public QObject
{
    Q_OBJECT
public:
    ~Reply();

    /** Possible error states of the job. */
    enum Error {
        NoError,
        NetworkError, ///< Network operation failed
        RateLimitExceeded, ///< Remote API rate limited exceeded
        NotFound, ///< The queried information could not be found by the backend (e.g. unknown location).
        NoService, ///< There is no service available for obtaining the requested information.
    };

    /** Error state of the job. */
    Error error() const;

    /** Error message of the job.
     *  Only valid if error() returns something other than NoError.
     */
    QString errorMessage() const;

Q_SIGNALS:
    /**
     * Emitted once the job has been finished, either successfully or with an error.
     *
     * Connect to this signal for every job you create to obtain its result and
     * delete it eventually.
     */
    void finished();

protected:
    explicit Reply(ReplyPrivate *dd, QObject *parent);

    Q_DECLARE_PRIVATE(Reply)
    std::unique_ptr<ReplyPrivate> d_ptr;
};

}

#endif // KWEATHERCORE_REPLY_H
