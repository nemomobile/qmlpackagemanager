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

#include "packagemanager.h"

#include "filterpackagemodel.h"
#include "repository.h"

#include "networktest.h"

#include <QQmlContext>
#include <QGraphicsObject>
#include <QSortFilterProxyModel>
#include <QDebug>

#include <QSet>

#include <Transaction>

//#define PACKAGEMANAGER_LOG	

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
    m_searchGroupsTransaction(0),
    m_bSimulation(false)
{
    m_packageManager = this;
    m_viewer = viewer;
    m_qmlContext = m_viewer->rootContext();

    //m_viewer->setWindowTitle("Package Manager");
   // m_viewer->showFullScreen();

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
    m_packManContext.setSelectedGroup(PackageKit::Transaction::GroupUnknown);

    m_packManContext.setRepositories(&m_repositories);
}

PackageManager *PackageManager::instance()
{
#ifdef PACKAGEMANAGER_LOG	
   qDebug() << Q_FUNC_INFO;	
#endif
    return m_packageManager;
}

bool PackageManager::testNetworkConnection(TransactionWrapper *tw)
{
#ifdef PACKAGEMANAGER_LOG	
   qDebug() << Q_FUNC_INFO;	
#endif
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
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif

    if (m_refreshCacheTransaction)
        return;

    TransactionWrapper *tw = new TransactionWrapper(0, true, this);
    m_packManContext.setRefreshCacheTransaction(tw);
    if (!testNetworkConnection(tw))
        return;

    m_refreshCacheTransaction = new PackageKit::Transaction(this);
    m_packManContext.setRefreshCacheTransaction(new TransactionWrapper(m_refreshCacheTransaction, true, this));

    connect(m_refreshCacheTransaction, SIGNAL(changed()), this, SLOT(onChanged()));
    connect(m_refreshCacheTransaction, SIGNAL(message(PackageKit::Transaction::Message, const QString &)),
            this, SLOT(onMessage(PackageKit::Transaction::Message, const QString &)));

    connect(m_refreshCacheTransaction, SIGNAL(repoSignatureRequired(const QString &,
                                                            const QString &,
                                                            const QString &,
                                                            const QString &,
                                                            const QString &,
                                                            const QString &,
                                                            const QString &,
                                                            PackageKit::Transaction::SigType)),
                this, SLOT(onRepoSignatureRequired(const QString &,
                                                   const QString &,
                                                   const QString &,
                                                   const QString &,
                                                   const QString &,
                                                   const QString &,
                                                   const QString &,
                                                   PackageKit::Transaction::SigType)));

    connect(m_refreshCacheTransaction, SIGNAL(errorCode(PackageKit::Transaction::Error, const QString &)),
            this, SLOT(onErrorCode(PackageKit::Transaction::Error, const QString &)));
    connect(m_refreshCacheTransaction, SIGNAL(finished(PackageKit::Transaction::Exit, uint)),
            this, SLOT(onRefreshCacheFinished(PackageKit::Transaction::Exit, uint)));

    m_refreshCacheTransaction->refreshCache(true);
    m_packManContext.setSelectedGroup(PackageKit::Transaction::GroupUnknown);

}

void PackageManager::refreshAll(uint delay)
{
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO << delay;
#endif
    if (delay == 0) {
        refreshUpdate();
        refreshInstalled();
    } else {
        QTimer::singleShot(delay, this, SLOT(refreshAll()));
    }
}

void PackageManager::refreshUpdate()
{
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif

    if (m_getUpdatesTransaction)
        return; // still updating

    m_updateAvailablePackagesModel->clear();

    m_getUpdatesTransaction = new PackageKit::Transaction(this);
    m_packManContext.setGetUpdatesTransaction(new TransactionWrapper(m_getUpdatesTransaction, false, this));

    connect(m_getUpdatesTransaction, SIGNAL(package(PackageKit::Transaction::Info, const QString &, const QString &)),
                this, SLOT(onUpdateAvailablePackage(PackageKit::Transaction::Info, const QString &, const QString &)));

    connect(m_getUpdatesTransaction, SIGNAL(repoSignatureRequired(const QString &,
                                                                  const QString &,
                                                                  const QString &,
                                                                  const QString &,
                                                                  const QString &,
                                                                  const QString &,
                                                                  const QString &,
                                                                  PackageKit::Transaction::SigType)),
            this, SLOT(onRepoSignatureRequired(const QString &,
                                               const QString &,
                                               const QString &,
                                               const QString &,
                                               const QString &,
                                               const QString &,
                                               const QString &,
                                               PackageKit::Transaction::SigType)));
    connect(m_getUpdatesTransaction, SIGNAL(finished(PackageKit::Transaction::Exit, uint)),
    	this, SLOT(onFinished(PackageKit::Transaction::Exit, uint)));
    m_getUpdatesTransaction->getUpdates(); // PackageKit::Enum::FilterInstalled);
}

void PackageManager::refreshInstalled()
{
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif

    m_installedPackagesModel->clear();

    m_getPackagesTransaction = new PackageKit::Transaction(this);
    m_packManContext.setGetPackagesTransaction(new TransactionWrapper(m_getPackagesTransaction, false, this));

    connect(m_getPackagesTransaction,
        SIGNAL(package(PackageKit::Transaction::Info, const QString &, const QString &)),
        this,
        SLOT(onInstalledPackage(PackageKit::Transaction::Info, const QString &, const QString &)));
    connect(m_getPackagesTransaction, SIGNAL(finished(PackageKit::Transaction::Exit,uint)),
        this, SLOT(onFinished(PackageKit::Transaction::Exit,uint)));
    m_getPackagesTransaction->getPackages(PackageKit::Transaction::FilterInstalled);
}

void PackageManager::refreshAvailable(uint group)
{
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO << group;
#endif
  //WHAT TO DO WITH THIS?
  //if (group >= PackageKit::Enum::LastGroup || m_packManContext.selectedGroup() == group)
  //      return;

    m_packManContext.setSelectedGroup(group);
    m_availablePackagesModel->clear();

    PackageKit::Transaction *t = new PackageKit::Transaction(this);
    m_packManContext.setSearchGroupsTransaction(new TransactionWrapper(t, false, this));

    connect(t, SIGNAL(package(PackageKit::Transaction::Info, const QString &, const QString &)),
            this, SLOT(onAvailablePackage(PackageKit::Transaction::Info, const QString &, const QString &)));
   /* connect(t, SIGNAL(repoSignatureRequired(PackageKit::Client::SignatureInfo)),
            this, SLOT(onRepoSignatureRequired(PackageKit::Client::SignatureInfo)));
            */
    connect(t, SIGNAL(repoSignatureRequired(const QString &,
                                                                  const QString &,
                                                                  const QString &,
                                                                  const QString &,
                                                                  const QString &,
                                                                  const QString &,
                                                                  const QString &,
                                                                  PackageKit::Transaction::SigType)),
            this, SLOT(onRepoSignatureRequired(const QString &,
                                               const QString &,
                                               const QString &,
                                               const QString &,
                                               const QString &,
                                               const QString &,
                                               const QString &,
                                               PackageKit::Transaction::SigType)));
    connect(t, SIGNAL(finished(PackageKit::Transaction::Exit, uint)),
            this, SLOT(onFinished(PackageKit::Transaction::Exit, uint)));
    t->searchGroup(groupNames[group]);
}

void PackageManager::refreshRepos()
{
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif

    PackageKit::Transaction *t = new PackageKit::Transaction(this);

    m_repositories.clear();
    m_packManContext.setRepositories(&m_repositories);
    m_packManContext.setRefreshReposTransaction(new TransactionWrapper(t, true, this));

    connect(t, SIGNAL(repoDetail(const QString &repoId, const QString &description, bool enabled)),
            this, SLOT(onRepoDetail(const QString &repoId, const QString &description, bool enabled)));

    connect(t, SIGNAL(finished(PackageKit::Transaction::Exit,uint)),
            this, SLOT(onRefreshReposFinished(PackageKit::Transaction::Exit status, uint runtime)));

    m_refreshReposTransaction = t;

    t->getRepoList();
}

Package *PackageManager::get(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
#ifdef PACKAGEMANAGER_LOG	
   qDebug() << Q_FUNC_INFO;
#endif
   Package *packagePtr = NULL;

    packagePtr = m_packagesToBeInstalled.get(info, packageID, summary);
    if (packagePtr) {
        return packagePtr;	
    }
    packagePtr = m_packagesToBeRemoved.get(info, packageID, summary);
    if (packagePtr) {
        return packagePtr;	
    }
    packagePtr = m_packagesToBeUpdated.get(info, packageID, summary);
    if (packagePtr) {
        return packagePtr;	
    }

    qDebug() << QString("%1:%2% %3").arg(Q_FUNC_INFO).arg(__LINE__).arg("WARNING - Package is NULL");

    return NULL;
}


void PackageManager::onPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
#ifdef PACKAGEMANAGER_LOG	
   qDebug() << Q_FUNC_INFO;	
#endif
    PackageKit::Transaction *transaction = static_cast<PackageKit::Transaction*>(sender());

    if (transaction->transactionFlags() & PackageKit::Transaction::TransactionFlagSimulate) {
        PackageInfo *pPackageInfo = NULL;
        pPackageInfo = new PackageInfo(info, packageID, summary);

        QSharedPointer<PackageInfo> packagePtr(pPackageInfo);

        QString packageString = pPackageInfo->summary(); //)p->name() + " (" + p->version() + ")  " + p->summary();
        if (info == PackageKit::Transaction::InfoInstalling) {
            m_packagesToBeInstalled << packagePtr;
        } else if (info == PackageKit::Transaction::InfoUpdating) {
            m_packagesToBeUpdated << packagePtr;
        } else if (info == PackageKit::Transaction::InfoRemoving) {
            m_packagesToBeRemoved << packagePtr;
        } else if (info != PackageKit::Transaction::InfoFinished) {
            qDebug() << "*** info not handled: " << packageString;
        }
    } else {
//        qDebug() << (*packagePtr).name();
    }
}

void PackageManager::onInstalledPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
#ifdef PACKAGEMANAGER_LOG	
   qDebug() << Q_FUNC_INFO;	
#endif
    PackageInfo *p = new PackageInfo(info, packageID, summary);
    QSharedPointer<PackageInfo> packagePtr(p);
    m_installedPackagesModel->addPackage(packagePtr, false);
}

void PackageManager::onUpdateAvailablePackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
#ifdef PACKAGEMANAGER_LOG	
   qDebug() << Q_FUNC_INFO;	
#endif
    PackageInfo *p = new PackageInfo(info, packageID, summary);
    QSharedPointer<PackageInfo> packagePtr(p);

    m_updateAvailablePackagesModel->addPackage(packagePtr, true);
}

void PackageManager::onAvailablePackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
#ifdef PACKAGEMANAGER_LOG	
   qDebug() << Q_FUNC_INFO;
#endif   	
    PackageInfo *p = new PackageInfo(info, packageID, summary);
    QSharedPointer<PackageInfo> packagePtr(p);

    QString name = p->id();
    if (m_installedPackagesModel->findPackage(name) != 0) {
        return;
    }

    QString id = (*packagePtr).id();
    QStringList parts = id.split(';');
    if (parts.count() >= 4 && !parts.at(3).startsWith("@")) {
        m_availablePackagesModel->addPackage(packagePtr, false);
    }
}

void PackageManager::onFinished(PackageKit::Transaction::Exit status, uint runtime)
{
#ifdef PACKAGEMANAGER_LOG	
   qDebug() << Q_FUNC_INFO << status << runtime;	
#endif
    PackageKit::Transaction *t = qobject_cast<PackageKit::Transaction*>(sender());

    if (sender() == m_getUpdatesTransaction) {
//        delete m_getUpdatesTransaction;
        m_getUpdatesTransaction = 0;
    } else if (sender() == m_getPackagesTransaction) {
//        delete m_getPackagesTransaction;
        m_getPackagesTransaction = 0;
    }

    if (t && t->role() == PackageKit::Transaction::RoleRemovePackages) {
        m_packManContext.setSelectedGroup(PackageKit::Transaction::GroupUnknown);
        refreshUpdate();
        refreshInstalled();
    } else if (t && t->role() == PackageKit::Transaction::RoleUpdatePackages) {
        m_packManContext.setSelectedGroup(PackageKit::Transaction::GroupUnknown);
        refreshUpdate();
        refreshInstalled();
    } else if (t && t->role() == PackageKit::Transaction::RoleInstallPackages) {
        uint group = m_packManContext.selectedGroup();
        refreshCache(); // includes refreshUpdate();
        refreshInstalled();
        if (m_bSimulation == false) {
	        refreshAvailable(group);
	}
    }
}

void PackageManager::onRefreshCacheFinished(PackageKit::Transaction::Exit status, uint runtime)
{
#ifdef PACKAGEMANAGER_LOG	
   qDebug() << Q_FUNC_INFO;	
#endif
    if (status == PackageKit::Transaction::ExitSuccess) {
        refreshUpdate();
//        refreshInstalled();
    }

    m_refreshCacheTransaction = 0;
}

void PackageManager::onRefreshReposFinished(PackageKit::Transaction::Exit status, uint runtime)
{
#ifdef PACKAGEMANAGER_LOG	
	   qDebug() << Q_FUNC_INFO;
#endif	
    m_refreshReposTransaction = 0;
    m_packManContext.setRepositories(&m_repositories);
}

void PackageManager::uninstallMarkedPackages(bool simulate, bool autoremove)
{
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO << simulate << autoremove;
#endif
    QList<Package*> packageList = m_installedPackagesModel->packageMarkings()->packagesMarked();
    QStringList packageIDs;

    if (packageList.count() == 0)
        return;

    foreach (Package *package, packageList) {
        packageIDs <<  package->id();
    }
    PackageKit::Transaction *t = new PackageKit::Transaction(this);
    m_packManContext.setUninstallPackagesTransaction(new TransactionWrapper(t, true, this));

    connect(t, SIGNAL(package(QSharedPointer<PackageKit::Package>)),
            this, SLOT(onPackage(QSharedPointer<PackageKit::Package>)));

    connect(t, SIGNAL(finished(PackageKit::Transaction::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Transaction::Exit,uint)));

    if (simulate) {
        m_packagesToBeInstalled.clear();
        m_packagesToBeUpdated.clear();
        m_packagesToBeRemoved.clear();
     //   t->simulateRemovePackages(packagePtrList, autoremove);
    } else {
        t->removePackages(packageIDs, false, autoremove);
    }
}


void PackageManager::updateMarkedPackages(bool simulate, bool onlyTrusted)
{
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO << simulate << onlyTrusted;
#endif

    QList<Package*> packageList = m_updateAvailablePackagesModel->packageMarkings()->packagesMarked();
    QStringList packageIDs;

    if (packageList.count() == 0)
        return;

    foreach (Package *package, packageList) {
        packageIDs <<  package->id();
    }

    TransactionWrapper *tw = new TransactionWrapper(0, true, this);
    m_packManContext.setUpdatePackagesTransaction(tw);
    if (!testNetworkConnection(tw))
        return;

    PackageKit::Transaction *t = new PackageKit::Transaction( this);
    m_packManContext.setUpdatePackagesTransaction(new TransactionWrapper(t, true, this));

    connect(t, SIGNAL(package(QSharedPointer<PackageKit::Package>)),
            this, SLOT(onPackage(QSharedPointer<PackageKit::Package>)));

    connect(t, SIGNAL(finished(PackageKit::Transaction::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Transaction::Exit,uint)));

    if (simulate) {
        m_packagesToBeInstalled.clear();
        m_packagesToBeUpdated.clear();
        m_packagesToBeRemoved.clear();
        t->updatePackages(packageIDs, PackageKit::Transaction::TransactionFlagSimulate);
    } else {
        if (onlyTrusted) {
            t->updatePackages(packageIDs, PackageKit::Transaction::TransactionFlagOnlyTrusted);
        } 	
        else {
            t->updatePackages(packageIDs, PackageKit::Transaction::TransactionFlagNone);
        }
    }
}


void PackageManager::installMarkedPackages(bool simulate, bool onlyTrusted)
{
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO << simulate << onlyTrusted;
#endif
    m_bSimulation = false;

    QList<Package*> packageList = m_availablePackagesModel->packageMarkings()->packagesMarked();
    QStringList packageIDs;

    if (packageList.count() == 0)
        return;

    foreach (Package *package, packageList) {
        packageIDs <<  package->id();
    }

    TransactionWrapper *tw = new TransactionWrapper(0, true, this);
    m_packManContext.setInstallPackagesTransaction(tw);
    if (!testNetworkConnection(tw)) {
        return;
    }

    PackageKit::Transaction *t = new PackageKit::Transaction(this);
    m_packManContext.setInstallPackagesTransaction(new TransactionWrapper(t, true, this));

    connect(t, SIGNAL(package(PackageKit::Transaction::Info, const QString &, const QString &)),
            this, SLOT(onPackage(PackageKit::Transaction::Info, const QString &, const QString &)));

    connect(t, SIGNAL(finished(PackageKit::Transaction::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Transaction::Exit,uint)));

    if (simulate) {
        m_packagesToBeInstalled.clear();
        m_packagesToBeUpdated.clear();
        m_packagesToBeRemoved.clear();
        m_bSimulation = true;
        t->installPackages(packageIDs, PackageKit::Transaction::TransactionFlagSimulate);
    } else {
        if (onlyTrusted) {
            t->installPackages(packageIDs, PackageKit::Transaction::TransactionFlagOnlyTrusted);
        } 	
        else {
            t->installPackages(packageIDs, PackageKit::Transaction::TransactionFlagNone);
        }
    }
}

void PackageManager::onChanged()
{
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif
}

void PackageManager::onCategory(const QString &parent_id, const QString &cat_id, const QString &name, const QString &summary, const QString &icon)
{
#ifdef PACKAGEMANAGER_LOG	
   qDebug() << Q_FUNC_INFO;	
#endif
//    qDebug() << Q_FUNC_INFO << parent_id << cat_id << name << summary << icon;
}

void PackageManager::onErrorCode(PackageKit::Transaction::Error error, const QString &details)
{
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO << error << details;
#endif
}
/*
void PackageManager::onEulaRequired(const PackageKit::Client::EulaInfo &info)
{
    qDebug() << Q_FUNC_INFO << info.id << info.licenseAgreement << info.vendorName;
}
*/
/*
void PackageManager::onMediaChangeRequired(PackageKit::Transaction::MediaType type, const QString& id, const QString& text)
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
void PackageManager::onMessage(PackageKit::Transaction::Message type, const QString &message)
{
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO << type << message;
#endif
}

void PackageManager::onRepoDetail(const QString &repoId, const QString &description, bool enabled)
{
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO << repoId << description << enabled;
#endif
    m_repositories << new Repository(repoId, description, enabled, this);
}

void PackageManager::onRepoSignatureRequired(const QString &pid, const QString &repoName, const QString &keyUrl, const QString &keyUserid, const QString &keyId, const QString &keyFingerprint, const QString &keyTimestamp, PackageKit::Transaction::SigType type)
{
#ifdef PACKAGEMANAGER_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif
    /*

    Package *p = info.package.data();

    PackageKit::Transaction *t = new PackageKit::Transaction(QString(), this);
    connect(t, SIGNAL(errorCode(PackageKit::Transaction::Error,QString)),
            this, SLOT(onErrorCode(PackageKit::Transaction::Error,QString)));
    connect(t, SIGNAL(finished(PackageKit::Transaction::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Transaction::Exit,uint)));

    t->installSignature(info.type, info.keyId, info.package);

    PackageKit::Transaction *origTransaction = qobject_cast<PackageKit::Transaction*>(sender());
    if (origTransaction) {
        if (origTransaction->role() == PackageKit::Transaction::RoleRefreshCache) {
            m_refreshCacheTransaction = 0;
            refreshCache();
        } else if (origTransaction->role() == PackageKit::Transaction::RoleSearchGroup) {
            uint group = m_packManContext.selectedGroup();
            refreshAvailable(group);
        } else if (origTransaction->role() == PackageKit::Transaction::RoleGetUpdates) {
            m_getUpdatesTransaction = 0;
            refreshUpdate();
        }
    }
*/
}
/*
void PackageManager::onRequireRestart(PackageKit::Transaction::Restart type, const QSharedPointer<PackageKit::Package> &packagePtr)
{
    PackageKit::Package *p = packagePtr.data();
    qDebug() << Q_FUNC_INFO << type << p->name();
}
*/
