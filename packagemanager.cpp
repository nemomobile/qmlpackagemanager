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

#include "packagemanager.h"

#include "filterpackagemodel.h"
#include "repository.h"

#include "networktest.h"

#include <QQmlContext>
#include <QGraphicsObject>
#include <QSortFilterProxyModel>


#include <QSet>


QString groupNames[] =
{
    "unknown",
    "accessibility",
    "accessories",
    "admin-tools",
    "communication",
    "desktop-gnome",
    "desktop-kde",
    "desktop-other",
    "desktop-xfce",
    "education",
    "fonts",
    "games",
    "graphics",
    "internet",
    "legacy",
    "localization",
    "maps",
    "multimedia",
    "network",
    "office",
    "other",
    "power-management",
    "programming",
    "publishing",
    "repos",
    "security",
    "servers",
    "system",
    "virtualization",
    "science",
    "documentaton",
    "electronics",
    "collections",
    "vendor",
    "newest"
};



PackageManager *PackageManager::m_packageManager = 0;

PackageManager::PackageManager(QQuickView *viewer, QObject *parent) :
    QObject(parent),
    m_refreshCacheTransaction(0),
    m_refreshReposTransaction(0),
    m_getPackagesTransaction(0),
    m_getUpdatesTransaction(0),
    m_searchGroupsTransaction(0)
{
    m_packageManager = this;
    m_viewer = viewer;
    m_qmlContext = m_viewer->rootContext();

    m_viewer->setWindowTitle("Package Manager");
    m_viewer->showFullScreen();

    m_installedPackagesModel = new PackageModel(this);
    m_installedPackagesFilterModel = new FilterPackageModel(this);
    m_installedPackagesFilterModel->setFilterRole(PackageModel::FilterNameRole);
    m_installedPackagesFilterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_installedPackagesFilterModel->setSourceModel(m_installedPackagesModel);

    m_updateAvailablePackagesModel = new PackageModel(this);
    m_updateAvailablePackagesFilterModel = new FilterPackageModel(this);
    m_updateAvailablePackagesFilterModel->setFilterRole(PackageModel::FilterNameRole);
    m_updateAvailablePackagesFilterModel->setSourceModel(m_updateAvailablePackagesModel);

    m_availablePackagesModel = new PackageModel(this);
    m_availablePackagesFilterModel = new FilterPackageModel(this);
    m_availablePackagesFilterModel->setFilterRole(PackageModel::FilterNameRole);
    m_availablePackagesFilterModel->setSourceModel(m_availablePackagesModel);

    m_packManContext.setPackageManager(this);
    m_packManContext.setInstalledPackagesModel(m_installedPackagesFilterModel);
    m_packManContext.setUpdateAvailablePackagesModel(m_updateAvailablePackagesFilterModel);
    m_packManContext.setAvailablePackagesModel(m_availablePackagesFilterModel);

    m_packManContext.setPackageMarkings(m_installedPackagesModel->packageMarkings());
    m_qmlContext->setContextObject(&m_packManContext);

    m_packManContext.setPackagesToBeInstalled(&m_packagesToBeInstalled);
    m_packManContext.setPackagesToBeUpdated(&m_packagesToBeUpdated);
    m_packManContext.setPackagesToBeRemoved(&m_packagesToBeRemoved);

    m_packManContext.setPackageGroups(&m_packageGroups);
    m_packManContext.setSelectedGroup(PackageKit::Enum::UnknownGroup);

    m_packManContext.setRepositories(&m_repositories);
}

PackageManager *PackageManager::instance()
{
    return m_packageManager;
}

bool PackageManager::testNetworkConnection(TransactionWrapper *tw)
{
    NetworkTest test;
    bool nwConnection = test.hasNetworkConnection();
    if (!nwConnection) {
        tw->setErrorText("Cannot connect to network");
        tw->setState("error");
    }

    return nwConnection;
}

void PackageManager::refreshCache()
{
//    qDebug() << Q_FUNC_INFO;

    if (m_refreshCacheTransaction)
        return;

    TransactionWrapper *tw = new TransactionWrapper(0, true, this);
    m_packManContext.setRefreshCacheTransaction(tw);
    if (!testNetworkConnection(tw))
        return;

    m_refreshCacheTransaction = new PackageKit::Transaction(0, this);
    m_packManContext.setRefreshCacheTransaction(new TransactionWrapper(m_refreshCacheTransaction, true, this));

    connect(m_refreshCacheTransaction, SIGNAL(changed()), this, SLOT(onChanged()));
    connect(m_refreshCacheTransaction, SIGNAL(message(uint type, const QString &message)),
            this, SLOT(onMessage(message(uint type, const QString &message))));
    connect(m_refreshCacheTransaction, SIGNAL(repoSignatureRequired(const QString &pid, const QString &repoName, const QString &keyUrl, const QString &keyUserid, const QString &keyId, const QString &keyFingerprint, const QString &keyTimestamp, uint type)),
            this, SLOT(onRepoSignatureRequired(const QString &pid, const QString &repoName, const QString &keyUrl, const QString &keyUserid, const QString &keyId, const QString &keyFingerprint, const QString &keyTimestamp, uint type)));
    connect(m_refreshCacheTransaction, SIGNAL(errorCode(uint error, const QString &details)),
            this, SLOT(onErrorCode(uint error, const QString &details)));
    connect(m_refreshCacheTransaction, SIGNAL(finished(PackageKit::Transaction::Exit status, uint runtime)),
            this, SLOT(onRefreshCacheFinished(PackageKit::Transaction::Exit status, uint runtime)));

    m_refreshCacheTransaction->refreshCache(true);
    m_packManContext.setSelectedGroup(PackageKit::Enum::UnknownGroup);
}

void PackageManager::refreshAll(uint delay)
{
//    qDebug() << Q_FUNC_INFO << delay;
    if (delay == 0) {
        refreshUpdate();
        refreshInstalled();
    } else {
        QTimer::singleShot(delay, this, SLOT(refreshAll()));
    }
}

void PackageManager::refreshUpdate()
{
//    qDebug() << Q_FUNC_INFO;

    if (m_getUpdatesTransaction)
        return; // still updating

    m_updateAvailablePackagesModel->clear();

    m_getUpdatesTransaction = new PackageKit::Transaction(0, this);
    m_packManContext.setGetUpdatesTransaction(new TransactionWrapper(m_getUpdatesTransaction, false, this));

    /*connect(m_getUpdatesTransaction, SIGNAL(package(QSharedPointer<PackageKit::Package>)),
            this, SLOT(onUpdateAvailablePackage(QSharedPointer<PackageKit::Package>)));*/

    connect(m_getUpdatesTransaction, SIGNAL(package(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)),
                this, SLOT(onUpdateAvailablePackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)));

    connect(m_getUpdatesTransaction, SIGNAL(repoSignatureRequired(const QString &packageID,
                                                                  const QString &repoName,
                                                                  const QString &keyUrl,
                                                                  const QString &keyUserid,
                                                                  const QString &keyId,
                                                                  const QString &keyFingerprint,
                                                                  const QString &keyTimestamp,
                                                                  PackageKit::Transaction::SigType type)),
            this, SLOT(onRepoSignatureRequired(const QString &packageID,
                                               const QString &repoName,
                                               const QString &keyUrl,
                                               const QString &keyUserid,
                                               const QString &keyId,
                                               const QString &keyFingerprint,
                                               const QString &keyTimestamp,
                                               PackageKit::Transaction::SigType type)));
    connect(m_getUpdatesTransaction, SIGNAL(finished(PackageKit::Enum::Exit,uint)), this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));
    m_getUpdatesTransaction->getUpdates(); // PackageKit::Enum::FilterInstalled);
}

void PackageManager::refreshInstalled()
{
//    qDebug() << Q_FUNC_INFO;

    m_installedPackagesModel->clear();

    m_getPackagesTransaction = new PackageKit::Transaction(0, this);
    m_packManContext.setGetPackagesTransaction(new TransactionWrapper(m_getPackagesTransaction, false, this));

    connect(m_getPackagesTransaction, SIGNAL(package(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)),
        this, SLOT(onInstalledPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)));
    connect(m_getPackagesTransaction, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
        this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));
    m_getPackagesTransaction->getPackages(PackageKit::Enum::FilterInstalled);
}

void PackageManager::refreshAvailable(uint group)
{
//    qDebug() << Q_FUNC_INFO << group;
    if (group >= PackageKit::Enum::LastGroup || m_packManContext.selectedGroup() == group)
        return;

    m_packManContext.setSelectedGroup(group);
    m_availablePackagesModel->clear();

    PackageKit::Transaction *t = new PackageKit::Transaction(0, this);
    m_packManContext.setSearchGroupsTransaction(new TransactionWrapper(t, false, this));

    connect(t, SIGNAL(package(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)),
            this, SLOT(onAvailablePackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)));
    connect(t, SIGNAL(repoSignatureRequired(PackageKit::Client::SignatureInfo)),
            this, SLOT(onRepoSignatureRequired(PackageKit::Client::SignatureInfo)));
    connect(t, SIGNAL(finished(PackageKit::Transaction::Exit status, uint runtime)),
            this, SLOT(onFinished(PackageKit::Transaction::Exit status, uint runtime)));
    t->searchGroups(groupNames[group]);
}

void PackageManager::refreshRepos()
{
//    qDebug() << Q_FUNC_INFO;

    PackageKit::Transaction *t = new PackageKit::Transaction(0, this);

    m_repositories.clear();
    m_packManContext.setRepositories(&m_repositories);
    m_packManContext.setRefreshReposTransaction(new TransactionWrapper(t, true, this));

    connect(t, SIGNAL(repoDetail(const QString &repoId, const QString &description, bool enabled)),
            this, SLOT(onRepoDetail(const QString &repoId, const QString &description, bool enabled)));

    connect(t, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
            this, SLOT(onRefreshReposFinished(PackageKit::Transaction::Exit status, uint runtime)));

    m_refreshReposTransaction = t;

    t->getRepoList();
}

void PackageManager::onPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    PackageKit::Transaction *transaction = static_cast<PackageKit::Transaction*>(sender());
    PackageKit::Enum::Role role = transaction->role();

    if (role == PackageKit::Enum::RoleSimulateRemovePackages
        || role == PackageKit::Enum::RoleSimulateInstallPackages
        || role == PackageKit::Enum::RoleSimulateUpdatePackages) {
        PackageKit::Package *p = packagePtr.data();
        QString packageString = p->name() + " (" + p->version() + ")  " + p->summary();
        if (p->info() == PackageKit::Enum::InfoInstalling) {
            m_packagesToBeInstalled << packagePtr;
        } else if (p->info() == PackageKit::Enum::InfoUpdating) {
            m_packagesToBeUpdated << packagePtr;
        } else if (p->info() == PackageKit::Enum::InfoRemoving) {
            m_packagesToBeRemoved << packagePtr;
        } else if (p->info() != PackageKit::Enum::InfoFinished) {
            qDebug() << "*** info not handled: " << packageString << p->info();
        }
    } else {
//        qDebug() << (*packagePtr).name();
    }
}

void PackageManager::onInstalledPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
//    qDebug() << Q_FUNC_INFO << (*packagePtr).id()  << (*packagePtr).info();

    m_installedPackagesModel->addPackage(packagePtr, false);
}

void PackageManager::onUpdateAvailablePackage(QSharedPointer<OldPackageKit::Package> packagePtr)
{
//    qDebug() << Q_FUNC_INFO << (*packagePtr).id()  << (*packagePtr).info();

    m_updateAvailablePackagesModel->addPackage(packagePtr, true);
}

void PackageManager::onAvailablePackage(QSharedPointer<PackageKit::Package> packagePtr)
{
//    qDebug() << Q_FUNC_INFO << (*packagePtr).id()  << (*packagePtr).info();

    QString name = (*packagePtr).name();
    if (m_installedPackagesModel->findPackage(name) != 0)
        return;

    QString id = (*packagePtr).id();
    QStringList parts = id.split(';');
    if (parts.count() >= 4 && !parts.at(3).startsWith("@"))
        m_availablePackagesModel->addPackage(packagePtr, false);
}

void PackageManager::onFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    PackageKit::Transaction *t = qobject_cast<PackageKit::Transaction*>(sender());

//    qDebug() << Q_FUNC_INFO << status << runtime << t->role();

    if (sender() == m_getUpdatesTransaction) {
//        delete m_getUpdatesTransaction;
        m_getUpdatesTransaction = 0;
    } else if (sender() == m_getPackagesTransaction) {
//        delete m_getPackagesTransaction;
        m_getPackagesTransaction = 0;
    }

    if (t && t->role() == PackageKit::Enum::RoleRemovePackages) {
        m_packManContext.setSelectedGroup(PackageKit::Enum::UnknownGroup);
        refreshUpdate();
        refreshInstalled();
    } else if (t && t->role() == PackageKit::Enum::RoleUpdatePackages) {
        m_packManContext.setSelectedGroup(PackageKit::Enum::UnknownGroup);
        refreshUpdate();
        refreshInstalled();
    } else if (t && t->role() == PackageKit::Enum::RoleInstallPackages) {
        uint group = m_packManContext.selectedGroup();
        refreshCache(); // includes refreshUpdate();
        refreshInstalled();
        refreshAvailable(group);
    }
}

void PackageManager::onRefreshCacheFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    if (status == PackageKit::Enum::ExitSuccess) {
        refreshUpdate();
//        refreshInstalled();
    }

    m_refreshCacheTransaction = 0;
}

void PackageManager::onRefreshReposFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    m_refreshReposTransaction = 0;
    m_packManContext.setRepositories(&m_repositories);
}

void PackageManager::uninstallMarkedPackages(bool simulate, bool autoremove)
{
//    qDebug() << Q_FUNC_INFO << simulate << autoremove;

    QList<Package*> packageList = m_installedPackagesModel->packageMarkings()->packagesMarked();

    if (packageList.count() == 0)
        return;

    QList<QSharedPointer<PackageKit::Package> > packagePtrList;
    foreach (Package *package, packageList)
        packagePtrList << package->package();

    PackageKit::Transaction *t = new PackageKit::Transaction(0, this);
    m_packManContext.setUninstallPackagesTransaction(new TransactionWrapper(t, true, this));

    connect(t, SIGNAL(package(QSharedPointer<PackageKit::Package>)),
            this, SLOT(onPackage(QSharedPointer<PackageKit::Package>)));

    connect(t, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));

    if (simulate) {
        m_packagesToBeInstalled.clear();
        m_packagesToBeUpdated.clear();
        m_packagesToBeRemoved.clear();
        t->simulateRemovePackages(packagePtrList, autoremove);
    } else {
        t->removePackages(packagePtrList, false, autoremove);
    }
}


void PackageManager::updateMarkedPackages(bool simulate, bool onlyTrusted)
{
//    qDebug() << Q_FUNC_INFO << simulate << onlyTrusted;

    QList<Package*> packageList = m_updateAvailablePackagesModel->packageMarkings()->packagesMarked();

    if (packageList.count() == 0)
        return;

    QList<QSharedPointer<PackageKit::Package> > packagePtrList;
    foreach (Package *package, packageList)
        packagePtrList << package->package();

    TransactionWrapper *tw = new TransactionWrapper(0, true, this);
    m_packManContext.setUpdatePackagesTransaction(tw);
    if (!testNetworkConnection(tw))
        return;

    PackageKit::Transaction *t = new PackageKit::Transaction(0, this);
    m_packManContext.setUpdatePackagesTransaction(new TransactionWrapper(t, true, this));

    connect(t, SIGNAL(package(QSharedPointer<PackageKit::Package>)),
            this, SLOT(onPackage(QSharedPointer<PackageKit::Package>)));

    connect(t, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));

    if (simulate) {
        m_packagesToBeInstalled.clear();
        m_packagesToBeUpdated.clear();
        m_packagesToBeRemoved.clear();
        t->simulateUpdatePackages(packagePtrList);
    } else {
        t->updatePackages(onlyTrusted, packagePtrList);
    }
}


void PackageManager::installMarkedPackages(bool simulate, bool onlyTrusted)
{
//    qDebug() << Q_FUNC_INFO << simulate << onlyTrusted;

    QList<Package*> packageList = m_availablePackagesModel->packageMarkings()->packagesMarked();

    if (packageList.count() == 0)
        return;

    QList<QSharedPointer<PackageKit::Package> > packagePtrList;
    foreach (Package *package, packageList)
        packagePtrList << package->package();

    TransactionWrapper *tw = new TransactionWrapper(0, true, this);
    m_packManContext.setInstallPackagesTransaction(tw);
    if (!testNetworkConnection(tw))
        return;

    PackageKit::Transaction *t = new PackageKit::Transaction(0, this);
    m_packManContext.setInstallPackagesTransaction(new TransactionWrapper(t, true, this));

    connect(t, SIGNAL(package(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)),
            this, SLOT(onPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)));

    connect(t, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));

    if (simulate) {
        m_packagesToBeInstalled.clear();
        m_packagesToBeUpdated.clear();
        m_packagesToBeRemoved.clear();
        t->simulateInstallPackages(packagePtrList);
    } else {
        t->installPackages(onlyTrusted, packagePtrList);
    }
}

void PackageManager::onChanged()
{
    //qDebug() << Q_FUNC_INFO;
}

void PackageManager::onCategory(const QString &parent_id, const QString &cat_id, const QString &name, const QString &summary, const QString &icon)
{
//    qDebug() << Q_FUNC_INFO << parent_id << cat_id << name << summary << icon;
}

void PackageManager::onErrorCode(uint error, const QString &details)
{
//    qDebug() << Q_FUNC_INFO << error << details;
}
/*
void PackageManager::onEulaRequired(const PackageKit::Client::EulaInfo &info)
{
    qDebug() << Q_FUNC_INFO << info.id << info.licenseAgreement << info.vendorName;
}
*/
/*
void PackageManager::onMediaChangeRequired(PackageKit::Enum::MediaType type, const QString& id, const QString& text)
{
    qDebug() << Q_FUNC_INFO << type << id << text;
}
*/
/*
void PackageManager::onFiles(const QSharedPointer<PackageKit::Package> &packagePtr, const QStringList &filenames)
{
    PackageKit::Package *p = packagePtr.data();
    qDebug() << Q_FUNC_INFO << p->name() << filenames;
}
*/
void PackageManager::onMessage(uint type, const QString &message)
{
    qDebug() << Q_FUNC_INFO << type << message;
}

void PackageManager::onRepoDetail(const QString &repoId, const QString &description, bool enabled)
{
//    qDebug() << Q_FUNC_INFO << repoId << description << enabled;
    m_repositories << new Repository(repoId, description, enabled, this);
}

void PackageManager::onRepoSignatureRequired(const QString &pid, const QString &repoName, const QString &keyUrl, const QString &keyUserid, const QString &keyId, const QString &keyFingerprint, const QString &keyTimestamp, uint type)
{
    PackageKit::Package *p = info.package.data();
//    qDebug() << Q_FUNC_INFO << p->name() << info.repoId;

    PackageKit::Transaction *t = new PackageKit::Transaction(QString(), this);
    connect(t, SIGNAL(errorCode(PackageKit::Enum::Error,QString)),
            this, SLOT(onErrorCode(PackageKit::Enum::Error,QString)));
    connect(t, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));

    t->installSignature(info.type, info.keyId, info.package);

    PackageKit::Transaction *origTransaction = qobject_cast<PackageKit::Transaction*>(sender());
    if (origTransaction) {
        if (origTransaction->role() == PackageKit::Enum::RoleRefreshCache) {
            m_refreshCacheTransaction = 0;
            refreshCache();
        } else if (origTransaction->role() == PackageKit::Enum::RoleSearchGroup) {
            uint group = m_packManContext.selectedGroup();
            refreshAvailable(group);
        } else if (origTransaction->role() == PackageKit::Enum::RoleGetUpdates) {
            m_getUpdatesTransaction = 0;
            refreshUpdate();
        }
    }

}
/*
void PackageManager::onRequireRestart(PackageKit::Enum::Restart type, const QSharedPointer<PackageKit::Package> &packagePtr)
{
    PackageKit::Package *p = packagePtr.data();
    qDebug() << Q_FUNC_INFO << type << p->name();
}
*/
