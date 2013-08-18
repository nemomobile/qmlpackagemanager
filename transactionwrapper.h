/*
 * This file is part of mg-package-manager
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: Kyösti Ranto <kyosti.ranto@digia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#ifndef TRANSACTIONWRAPPER_H
#define TRANSACTIONWRAPPER_H

#include <QObject>

#include "transaction.h"

class TransactionWrapper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString state READ state NOTIFY changed);
    Q_PROPERTY(uint errorCode READ errorCode NOTIFY changed);
    Q_PROPERTY(QString errorText READ errorText NOTIFY changed);
    Q_PROPERTY(bool allowCancel READ allowCancel NOTIFY changed);
    Q_PROPERTY(int percentage READ percentage NOTIFY changed);
    Q_PROPERTY(int subpercentage READ subpercentage NOTIFY changed);
    Q_PROPERTY(uint status READ status NOTIFY changed);
    Q_PROPERTY(QString currentPackage READ currentPackage NOTIFY changed);

public:
    explicit TransactionWrapper(PackageKit::Transaction *transaction, bool readPackages = true, QObject *parent = 0);

    QString state();
    uint errorCode();
    QString errorText();
    bool allowCancel();
    int percentage();
    int subpercentage();
    uint status();
    QString currentPackage();

signals:
    void changed();

public slots:
    void cancel();
    void setState(QString state);
    void setErrorText(const QString &error);

private slots:
    void onChanged();
    void onPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);
    void onErrorCode(PackageKit::Transaction::Error error, const QString &details);
    void onMessage(PackageKit::Transaction::Message type, const QString &message);
    void onRepoSignatureRequired(const QString &packageID, const QString &repoName, const QString &keyUrl, const QString &keyUserid, const QString &keyId, const QString &keyFingerprint, const QString &keyTimestamp, PackageKit::Transaction::SigType type);
    void onFinished(PackageKit::Transaction::Exit status, uint runtime);
    void onDestroyed();

private:
    PackageKit::Transaction *m_transaction;

    QString m_errorText;
    PackageKit::Transaction::Error m_errorCode;

    QString m_state;

    QString m_currentPackage;
};

#endif // TRANSACTIONWRAPPER_H
