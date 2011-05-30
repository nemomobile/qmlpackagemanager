#ifndef PACKMANCONTEXT_H
#define PACKMANCONTEXT_H

#include "transactionwrapper.h"

#include "package.h"
#include "packagegroup.h"

#include <QPackageKit>
#include <QObject>
#include <QDeclarativeListProperty>

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

    void setRefreshCacheTransaction(PackageKit::Transaction *transaction);
    QObject *refreshCacheTransaction();

    void setGetPackagesTransaction(PackageKit::Transaction *transaction);
    QObject *getPackagesTransaction();

    void setGetUpdatesTransaction(PackageKit::Transaction *transaction);
    QObject *getUpdatesTransaction();

    void setSearchGroupsTransaction(PackageKit::Transaction *transaction);
    QObject *searchGroupsTransaction();

    void setUpdatePackagesTransaction(PackageKit::Transaction *transaction);
    QObject *updatePackagesTransaction();

    void setUninstallPackagesTransaction(PackageKit::Transaction *transaction);
    QObject *uninstallPackagesTransaction();

    void setInstallPackagesTransaction(PackageKit::Transaction *transaction);
    QObject *installPackagesTransaction();

    void setPackagesToBeInstalled(PackageList *list);
    QDeclarativeListProperty<Package> packagesToBeInstalled();

    void setPackagesToBeUpdated(PackageList *list);
    QDeclarativeListProperty<Package> packagesToBeUpdated();

    void setPackagesToBeRemoved(PackageList *list);
    QDeclarativeListProperty<Package> packagesToBeRemoved();

    void setPackageGroups(PackageGroupList *list);
    QDeclarativeListProperty<PackageGroup> packageGroups();

signals:
    void packageManagerChanged();

    void installedPackagesModelChanged();
    void updateAvailablePackagesModelChanged();
    void availablePackagesModelChanged();

    void selectedGroupChanged();
    void packageMarkingsChanged();

    void refreshCacheTransactionChanged();
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

public slots:

private:
    QObject *m_packageManager;

    QAbstractItemModel *m_installedPackagesModel;
    QAbstractItemModel *m_updateAvailablePackagesModel;
    QAbstractItemModel *m_availablePackagesModel;

    uint m_selectedGroup;
    QObject *m_packageMarkings;

    TransactionWrapper *m_refreshCacheTransaction;
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
};

#endif // PACKMANCONTEXT_H
