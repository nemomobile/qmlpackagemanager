#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H

#include "qmlapplicationviewer.h"

#include "packmancontext.h"
#include "packagemodel.h"
#include "packagelist.h"
#include "packagegroup.h"

#include <QObject>
#include <QStringList>
#include <QPackageKit>
#include <QDeclarativeContext>
#include <QSortFilterProxyModel>

class FilterPackageModel;

class PackageManager : public QObject
{
    Q_OBJECT

public:
    static PackageManager *instance();

    explicit PackageManager(QmlApplicationViewer *viewer, QObject *parent = 0);

signals:
    void updateStateChanged();
    void cacheRefreshed();
    void categoriesRefreshed();

public slots:
    void refreshAll();
    void refreshCache();
    void refreshUpdate();
    void refreshInstalled();
    void refreshAvailable(uint group);

    void resetMarkings(uint marking);
    void setFilterString(const QString &text);

    void uninstallMarkedPackages(bool simulate, bool autoremove);
    void updateMarkedPackages(bool simulate, bool onlyTrusted);
    void installMarkedPackages(bool simulate, bool onlyTrusted);

private slots:
    void refreshInstalledWhenUpdateComplete();

    void onPackage(QSharedPointer<PackageKit::Package> packagePtr);
    void onInstalledPackage(QSharedPointer<PackageKit::Package> packagePtr);
    void onUpdateAvailablePackage(QSharedPointer<PackageKit::Package> packagePtr);
    void onAvailablePackage(QSharedPointer<PackageKit::Package> packagePtr);
    void onFinished(PackageKit::Enum::Exit,uint);

    void onChanged();
    void onCategory(const QString &parent_id, const QString &cat_id, const QString &name, const QString &summary, const QString &icon);
    void onErrorCode(PackageKit::Enum::Error error, const QString& details);
    void onEulaRequired(const PackageKit::Client::EulaInfo &info);
    void onMediaChangeRequired(PackageKit::Enum::MediaType type, const QString& id, const QString& text);
    void onFiles(const QSharedPointer<PackageKit::Package> &package, const QStringList &filenames);
    void onMessage(PackageKit::Enum::Message type, const QString &message);
    void onRepoDetail(const QString& repoId, const QString& description, bool enabled);
    void onRepoSignatureRequired(const PackageKit::Client::SignatureInfo &info);
    void onRequireRestart(PackageKit::Enum::Restart type, const QSharedPointer<PackageKit::Package> &package);

    void onRefreshCacheFinished(PackageKit::Enum::Exit,uint);

 private:
    static PackageManager *m_packageManager;

    PackManContext m_packManContext;

    QmlApplicationViewer *m_viewer;
    PackageKit::Client *m_packageKit;

    PackageKit::Transaction *m_refreshCacheTransaction;
    PackageKit::Transaction *m_getPackagesTransaction;
    PackageKit::Transaction *m_getUpdatesTransaction;
    PackageKit::Transaction *m_searchGroupsTransaction;

    QDeclarativeContext *m_qmlContext;

    PackageModel *m_installedPackagesModel;
    PackageModel *m_availablePackagesModel;
    PackageModel *m_updateAvailablePackagesModel;
    FilterPackageModel *m_installedPackagesFilterModel;
    FilterPackageModel *m_availablePackagesFilterModel;
    FilterPackageModel *m_updateAvailablePackagesFilterModel;

    PackageList m_packagesToBeInstalled;
    PackageList m_packagesToBeRemoved;
    PackageList m_packagesToBeUpdated;

    PackageGroupList m_packageGroups;
};

#endif // PACKAGEMANAGER_H
