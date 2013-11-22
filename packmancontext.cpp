/*
 * This file is part of mg-package-manager
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * Copyright (C) 2013 Timo Hannukkala <timo.hannukkala@nomovok.com>
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
#include <QDebug>
#include "packmancontext.h"
#include "packagemodel.h"
#include "packagelist.h"
#include "filterpackagemodel.h"

//#define PACKAGECONTEXT_LOG


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
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
}

void PackManContext::setPackageManager(QObject *manager)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    m_packageManager = manager;
    emit packageManagerChanged();
}

QObject *PackManContext::packageManager()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    return m_packageManager;
}

void PackManContext::setInstalledPackagesModel(FilterPackageModel *model)
{
#ifdef PACKAGECONTEXT_LOG
qDebug() << "void PackManContext::setInstalledPackagesModel(QAbstractItemModel *model)";
#endif
    if (model != m_installedPackagesModel) {
        m_installedPackagesModel = model;
        emit installedPackagesModelChanged();
    }
}

FilterPackageModel *PackManContext::installedPackagesModel()
{
#ifdef PACKAGECONTEXT_LOG
	qDebug() << Q_FUNC_INFO;
#endif
    return m_installedPackagesModel;
}

void PackManContext::setUpdateAvailablePackagesModel(FilterPackageModel *model)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    if (model != m_updateAvailablePackagesModel) {
        m_updateAvailablePackagesModel = model;
        emit updateAvailablePackagesModelChanged();
    }
}

FilterPackageModel *PackManContext::updateAvailablePackagesModel()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
	
    return m_updateAvailablePackagesModel;
}

FilterPackageModel *PackManContext::availablePackagesModel()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    return m_availablePackagesModel;
}

void PackManContext::setAvailablePackagesModel(FilterPackageModel *model)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    if (model != m_availablePackagesModel) {
        m_availablePackagesModel = model;
        emit availablePackagesModelChanged();
    }
}

void PackManContext::setSelectedGroup(uint group)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    if (group != m_selectedGroup) {
        m_selectedGroup = group;
        emit selectedGroupChanged();
    }
}

uint PackManContext::selectedGroup()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    return m_selectedGroup;
}

void PackManContext::setPackageMarkings(QObject *markings)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    m_packageMarkings = markings;
    emit packageMarkingsChanged();
}

QObject *PackManContext::packageMarkings()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;
#endif    	
    return m_packageMarkings;
}

void PackManContext::setRefreshCacheTransaction(TransactionWrapper *transaction)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    if (m_refreshCacheTransaction)
        m_refreshCacheTransaction->deleteLater();

    m_refreshCacheTransaction = transaction;
    emit refreshCacheTransactionChanged();
}


QObject *PackManContext::refreshCacheTransaction()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    return m_refreshCacheTransaction;
}

void PackManContext::setRefreshReposTransaction(TransactionWrapper *transaction)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    if (m_refreshReposTransaction)
        m_refreshReposTransaction->deleteLater();

    m_refreshReposTransaction = transaction;
    emit refreshReposTransactionChanged();
}

QObject *PackManContext::refreshReposTransaction()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    return m_refreshReposTransaction;
}

void PackManContext::setGetPackagesTransaction(TransactionWrapper *transaction)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    if (m_getPackagesTransaction)
        m_getPackagesTransaction->deleteLater();

    m_getPackagesTransaction = transaction;
    emit getPackagesTransactionChanged();
}


QObject *PackManContext::getPackagesTransaction()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    return m_getPackagesTransaction;
}

void PackManContext::setGetUpdatesTransaction(TransactionWrapper *transaction)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    if (m_getUpdatesTransaction)
        m_getUpdatesTransaction->deleteLater();

    m_getUpdatesTransaction = transaction;
    emit getUpdatesTransactionChanged();
}

QObject *PackManContext::getUpdatesTransaction()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    return m_getUpdatesTransaction;
}

QObject *PackManContext::searchGroupsTransaction()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    return m_searchGroupsTransaction;
}

void PackManContext::setSearchGroupsTransaction(TransactionWrapper *transaction)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    if (m_searchGroupsTransaction)
        m_searchGroupsTransaction->deleteLater();

    m_searchGroupsTransaction = transaction;
    emit searchGroupsTransactionChanged();
}

void PackManContext::setUpdatePackagesTransaction(TransactionWrapper *transaction)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    if (m_updatePackagesTransaction)
        m_updatePackagesTransaction->deleteLater();

    m_updatePackagesTransaction = transaction;
    emit updatePackagesTransactionChanged();
}

QObject *PackManContext::updatePackagesTransaction()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    return m_updatePackagesTransaction;
}

void PackManContext::setUninstallPackagesTransaction(TransactionWrapper *transaction)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    if (m_uninstallPackagesTransaction)
        m_uninstallPackagesTransaction->deleteLater();

    m_uninstallPackagesTransaction = transaction;
    emit uninstallPackagesTransactionChanged();
}

QObject *PackManContext::uninstallPackagesTransaction()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    return m_uninstallPackagesTransaction;
}

void PackManContext::setInstallPackagesTransaction(TransactionWrapper *transaction)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    if (m_installPackagesTransaction)
        m_installPackagesTransaction->deleteLater();

    m_installPackagesTransaction = transaction;
    emit installPackagesTransactionChanged();
}

QObject *PackManContext::installPackagesTransaction()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    return m_installPackagesTransaction;
}


void PackManContext::setPackagesToBeInstalled(PackageList *list)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    m_packagesToBeInstalled = list;
    connect(m_packagesToBeInstalled, SIGNAL(changed()), this, SIGNAL(packagesToBeInstalledChanged()));
    emit packagesToBeInstalledChanged();
}

QQmlListProperty<Package> PackManContext::packagesToBeInstalled()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    int i;
    QList<Package*> *list = m_packagesToBeInstalled->list();
    QList<Package*>list2; // = m_packagesToBeInstalled->list();

    for (i=0;i<list->size();i++) {
        list2.append(list->at(i));
    }

    return QQmlListProperty<Package>(this, list2);
}

void PackManContext::setPackagesToBeUpdated(PackageList *list)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    m_packagesToBeUpdated = list;
    connect(m_packagesToBeUpdated, SIGNAL(changed()), this, SIGNAL(packagesToBeUpdatedChanged()));
    emit packagesToBeUpdatedChanged();
}

QQmlListProperty<Package> PackManContext::packagesToBeUpdated()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    int i;
    QList<Package*> *list = m_packagesToBeUpdated->list();
    QList<Package*>list2;

    for (i=0;i<list->size();i++) {
        list2.append(list->at(i));
    }	

    return QQmlListProperty<Package>(this, list2);
}

void PackManContext::setPackagesToBeRemoved(PackageList *list)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    m_packagesToBeRemoved = list;
    connect(m_packagesToBeRemoved, SIGNAL(changed()), this, SIGNAL(packagesToBeRemovedChanged()));
    emit packagesToBeRemovedChanged();
}

QQmlListProperty<Package> PackManContext::packagesToBeRemoved()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    int i;
    QList<Package*> *list = m_packagesToBeRemoved->list();
    QList<Package*>list2;

    for (i=0;i<list->size();i++) {
        list2.append(list->at(i));
    }		
	
    return QQmlListProperty<Package>(this, list2);
}

void PackManContext::setPackageGroups(PackageGroupList *list)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    m_packageGroups = list;
    emit packageGroupsChanged();
}

QQmlListProperty<PackageGroup> PackManContext::packageGroups()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    return QQmlListProperty<PackageGroup>(this, *m_packageGroups->list());
}

void PackManContext::setRepositories(RepositoryList *list)
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    m_repositories = list;
    emit repositoriesChanged();
}

QQmlListProperty<Repository> PackManContext::repositories()
{
#ifdef PACKAGECONTEXT_LOG
    qDebug() << Q_FUNC_INFO;	
#endif
    return QQmlListProperty<Repository>(this, *m_repositories->list());
}
