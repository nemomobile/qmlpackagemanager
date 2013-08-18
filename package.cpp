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

#include "package.h"

#include "packagemanager.h"



Package::Package(QSharedPointer<PackageInfo> packagePtr, bool isUpdatePackage, QObject *parent) :
    QObject(parent),
    m_mark(false),
    m_updateInfo(0),
    m_basicDetailsTransaction(0),
    m_updateDetailsTransaction(0),
    m_updateInfoTransaction(0)
{
    if (isUpdatePackage) {
        m_updatePackage = packagePtr;
        m_name = PackageKit::Transaction::packageName(m_updatePackage->id());
    } else {
        m_package = packagePtr;
        m_name = PackageKit::Transaction::packageName(basicInfo()->id());
    }
}

bool Package::operator==(Package other) const
{
    return m_name == other.m_name;
}

bool Package::isMarked()
{
    return m_mark;
}

void Package::mark(bool set)
{
    if (m_mark != set) {
        m_mark = set;
        emit changed();
    }
}

QString Package::name() const
{
    return m_name;
}

QString Package::version() const
{
    PackageKit::Transaction::packageVersion(basicInfo()->id());
}


QString Package::displayName() const
{
    QSharedPointer<PackageInfo> package = this->package();
    if (package.isNull())
        return m_name;
    else
        return package.data()->summary();
}

QString Package::filterName() const
{
    if (m_package.isNull() && m_updatePackage.isNull())
        return m_name;
    else
        return basicInfo()->name() + ":::" + basicInfo()->summary();
}

QSharedPointer<PackageKit::Package> Package::package() const
{
    if (m_package.isNull())
        return m_updatePackage;
    else
        return m_package;
}

PackageInfo *Package::basicInfo() const
{
    if (m_package.isNull()) {
        if (m_updatePackage.isNull())
            return 0;
        else
            return m_updatePackage.data();
    } else {
        return m_package.data();
    }
}

DetailsInfo *Package::details() const
{
    if (m_detailsPackage.isNull()) {
        return updateDetails();
    } else {
        return (*m_detailsPackage.data()).details();
    }
}

PackageInfo *Package::updateBasicInfo() const
{
    if (m_updatePackage.isNull())
        return 0;
    else
        return m_updatePackage.data();
}

DetailsInfo *Package::updateDetails() const
{
    if (m_updateDetailsPackage.isNull()) {
        return 0;
    } else {
        PackageInfo *p = m_updateDetailsPackage.data();
        return p->details();
    }
}

UpdateDetails *Package::updateInfo() const
{
    return m_updateInfo;
}

bool Package::isUpdateAvailable()
{
    return !m_updatePackage.isNull();
}

void Package::setPackage(QSharedPointer<PackageInfo> packagePtr)
{
//    qDebug() << Q_FUNC_INFO << m_name;
    m_package = packagePtr;
    emit changed();
}

void Package::setPackageDetails(QSharedPointer<DetailsInfo> packagePtr)
{
//    qDebug() << Q_FUNC_INFO << m_name;
    m_detailsPackage = packagePtr;
    emit changed();
}

void Package::setUpdateDetails(QSharedPointer<DetailsInfo> packagePtr)
{
//    qDebug() << Q_FUNC_INFO << m_name;
    m_updateDetailsPackage = packagePtr;
    emit changed();
}

void Package::setUpdateInfo(const QString &packageID,
                            const QStringList &updates,
                            const QStringList &obsoletes,
                            const QStringList &vendorUrls,
                            const QStringList &bugzillaUrls,
                            const QStringList &cveUrls,
                            PackageKit::Transaction::Restart restart,
                            const QString &updateText,
                            const QString &changelog,
                            PackageKit::Transaction::UpdateState state,
                            const QDateTime &issued,
                            const QDateTime &updated)
{
//    qDebug() << Q_FUNC_INFO << m_name;
    if (m_updateInfo)
        delete m_updateInfo;

    m_updateInfo = new UpdateDetails(packageID, updates, obsoletes, vendorUrls, bugzillaUrls,
                                     cveUrls, restart, updateText, changelog, state, issued, updated);

    emit changed();
}

void Package::fetchPackageDetails()
{
//    qDebug() << Q_FUNC_INFO << m_name;

    if (m_basicDetailsTransaction || m_package.isNull() || !m_detailsPackage.isNull())
        return;

    m_basicDetailsTransaction = new PackageKit::Transaction(0, this);

    connect(m_basicDetailsTransaction, SIGNAL(details(QSharedPointer<PackageKit::Package>)),
            this, SLOT(setPackageDetails(QSharedPointer<PackageKit::Package>)));
    connect(m_basicDetailsTransaction, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));
    m_basicDetailsTransaction->getDetails(m_package);
}

void Package::fetchUpdateDetails()
{
//    qDebug() << Q_FUNC_INFO << m_name;

    if (m_updateDetailsTransaction || m_updatePackage.isNull() || !m_updateDetailsPackage.isNull())
        return;

    m_updateDetailsTransaction = new PackageKit::Transaction(0, this);

    connect(m_updateDetailsTransaction, SIGNAL(details(QSharedPointer<PackageKit::Package>)),
            this, SLOT(setUpdateDetails(QSharedPointer<PackageKit::Package>)));
    connect(m_updateDetailsTransaction, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));
    m_updateDetailsTransaction->getDetails(m_updatePackage);
}

void Package::fetchUpdateInfo()
{
//    qDebug() << Q_FUNC_INFO << m_name;

    if (m_updateInfoTransaction || m_updatePackage.isNull() || m_updateInfo)
        return;

    m_updateInfoTransaction = new PackageKit::Transaction(0, this);

    connect(m_updateInfoTransaction, SIGNAL(updateDetail(const QString &packageID,
                                                         const QStringList &updates,
                                                         const QStringList &obsoletes,
                                                         const QStringList &vendorUrls,
                                                         const QStringList &bugzillaUrls,
                                                         const QStringList &cveUrls,
                                                         PackageKit::Transaction::Restart restart,
                                                         const QString &updateText,
                                                         const QString &changelog,
                                                         PackageKit::Transaction::UpdateState state,
                                                         const QDateTime &issued,
                                                         const QDateTime &updated)),
            this, SLOT(setUpdateInfo(const QString &packageID,
                                     const QStringList &updates,
                                     const QStringList &obsoletes,
                                     const QStringList &vendorUrls,
                                     const QStringList &bugzillaUrls,
                                     const QStringList &cveUrls,
                                     PackageKit::Transaction::Restart restart,
                                     const QString &updateText,
                                     const QString &changelog,
                                     PackageKit::Transaction::UpdateState state,
                                     const QDateTime &issued,
                                     const QDateTime &updated)));
    connect(m_updateInfoTransaction, SIGNAL(finished(PackageKit::Transaction::Exit status, uint runtime)),
            this, SLOT(onFinished(PackageKit::Transaction::Exit status, uint runtime)));
    m_updateInfoTransaction->getUpdateDetail(m_updatePackage);
}

Package::DataAvailability Package::detailsAvailability()
{
    return m_package.isNull()? updateDetailsAvailability():
                               m_detailsPackage? Available:
                                                 m_basicDetailsTransaction? Fetching:
                                                                            NotAvailable;
}

Package::DataAvailability Package::updateDetailsAvailability()
{
    return m_updatePackage.isNull()? NotApplicable:
                                     m_updateDetailsPackage? Available:
                                                             m_updateDetailsTransaction? Fetching:
                                                                                         NotAvailable;
}

Package::DataAvailability Package::updateInfoAvailability()
{
    return m_updatePackage.isNull()? NotApplicable:
                                     m_updateInfo? Available:
                                                   m_updateInfoTransaction? Fetching:
                                                                            NotAvailable;
}

void Package::onFinished(PackageKit::Enum::Exit status, uint runtime)
{
//    qDebug() << Q_FUNC_INFO << status << runtime << m_name;
    if (sender() == m_basicDetailsTransaction)
        m_basicDetailsTransaction = 0;
    else if (sender() == m_updateDetailsTransaction)
        m_updateDetailsTransaction = 0;
    else if (sender() == m_updateInfoTransaction)
        m_updateInfoTransaction = 0;

    emit changed();
}

