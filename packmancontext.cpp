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

#include "packmancontext.h"

#include "packagelist.h"

PackManContext::PackManContext(QObject *parent) :
    QObject(parent),
    m_packageManager(0),
    m_installedPackagesModel(0),
    m_updateAvailablePackagesModel(0),
    m_availablePackagesModel(0),
    m_packageMarkings(0),
    m_refreshCacheTransaction(0),
    m_refreshReposTransaction(0),
    m_getPackagesTransaction(0),
    m_getUpdatesTransaction(0),
    m_searchGroupsTransaction(0),
    m_updatePackagesTransaction(0),
    m_uninstallPackagesTransaction(0),
    m_installPackagesTransaction(0)
{
}

void PackManContext::setPackageManager(QObject *manager)
{
    m_packageManager = manager;
    emit packageManagerChanged();
}

QObject *PackManContext::packageManager()
{
    return m_packageManager;
}

void PackManContext::setInstalledPackagesModel(QAbstractItemModel *model)
{
    if (model != m_installedPackagesModel) {
        m_installedPackagesModel = model;
        emit installedPackagesModelChanged();
    }
}

QAbstractItemModel *PackManContext::installedPackagesModel()
{
    return m_installedPackagesModel;
}

void PackManContext::setUpdateAvailablePackagesModel(QAbstractItemModel *model)
{
    if (model != m_updateAvailablePackagesModel) {
        m_updateAvailablePackagesModel = model;
        emit updateAvailablePackagesModelChanged();
    }
}

QAbstractItemModel *PackManContext::updateAvailablePackagesModel()
{
    return m_updateAvailablePackagesModel;
}

QAbstractItemModel *PackManContext::availablePackagesModel()
{
    return m_availablePackagesModel;
}

void PackManContext::setAvailablePackagesModel(QAbstractItemModel *model)
{
    if (model != m_availablePackagesModel) {
        m_availablePackagesModel = model;
        emit availablePackagesModelChanged();
    }
}

void PackManContext::setSelectedGroup(uint group)
{
    if (group != m_selectedGroup) {
        m_selectedGroup = group;
        emit selectedGroupChanged();
    }
}

uint PackManContext::selectedGroup()
{
    return m_selectedGroup;
}

void PackManContext::setPackageMarkings(QObject *markings)
{
    m_packageMarkings = markings;
    emit packageMarkingsChanged();
}

QObject *PackManContext::packageMarkings()
{
    return m_packageMarkings;
}

void PackManContext::setRefreshCacheTransaction(TransactionWrapper *transaction)
{
    if (m_refreshCacheTransaction)
        m_refreshCacheTransaction->deleteLater();

    m_refreshCacheTransaction = transaction;
    emit refreshCacheTransactionChanged();
}


QObject *PackManContext::refreshCacheTransaction()
{
    return m_refreshCacheTransaction;
}

void PackManContext::setRefreshReposTransaction(TransactionWrapper *transaction)
{
    if (m_refreshReposTransaction)
        m_refreshReposTransaction->deleteLater();

    m_refreshReposTransaction = transaction;
    emit refreshReposTransactionChanged();
}

QObject *PackManContext::refreshReposTransaction()
{
    return m_refreshReposTransaction;
}

void PackManContext::setGetPackagesTransaction(TransactionWrapper *transaction)
{
    if (m_getPackagesTransaction)
        m_getPackagesTransaction->deleteLater();

    m_getPackagesTransaction = transaction;
    emit getPackagesTransactionChanged();
}


QObject *PackManContext::getPackagesTransaction()
{
    return m_getPackagesTransaction;
}

void PackManContext::setGetUpdatesTransaction(TransactionWrapper *transaction)
{
    if (m_getUpdatesTransaction)
        m_getUpdatesTransaction->deleteLater();

    m_getUpdatesTransaction = transaction;
    emit getUpdatesTransactionChanged();
}

QObject *PackManContext::getUpdatesTransaction()
{
    return m_getUpdatesTransaction;
}

QObject *PackManContext::searchGroupsTransaction()
{
    return m_searchGroupsTransaction;
}

void PackManContext::setSearchGroupsTransaction(TransactionWrapper *transaction)
{
    if (m_searchGroupsTransaction)
        m_searchGroupsTransaction->deleteLater();

    m_searchGroupsTransaction = transaction;
    emit searchGroupsTransactionChanged();
}

void PackManContext::setUpdatePackagesTransaction(TransactionWrapper *transaction)
{
    if (m_updatePackagesTransaction)
        m_updatePackagesTransaction->deleteLater();

    m_updatePackagesTransaction = transaction;
    emit updatePackagesTransactionChanged();
}

QObject *PackManContext::updatePackagesTransaction()
{
    return m_updatePackagesTransaction;
}

void PackManContext::setUninstallPackagesTransaction(TransactionWrapper *transaction)
{
    if (m_uninstallPackagesTransaction)
        m_uninstallPackagesTransaction->deleteLater();

    m_uninstallPackagesTransaction = transaction;
    emit uninstallPackagesTransactionChanged();
}

QObject *PackManContext::uninstallPackagesTransaction()
{
    return m_uninstallPackagesTransaction;
}

void PackManContext::setInstallPackagesTransaction(TransactionWrapper *transaction)
{
    if (m_installPackagesTransaction)
        m_installPackagesTransaction->deleteLater();

    m_installPackagesTransaction = transaction;
    emit installPackagesTransactionChanged();
}

QObject *PackManContext::installPackagesTransaction()
{
    return m_installPackagesTransaction;
}


void PackManContext::setPackagesToBeInstalled(PackageList *list)
{
    m_packagesToBeInstalled = list;
    connect(m_packagesToBeInstalled, SIGNAL(changed()), this, SIGNAL(packagesToBeInstalledChanged()));
    emit packagesToBeInstalledChanged();
}

QDeclarativeListProperty<Package> PackManContext::packagesToBeInstalled()
{
    return QDeclarativeListProperty<Package>(this, *m_packagesToBeInstallem_list());
}

void PackManContext::setPackagesToBeUpdated(PackageList *list)
{
    m_packagesToBeUpdated = list;
    connect(m_packagesToBeUpdated, SIGNAL(changed()), this, SIGNAL(packagesToBeUpdatedChanged()));
    emit packagesToBeUpdatedChanged();
}

QDeclarativeListProperty<Package> PackManContext::packagesToBeUpdated()
{
    return QDeclarativeListProperty<Package>(this, *m_packagesToBeUpdatem_list());
}

void PackManContext::setPackagesToBeRemoved(PackageList *list)
{
    m_packagesToBeRemoved = list;
    connect(m_packagesToBeRemoved, SIGNAL(changed()), this, SIGNAL(packagesToBeRemovedChanged()));
    emit packagesToBeRemovedChanged();
}

QDeclarativeListProperty<Package> PackManContext::packagesToBeRemoved()
{
    return QDeclarativeListProperty<Package>(this, *m_packagesToBeRemovem_list());
}

void PackManContext::setPackageGroups(PackageGroupList *list)
{
    m_packageGroups = list;
    emit packageGroupsChanged();
}

QDeclarativeListProperty<PackageGroup> PackManContext::packageGroups()
{
    return QDeclarativeListProperty<PackageGroup>(this, *m_packageGroups->list());
}

void PackManContext::setRepositories(RepositoryList *list)
{
    m_repositories = list;
    emit repositoriesChanged();
}

QDeclarativeListProperty<Repository> PackManContext::repositories()
{
    return QDeclarativeListProperty<Repository>(this, *m_repositories->list());
}
