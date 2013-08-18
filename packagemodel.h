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

#ifndef PACKAGEMODEL_H
#define PACKAGEMODEL_H

#include <QAbstractListModel>
#include <QList>


#include "package.h"
#include "packagemarkings.h"
#include "packageinfo.h"
#include <QTimer>


class PackageModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(int markedCount READ markedCount NOTIFY markedCountChanged)

    void setRoleNames(QHash<int, QByteArray> roles);
public:

    enum PackageRoles {
        DisplayNameRole = Qt::UserRole + 1,
        FilterNameRole,

        PackageObjectRole,
        IsMarkedRole,

        IdRole,
        NameRole,
        VersionRole,
        ArchRole,
        DataRole,
        SummaryRole,
        InfoRole,
        IconRole,

        IsUpdateAvailableRole,

        UpdateIdRole,
        UpdateNameRole,
        UpdateVersionRole,
        UpdateArchRole,
        UpdateDataRole,
        UpdateSummaryRole,
        UpdateInfoRole,
        UpdateIconRole,

        DetailsAvailableRole,
        UpdateDetailsAvailableRole,
        DependsAvailableRole,
        UpdateInfoAvailableRole,

        DetailMask = 1024 + 2048 + 4096 + 8192,

        DetailsRoleMask = 1024,
        DetailsLicenseRole,
        DetailsGroupRole,
        DetailsDescriptionRole,
        DetailsUrlRole,
        DetailsSizeRole,

        UpdateDetailsRoleMask = 2048,
        UpdateDetailsLicenseRole,
        UpdateDetailsGroupRole,
        UpdateDetailsDescriptionRole,
        UpdateDetailsUrlRole,
        UpdateDetailsSizeRole,

        UpdateInfoRoleMask = 4096,
        UpdateUpdatesRole,
        UpdateObsoletesRole,
        UpdateVendorUrlRole,
        UpdateBugzillaUrlRole,
        UpdateCveUrlRole,
        UpdateRestartNeededRole,
        UpdateTextRole,
        UpdateChangeLogRole,
        UpdateStateRole,
        UpdateIssuedRole,
        UpdateUpdatedRole
    };

public:
    explicit PackageModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;

    void addPackage(QSharedPointer<PackageInfo> packagePtr, bool isUpdatePackage = false);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int markedCount();

    Package *findPackage(const QString &name);
    Package *findPackage(QSharedPointer<PackageInfo> packagePtr);

    void clear();

    QList<int> groups();

    PackageMarkings *packageMarkings() { return &m_packageMarkings; }

    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;

signals:
    void countChanged();
    void markedCountChanged();

public slots:
    void mark(int row, bool set);
    void resetMarkings();

private slots:
    void onPackageChanged();
    void flushBuffer();

private:
    QList<Package*> m_packageList;
    QList<Package*> m_packageBuffer;
    PackageMarkings m_packageMarkings;
    QTimer m_addTimer;
    QHash<int, QByteArray> m_roles;
};

#endif // PACKAGEMODEL_H
