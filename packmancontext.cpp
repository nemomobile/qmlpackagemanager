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

void PackManContext::setRefreshCacheTransaction(PackageKit::Transaction *transaction)
{
    if (m_refreshCacheTransaction)
        m_refreshCacheTransaction->deleteLater();

    m_refreshCacheTransaction = new TransactionWrapper(transaction, true, this);
    emit refreshCacheTransactionChanged();
}


QObject *PackManContext::refreshCacheTransaction()
{
    return m_refreshCacheTransaction;
}

void PackManContext::setRefreshReposTransaction(PackageKit::Transaction *transaction)
{
    if (m_refreshReposTransaction)
        m_refreshReposTransaction->deleteLater();

    m_refreshReposTransaction = new TransactionWrapper(transaction, true, this);
    emit refreshReposTransactionChanged();
}

QObject *PackManContext::refreshReposTransaction()
{
    return m_refreshReposTransaction;
}

void PackManContext::setGetPackagesTransaction(PackageKit::Transaction *transaction)
{
    if (m_getPackagesTransaction)
        m_getPackagesTransaction->deleteLater();

    m_getPackagesTransaction = new TransactionWrapper(transaction, false, this);
    emit getPackagesTransactionChanged();
}


QObject *PackManContext::getPackagesTransaction()
{
    return m_getPackagesTransaction;
}

void PackManContext::setGetUpdatesTransaction(PackageKit::Transaction *transaction)
{
    if (m_getUpdatesTransaction)
        m_getUpdatesTransaction->deleteLater();

    m_getUpdatesTransaction = new TransactionWrapper(transaction, false, this);
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

void PackManContext::setSearchGroupsTransaction(PackageKit::Transaction *transaction)
{
    if (m_searchGroupsTransaction)
        m_searchGroupsTransaction->deleteLater();

    m_searchGroupsTransaction = new TransactionWrapper(transaction, false, this);
    emit searchGroupsTransactionChanged();
}

void PackManContext::setUpdatePackagesTransaction(PackageKit::Transaction *transaction)
{
    if (m_updatePackagesTransaction)
        m_updatePackagesTransaction->deleteLater();

    m_updatePackagesTransaction = new TransactionWrapper(transaction, true, this);
    emit updatePackagesTransactionChanged();
}

QObject *PackManContext::updatePackagesTransaction()
{
    return m_updatePackagesTransaction;
}

void PackManContext::setUninstallPackagesTransaction(PackageKit::Transaction *transaction)
{
    if (m_uninstallPackagesTransaction)
        m_uninstallPackagesTransaction->deleteLater();

    m_uninstallPackagesTransaction = new TransactionWrapper(transaction, true, this);
    emit uninstallPackagesTransactionChanged();
}

QObject *PackManContext::uninstallPackagesTransaction()
{
    return m_uninstallPackagesTransaction;
}

void PackManContext::setInstallPackagesTransaction(PackageKit::Transaction *transaction)
{
    if (m_installPackagesTransaction)
        m_installPackagesTransaction->deleteLater();

    m_installPackagesTransaction = new TransactionWrapper(transaction, true, this);
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
    return QDeclarativeListProperty<Package>(this, *m_packagesToBeInstalled->list());
}

void PackManContext::setPackagesToBeUpdated(PackageList *list)
{
    m_packagesToBeUpdated = list;
    connect(m_packagesToBeUpdated, SIGNAL(changed()), this, SIGNAL(packagesToBeUpdatedChanged()));
    emit packagesToBeUpdatedChanged();
}

QDeclarativeListProperty<Package> PackManContext::packagesToBeUpdated()
{
    return QDeclarativeListProperty<Package>(this, *m_packagesToBeUpdated->list());
}

void PackManContext::setPackagesToBeRemoved(PackageList *list)
{
    m_packagesToBeRemoved = list;
    connect(m_packagesToBeRemoved, SIGNAL(changed()), this, SIGNAL(packagesToBeRemovedChanged()));
    emit packagesToBeRemovedChanged();
}

QDeclarativeListProperty<Package> PackManContext::packagesToBeRemoved()
{
    return QDeclarativeListProperty<Package>(this, *m_packagesToBeRemoved->list());
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
