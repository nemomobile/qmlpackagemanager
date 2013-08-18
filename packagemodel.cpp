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

#include "packagemodel.h"

#include "packagemanager.h"

#include <QHash>
#include <QQmlContext>
//#include <QtDeclarative>

// ------------------------------------------------------------

void PackageModel::setRoleNames(QHash<int, QByteArray> roles)
{
    m_roles = roles;
}

PackageModel::PackageModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_addTimer.setInterval(200);
    m_addTimer.setSingleShot(true);
    connect(&m_addTimer, SIGNAL(timeout()), this, SLOT(flushBuffer()));

    QHash<int, QByteArray> roles;
    roles[DisplayNameRole] = "packageDisplayName";
    roles[IsMarkedRole] = "packageIsMarked";

    roles[PackageObjectRole] = "packageObject";

    roles[IdRole] = "packageId";
    roles[NameRole] = "packageName";
    roles[VersionRole] = "packageVersion";
    roles[ArchRole] = "packageArch";
    roles[DataRole] = "packageData";
    roles[SummaryRole] = "packageSummary";
    roles[InfoRole] = "packageInfo";
    roles[IconRole] = "packageIcon";

    roles[IsUpdateAvailableRole] = "packageIsUpdateAvailable";

    roles[UpdateIdRole] = "packageUpdateId";
    roles[UpdateNameRole] = "packageUpdateName";
    roles[UpdateVersionRole] = "packageUpdateVersion";
    roles[UpdateArchRole] = "packageUpdateArch";
    roles[UpdateDataRole] = "packageUpdateData";
    roles[UpdateSummaryRole] = "packageUpdateSummary";
    roles[UpdateInfoRole] = "packageUpdateInfo";
    roles[UpdateIconRole] = "packageUpdateIcon";

    roles[DetailsAvailableRole] = "packageDetailsAvailable";
    roles[DependsAvailableRole] = "packageDependsAvailable";
    roles[UpdateDetailsAvailableRole] = "packageUpdateDetailsAvailable";
    roles[UpdateInfoAvailableRole] = "packageUpdateInfoAvailable";

    roles[DetailsLicenseRole] = "packageDetailsLicense";
    roles[DetailsGroupRole] = "packageDetailsGroup";
    roles[DetailsDescriptionRole] = "packageDetailsDescription";
    roles[DetailsUrlRole] = "packageDetailsUrl";
    roles[DetailsSizeRole] = "packageDetailsSize";

    roles[UpdateDetailsLicenseRole] = "packageUpdateDetailsLicense";
    roles[UpdateDetailsGroupRole] = "packageUpdateDetailsGroup";
    roles[UpdateDetailsDescriptionRole] = "packageUpdateDetailsDescription";
    roles[UpdateDetailsUrlRole] = "packageUpdateDetailsUrl";
    roles[UpdateDetailsSizeRole] = "packageUpdateDetailsSize";

    roles[UpdateUpdatesRole] = "packageUpdateUpdates";
    roles[UpdateObsoletesRole] = "packageUpdateObsoletes";
    roles[UpdateVendorUrlRole] = "packageUpdateVendorUrl";
    roles[UpdateBugzillaUrlRole] = "packageUpdateBugzillaUrl";
    roles[UpdateCveUrlRole] = "packageUpdateCveUrl";
    roles[UpdateRestartNeededRole] = "packageUpdateRestart";
    roles[UpdateTextRole] = "packageUpdateText";
    roles[UpdateChangeLogRole] = "packageUpdateChangeLog";
    roles[UpdateStateRole] = "packageUpdateState";
    roles[UpdateIssuedRole] = "packageUpdateIssued";
    roles[UpdateUpdatedRole] = "packageUpdateUpdated";

    setRoleNames(roles);

    connect(&m_packageMarkings, SIGNAL(changed()), this, SIGNAL(markedCountChanged()));
}

QVariant PackageModel::data(const QModelIndex &index, int role) const
{
//    qDebug() << Q_FUNC_INFO << index << role;

    if (!index.isValid() || index.row() >= m_packageList.count())
        return QVariant("");

    // Package *package = m_packageList.at(index.row());
    Package *package = static_cast<Package*>(index.internalPointer());

    QObject *object = static_cast<QObject*>(package);
    if (role == PackageObjectRole)
        return qVariantFromValue(object);

    QSharedPointer <PackageInfo> data =  package->package();
    PackageInfo *updateData = package->updateBasicInfo();
    DetailsInfo *details = package->details();
    DetailsInfo *updateDetails = package->updateDetails();

    if (data.isNull())
        return QVariant("");

    UpdateDetails *updateInfo = package->updateInfo();

    int group = (role & DetailMask);
    switch (group) {
    case DetailsRoleMask:
        QTimer::singleShot(1, package, SLOT(fetchPackageDetails()));
        break;
    case UpdateDetailsRoleMask:
        QTimer::singleShot(1, package, SLOT(fetchUpdateDetails()));
        break;
    case UpdateInfoRoleMask:
        QTimer::singleShot(1, package, SLOT(fetchUpdateInfo()));
        break;
    default:
        break;
    }

    switch (role) {

    case DisplayNameRole:   return package->displayName();
    case FilterNameRole:    return package->filterName();
    case IsMarkedRole:      return QVariant(package->isMarked());

    case IdRole:        return data->id();
    case NameRole:      return PackageKit::Transaction::packageName(data.data()->id());
    case VersionRole:   return PackageKit::Transaction::packageVersion(data.data()->id());
    case ArchRole:      return PackageKit::Transaction::packageArch(data.data()->id());
    case DataRole:      return PackageKit::Transaction::packageData(data.data()->id());
    case SummaryRole:   return data.data()->summary();
    case InfoRole:      return data.data()->info();
    case IconRole:      return PackageKit::Transaction::packageIcon(data.data()->id());

    case IsUpdateAvailableRole:
        return package->isUpdateAvailable();

    case UpdateIdRole:
        return updateData? QVariant(updateData->id()): QVariant("");
    case UpdateNameRole:
        return updateData? QVariant(PackageKit::Transaction::packageName(updateData->id())): QVariant("");
    case UpdateVersionRole:
        return updateData? QVariant(PackageKit::Transaction::packageVersion(updateData->id())): QVariant("");
    case UpdateArchRole:
        return updateData? QVariant(PackageKit::Transaction::packageArch(updateData->id())): QVariant("");
    case UpdateDataRole:
        return updateData? QVariant(PackageKit::Transaction::packageData(updateData->id())): QVariant("");
    case UpdateSummaryRole:
        return updateData? QVariant(updateData->summary()): QVariant("");
    case UpdateInfoRole:
        return updateData? QVariant(updateData->info()): QVariant("");
    case UpdateIconRole:
        return updateData? QVariant(PackageKit::Transaction::packageIcon(updateData->id())): QVariant("");

    case DetailsLicenseRole:
        return details? QVariant(details->license()): QVariant("");
    case DetailsGroupRole:
        return details? QVariant(details->group()): QVariant("");
    case DetailsDescriptionRole:
        return details? QVariant(details->description()): QVariant("");
    case DetailsUrlRole:
        return details? QVariant(details->url()): QVariant("");
    case DetailsSizeRole:
        return details? QVariant(QVariant(details->size())): QVariant("");

    case UpdateDetailsLicenseRole:
        return updateDetails? QVariant(updateDetails->license()): QVariant("");
    case UpdateDetailsGroupRole:
        return updateDetails? QVariant(updateDetails->group()): QVariant("");
    case UpdateDetailsDescriptionRole:
        return updateDetails? QVariant(updateDetails->description()): QVariant("");
    case UpdateDetailsUrlRole:
        return updateDetails? QVariant(updateDetails->url()): QVariant("");
    case UpdateDetailsSizeRole:
        return updateDetails? QVariant(updateDetails->size()): QVariant("");

    case DetailsAvailableRole:
        return package->detailsAvailability();
    case DependsAvailableRole:
        return "TODO";
    case UpdateDetailsAvailableRole:
        return package->updateDetailsAvailability();
    case UpdateInfoAvailableRole:
        return package->updateInfoAvailability();

//  These should be converted to string lists etc.
//    case UpdateUpdatesRole:
//        return updateInfo->updates;
//    case UpdateObsoletesRole:
//        return updateInfo->obsoletes;

    case UpdateVendorUrlRole:
        return updateInfo? QVariant(updateInfo->vendorUrl): QVariant("");
    case UpdateBugzillaUrlRole:
        return updateInfo? QVariant(updateInfo->bugzillaUrl): QVariant("");
    case UpdateCveUrlRole:
        return updateInfo? QVariant(updateInfo->cveUrl): QVariant("");
    case UpdateRestartNeededRole:
        return updateInfo? QVariant(updateInfo->restart): QVariant("");
    case UpdateTextRole:
        return updateInfo? QVariant(updateInfo->updateText): QVariant("");
    case UpdateChangeLogRole:
        return updateInfo? QVariant(updateInfo->changelog): QVariant("");
    case UpdateStateRole:
        return updateInfo? QVariant(updateInfo->state): QVariant("");
    case UpdateIssuedRole:
        return updateInfo && updateInfo->issued.isValid()? QVariant(updateInfo->issued.toString()): QVariant("-");
    case UpdateUpdatedRole:
        return updateInfo && updateInfo->updated.isValid()? QVariant(updateInfo->updated.toString()): QVariant("-");

    default:
        return QVariant("");
    }

}

void PackageModel::addPackage(QSharedPointer<PackageInfo> packagePtr, bool isUpdatePackage)
{
    // qDebug() << Q_FUNC_INFO << isUpdatePackage;

    Package *p = new Package(packagePtr, isUpdatePackage, this);
    connect(p, SIGNAL(changed()), this, SLOT(onPackageChanged()));
    m_packageBuffer << p;
    if (isUpdatePackage)
        m_packageMarkings.mark(p, true);

    if (!m_addTimer.isActive())
        m_addTimer.start();
}

void PackageModel::flushBuffer()
{
    beginInsertRows(QModelIndex(), m_packageList.count(), m_packageList.count()+m_packageBuffer.count()-1);
    m_packageList << m_packageBuffer;
    m_packageBuffer.clear();
    emit countChanged();
    endInsertRows();
}

int PackageModel::rowCount(const QModelIndex &parent) const
{
//    qDebug() << Q_FUNC_INFO << m_packageList.count();
    if (!parent.isValid())
        return m_packageList.count();
    else
        return 0;
}

void PackageModel::clear()
{
    beginResetModel();
    m_packageMarkings.resetMarkings();
    m_packageList.clear();
    endResetModel();
    emit countChanged();
}

Package *PackageModel::findPackage(QSharedPointer<PackageInfo> packagePtr)
{
    PackageKit::Package *p1 = packagePtr.data();
    QString name1 = p1? p1->name(): QString();

    return findPackage(name1);
}

Package *PackageModel::findPackage(const QString &name)
{
    for (int i = 0; i < m_packageList.count(); i++) {
        Package *package = m_packageList.at(i);
        PackageKit::Package *p2 = package->basicInfo();
        QString name2 = p2? p2->name(): QString();
        if (name == name2)
            return package;
    }
    return 0;
}

void PackageModel::onPackageChanged()
{
//    qDebug() << Q_FUNC_INFO;

    Package *p = dynamic_cast<Package*>(sender());
    if (p) {
        int row = m_packageList.indexOf(p);
        if (row >= 0) {
            QModelIndex packageIndex = index(row, 0);
            emit dataChanged(packageIndex, packageIndex);
        }
    }
}

void PackageModel::mark(int row, bool set)
{
//    qDebug() << Q_FUNC_INFO << row << set;

    Package *package = m_packageList.at(row);

    m_packageMarkings.mark(package, set);
}

void PackageModel::resetMarkings()
{
    m_packageMarkings.resetMarkings();
}

int PackageModel::markedCount()
{
    return m_packageMarkings.count();
}

QModelIndex PackageModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid() && row >= 0 && row < m_packageList.count() && column == 0)
        return createIndex(row, column, m_packageList.at(row));
    else
        return QModelIndex();
}
