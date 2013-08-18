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

#ifndef __PACKAGE_H
#define __PACKAGE_H

#include <QObject>
#include "packageinfo.h"
#include "detailsinfo.h"
#include "updatedetails.h"

class Package : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY changed)
    Q_PROPERTY(QString displayName READ displayName NOTIFY changed)
    Q_PROPERTY(QString version READ version NOTIFY changed)
    Q_PROPERTY(bool isMarked READ isMarked NOTIFY changed)

public:
    enum DataAvailability { NotApplicable, Fetching, Available, NotAvailable };
    enum Marking { NoOperation = 0, Install = 1, Update = 2, Uninstall = 4 };
        Q_DECLARE_FLAGS(Markings, Marking)

    explicit Package(QSharedPointer<PackageInfo> packagePtr, bool isUpdatePackage = false, QObject *parent = 0);

    QString displayName() const;
    QString filterName() const;

    QString name() const;
    QString version() const;

    QSharedPointer<PackageInfo> package() const;
    PackageInfo *basicInfo() const;
    PackageInfo *updateBasicInfo() const;
    DetailsInfo *details() const;
    DetailsInfo *updateDetails() const;
    UpdateDetails *updateInfo() const;

    bool isUpdateAvailable();

    void mark(bool set);
    bool isMarked();

    void setPackage(QSharedPointer<PackageInfo>);

    DataAvailability detailsAvailability();
    DataAvailability updateDetailsAvailability();
    DataAvailability updateInfoAvailability();

    bool operator==(Package other) const;

signals:
    void changed();

public slots:
    void fetchPackageDetails();
    void fetchUpdateDetails();
    void fetchUpdateInfo();

private slots:
    void setPackageDetails(QSharedPointer<DetailsInfo> packagePtr);
    void setUpdateDetails(QSharedPointer<DetailsInfo> packagePtr);
    void setUpdateInfo(const QString &packageID, const QStringList &updates, const QStringList &obsoletes, const QStringList &vendorUrls, const QStringList &bugzillaUrls, const QStringList &cveUrls, PackageKit::Transaction::Restart restart, const QString &updateText, const QString &changelog, PackageKit::Transaction::UpdateState state, const QDateTime &issued, const QDateTime &updated);
    void onFinished(PackageKit::Transaction::Exit status, uint runtime);

private:
    QString m_name;
    bool m_mark;

    QSharedPointer<PackageInfo> m_package;
    QSharedPointer<DetailsInfo> m_detailsPackage;
    QSharedPointer<PackageInfo> m_updatePackage;
    QSharedPointer<DetailsInfo> m_updateDetailsPackage;
    UpdateDetails *m_updateInfo;

    PackageKit::Transaction *m_basicDetailsTransaction;
    PackageKit::Transaction *m_updateDetailsTransaction;
    PackageKit::Transaction *m_updateInfoTransaction;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Package::Markings)

#endif // PACKAGE_H
