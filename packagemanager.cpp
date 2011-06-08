#include "packagemanager.h"

#include "filterpackagemodel.h"

#include <QDeclarativeContext>
#include <QGraphicsObject>
#include <QSortFilterProxyModel>

#include <QPackageKit>
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

PackageManager::PackageManager(QmlApplicationViewer *viewer, QObject *parent) :
    QObject(parent),
    m_refreshCacheTransaction(0),
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

    QTimer::singleShot(3000, this, SLOT(refreshAll()));
}

PackageManager *PackageManager::instance()
{
    return m_packageManager;
}

void PackageManager::refreshCache()
{
//    qDebug() << Q_FUNC_INFO;

    if (m_refreshCacheTransaction)
        return;

    m_refreshCacheTransaction = new PackageKit::Transaction(0, this);
    m_packManContext.setRefreshCacheTransaction(m_refreshCacheTransaction);

    connect(m_refreshCacheTransaction, SIGNAL(changed()), this, SLOT(onChanged()));
    connect(m_refreshCacheTransaction, SIGNAL(message(PackageKit::Enum::Message,QString)),
            this, SLOT(onMessage(PackageKit::Enum::Message,QString)));
    connect(m_refreshCacheTransaction, SIGNAL(repoSignatureRequired(PackageKit::Client::SignatureInfo)),
            this, SLOT(onRepoSignatureRequired(PackageKit::Client::SignatureInfo)));
    connect(m_refreshCacheTransaction, SIGNAL(errorCode(PackageKit::Enum::Error,QString)),
            this, SLOT(onErrorCode(PackageKit::Enum::Error,QString)));
    connect(m_refreshCacheTransaction, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
            this, SLOT(onRefreshCacheFinished(PackageKit::Enum::Exit,uint)));

    m_refreshCacheTransaction->refreshCache(true);

    m_packManContext.setSelectedGroup(PackageKit::Enum::UnknownGroup);
    refreshUpdate();
    refreshInstalled();
}

void PackageManager::refreshAll()
{
    refreshUpdate();
    refreshInstalled();
}

void PackageManager::refreshUpdate()
{
//    qDebug() << Q_FUNC_INFO;

    if (m_getUpdatesTransaction)
        return; // still updating

    m_updateAvailablePackagesModel->clear();

    m_getUpdatesTransaction = new PackageKit::Transaction(0, this);
    m_packManContext.setGetUpdatesTransaction(m_getUpdatesTransaction);

    emit updateStateChanged();
    connect(m_getUpdatesTransaction, SIGNAL(package(QSharedPointer<PackageKit::Package>)),
            this, SLOT(onUpdateAvailablePackage(QSharedPointer<PackageKit::Package>)));
    connect(m_getUpdatesTransaction, SIGNAL(finished(PackageKit::Enum::Exit,uint)), this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));
    m_getUpdatesTransaction->getUpdates(); // PackageKit::Enum::FilterInstalled);
}

void PackageManager::refreshInstalled()
{
//    qDebug() << Q_FUNC_INFO;

    m_installedPackagesModel->clear();

    m_getPackagesTransaction = new PackageKit::Transaction(0, this);
    m_packManContext.setGetPackagesTransaction(m_getPackagesTransaction);

    connect(m_getPackagesTransaction, SIGNAL(package(QSharedPointer<PackageKit::Package>)),
        this, SLOT(onInstalledPackage(QSharedPointer<PackageKit::Package>)));
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
    m_packManContext.setSearchGroupsTransaction(t);

    connect(t, SIGNAL(package(QSharedPointer<PackageKit::Package>)),
            this, SLOT(onAvailablePackage(QSharedPointer<PackageKit::Package>)));
    connect(t, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));
    t->searchGroups(groupNames[group]); // , PackageKit::Enum::FilterNotInstalled);
//    t->getPackages(PackageKit::Enum::FilterNotInstalled);
}

void PackageManager::refreshRepos()
{
    qDebug() << Q_FUNC_INFO;

    PackageKit::Transaction *t = new PackageKit::Transaction(0, this);

    connect(t, SIGNAL(repoDetail(QString,QString,bool)),
            this, SLOT(onRepoDetail(QString,QString,bool)));

    t->getRepoList();
}

void PackageManager::onPackage(QSharedPointer<PackageKit::Package> packagePtr)
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
        qDebug() << (*packagePtr).name();
    }
}

void PackageManager::onInstalledPackage(QSharedPointer<PackageKit::Package> packagePtr)
{
//    qDebug() << Q_FUNC_INFO << (*packagePtr).id()  << (*packagePtr).info();

    m_installedPackagesModel->addPackage(packagePtr, false);
}

void PackageManager::onUpdateAvailablePackage(QSharedPointer<PackageKit::Package> packagePtr)
{
//    qDebug() << Q_FUNC_INFO << (*packagePtr).id()  << (*packagePtr).info();

    m_updateAvailablePackagesModel->addPackage(packagePtr, true);
}

void PackageManager::onAvailablePackage(QSharedPointer<PackageKit::Package> packagePtr)
{
    qDebug() << Q_FUNC_INFO << (*packagePtr).id()  << (*packagePtr).info();

    QString name = (*packagePtr).name();
    if (m_installedPackagesModel->findPackage(name) != 0)
        return;

    QString id = (*packagePtr).id();
    QStringList parts = id.split(';');
    if (parts.count() >= 4 && !parts.at(3).startsWith("@"))
        m_availablePackagesModel->addPackage(packagePtr, false);
}

void PackageManager::onFinished(PackageKit::Enum::Exit status, uint runtime)
{
    PackageKit::Transaction *t = qobject_cast<PackageKit::Transaction*>(sender());

    qDebug() << Q_FUNC_INFO << status << runtime << t->role();

    if (sender() == m_getUpdatesTransaction) {
//        delete m_getUpdatesTransaction;
        m_getUpdatesTransaction = 0;
        emit updateStateChanged();
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
        refreshCache(); // includes refreshUpdate(); refreshInstalled();
        refreshAvailable(group);
    }
}

void PackageManager::onRefreshCacheFinished(PackageKit::Enum::Exit exitCode, uint duration)
{
//    qDebug() << Q_FUNC_INFO << exitCode << duration;

//    delete m_refreshCacheTransaction;
    m_refreshCacheTransaction = 0;
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
    m_packManContext.setUninstallPackagesTransaction(t);

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

    PackageKit::Transaction *t = new PackageKit::Transaction(0, this);
    m_packManContext.setUpdatePackagesTransaction(t);

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

    PackageKit::Transaction *t = new PackageKit::Transaction(0, this);
    m_packManContext.setInstallPackagesTransaction(t);

    connect(t, SIGNAL(package(QSharedPointer<PackageKit::Package>)),
            this, SLOT(onPackage(QSharedPointer<PackageKit::Package>)));

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
    //oqDebug() << Q_FUNC_INFO;
}

void PackageManager::onCategory(const QString &parent_id, const QString &cat_id, const QString &name, const QString &summary, const QString &icon)
{
    qDebug() << Q_FUNC_INFO << parent_id << cat_id << name << summary << icon;
}

void PackageManager::onErrorCode(PackageKit::Enum::Error error, const QString& details)
{
    qDebug() << Q_FUNC_INFO << error << details;
}

void PackageManager::onEulaRequired(const PackageKit::Client::EulaInfo &info)
{
    qDebug() << Q_FUNC_INFO << info.id << info.licenseAgreement << info.vendorName;
}

void PackageManager::onMediaChangeRequired(PackageKit::Enum::MediaType type, const QString& id, const QString& text)
{
    qDebug() << Q_FUNC_INFO << type << id << text;
}

void PackageManager::onFiles(const QSharedPointer<PackageKit::Package> &packagePtr, const QStringList &filenames)
{
    PackageKit::Package *p = packagePtr.data();
    qDebug() << Q_FUNC_INFO << p->name() << filenames;
}

void PackageManager::onMessage(PackageKit::Enum::Message type, const QString &message)
{
    qDebug() << Q_FUNC_INFO << type << message;
}

void PackageManager::onRepoDetail(const QString& repoId, const QString& description, bool enabled)
{
    qDebug() << Q_FUNC_INFO << repoId << description << enabled;
}

void PackageManager::onRepoSignatureRequired(const PackageKit::Client::SignatureInfo &info)
{
    PackageKit::Package *p = info.package.data();
    qDebug() << Q_FUNC_INFO << p->name() << info.repoId;

    PackageKit::Transaction *t = new PackageKit::Transaction(QString(), this);
    t->installSignature(info.type, info.keyId, info.package);

    connect(t, SIGNAL(errorCode(PackageKit::Enum::Error,QString)),
            this, SLOT(onErrorCode(PackageKit::Enum::Error,QString)));
    connect(t, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));
}

void PackageManager::onRequireRestart(PackageKit::Enum::Restart type, const QSharedPointer<PackageKit::Package> &packagePtr)
{
    PackageKit::Package *p = packagePtr.data();
    qDebug() << Q_FUNC_INFO << type << p->name();
}

