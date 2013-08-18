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

#ifndef PACKMANCONTEXT_H
#define PACKMANCONTEXT_H

#include "transactionwrapper.h"

#include "package.h"
#include "packagegroup.h"
#include "repository.h"

//#include <QPackageKit>
#include <QObject>
#include <QQmlListProperty>

class QAbstractItemModel;
class Package;
class PackageList;

class PackManContext : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject *packageManager READ packageManager NOTIFY packageManagerChanged)

    Q_PROPERTY(QObject *installedPackagesModel READ installedPackagesModel NOTIFY installedPackagesModelChanged)
    Q_PROPERTY(QObject *updateAvailablePackagesModel READ updateAvailablePackagesModel NOTIFY updateAvailablePackagesModelChanged)
    Q_PROPERTY(QObject *availablePackagesModel READ availablePackagesModel NOTIFY availablePackagesModelChanged)

    Q_PROPERTY(uint selectedGroup READ selectedGroup NOTIFY selectedGroupChanged)
    Q_PROPERTY(QObject *packageMarkings READ packageMarkings NOTIFY packageMarkingsChanged)

    Q_PROPERTY(QObject *refreshCacheTransaction READ refreshCacheTransaction NOTIFY refreshCacheTransactionChanged)
    Q_PROPERTY(QObject *refreshReposTransaction READ refreshReposTransaction NOTIFY refreshReposTransactionChanged)
    Q_PROPERTY(QObject *getPackagesTransaction READ getPackagesTransaction NOTIFY getPackagesTransactionChanged)
    Q_PROPERTY(QObject *getUpdatesTransaction READ getUpdatesTransaction NOTIFY getUpdatesTransactionChanged)
    Q_PROPERTY(QObject *searchGroupsTransaction READ searchGroupsTransaction NOTIFY searchGroupsTransactionChanged)

    Q_PROPERTY(QObject *updatePackagesTransaction READ updatePackagesTransaction NOTIFY updatePackagesTransactionChanged)
    Q_PROPERTY(QObject *uninstallPackagesTransaction READ uninstallPackagesTransaction NOTIFY uninstallPackagesTransactionChanged)
    Q_PROPERTY(QObject *installPackagesTransaction READ installPackagesTransaction NOTIFY installPackagesTransactionChanged)

    Q_PROPERTY(QDeclarativeListProperty<Package> packagesToBeInstalled READ packagesToBeInstalled NOTIFY packagesToBeInstalledChanged)
    Q_PROPERTY(QDeclarativeListProperty<Package> packagesToBeUpdated READ packagesToBeUpdated NOTIFY packagesToBeUpdatedChanged)
    Q_PROPERTY(QDeclarativeListProperty<Package> packagesToBeRemoved READ packagesToBeRemoved NOTIFY packagesToBeRemovedChanged)

    Q_PROPERTY(QDeclarativeListProperty<PackageGroup> packageGroups READ packageGroups NOTIFY packageGroupsChanged)

    Q_PROPERTY(QDeclarativeListProperty<Repository> repositories READ repositories NOTIFY repositoriesChanged)

public:
    explicit PackManContext(QObject *parent = 0);

    void setPackageManager(QObject *manager);
    QObject *packageManager();

    void setInstalledPackagesModel(QAbstractItemModel *model);
    QAbstractItemModel *installedPackagesModel();

    void setUpdateAvailablePackagesModel(QAbstractItemModel *model);
    QAbstractItemModel *updateAvailablePackagesModel();

    void setAvailablePackagesModel(QAbstractItemModel *model);
    QAbstractItemModel *availablePackagesModel();

    void setSelectedGroup(uint group);
    uint selectedGroup();

    void setPackageMarkings(QObject *markings);
    QObject *packageMarkings();

    void setRefreshCacheTransaction(TransactionWrapper *transaction);
    QObject *refreshCacheTransaction();

    void setRefreshReposTransaction(TransactionWrapper *transaction);
    QObject *refreshReposTransaction();

    void setGetPackagesTransaction(TransactionWrapper *transaction);
    QObject *getPackagesTransaction();

    void setGetUpdatesTransaction(TransactionWrapper *transaction);
    QObject *getUpdatesTransaction();

    void setSearchGroupsTransaction(TransactionWrapper *transaction);
    QObject *searchGroupsTransaction();

    void setUpdatePackagesTransaction(TransactionWrapper *transaction);
    QObject *updatePackagesTransaction();

    void setUninstallPackagesTransaction(TransactionWrapper *transaction);
    QObject *uninstallPackagesTransaction();

    void setInstallPackagesTransaction(TransactionWrapper *transaction);
    QObject *installPackagesTransaction();

    void setPackagesToBeInstalled(PackageList *list);
    QQmlListProperty<Package> packagesToBeInstalled();

    void setPackagesToBeUpdated(PackageList *list);
    QQmlListProperty<Package> packagesToBeUpdated();

    void setPackagesToBeRemoved(PackageList *list);
    QQmlListProperty<Package> packagesToBeRemoved();

    void setPackageGroups(PackageGroupList *list);
    QQmlListProperty<PackageGroup> packageGroups();

    void setRepositories(RepositoryList *list);
    QQmlListProperty<Repository> repositories();

signals:
    void packageManagerChanged();

    void installedPackagesModelChanged();
    void updateAvailablePackagesModelChanged();
    void availablePackagesModelChanged();

    void selectedGroupChanged();
    void packageMarkingsChanged();

    void refreshCacheTransactionChanged();
    void refreshReposTransactionChanged();
    void getPackagesTransactionChanged();
    void getUpdatesTransactionChanged();
    void searchGroupsTransactionChanged();

    void updatePackagesTransactionChanged();
    void uninstallPackagesTransactionChanged();
    void installPackagesTransactionChanged();

    void packagesToBeInstalledChanged();
    void packagesToBeUpdatedChanged();
    void packagesToBeRemovedChanged();

    void packageGroupsChanged();

    void repositoriesChanged();

public slots:

private:
    QObject *m_packageManager;

    QAbstractItemModel *m_installedPackagesModel;
    QAbstractItemModel *m_updateAvailablePackagesModel;
    QAbstractItemModel *m_availablePackagesModel;

    uint m_selectedGroup;
    QObject *m_packageMarkings;

    TransactionWrapper *m_refreshCacheTransaction;
    TransactionWrapper *m_refreshReposTransaction;
    TransactionWrapper *m_getPackagesTransaction;
    TransactionWrapper *m_getUpdatesTransaction;
    TransactionWrapper *m_searchGroupsTransaction;

    TransactionWrapper *m_updatePackagesTransaction;
    TransactionWrapper *m_uninstallPackagesTransaction;
    TransactionWrapper *m_installPackagesTransaction;

    PackageList *m_packagesToBeInstalled;
    PackageList *m_packagesToBeUpdated;
    PackageList *m_packagesToBeRemoved;

    PackageGroupList *m_packageGroups;

    RepositoryList *m_repositories;
};

#endif // PACKMANCONTEXT_H
