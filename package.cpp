#include "package.h"

#include "packagemanager.h"

#include <QPackageKit>

Package::Package(QSharedPointer<PackageKit::Package> packagePtr, bool isUpdatePackage, QObject *parent) :
    QObject(parent),
    m_mark(false),
    m_updateInfo(0),
    m_basicDetailsTransaction(0),
    m_updateDetailsTransaction(0),
    m_updateInfoTransaction(0)
{
    if (isUpdatePackage) {
        m_updatePackage = packagePtr;
        m_name = updateBasicInfo()->name();
    } else {
        m_package = packagePtr;
        m_name = basicInfo()->name();
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
    return basicInfo()->version();
}


QString Package::displayName() const
{
    QSharedPointer<PackageKit::Package> package = this->package();
    if (package.isNull())
        return m_name;
    else
        return (*package).summary();
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

PackageKit::Package *Package::basicInfo() const
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

PackageKit::Package::Details *Package::details() const
{
    if (m_detailsPackage.isNull()) {
        return updateDetails();
    } else {
        return (*m_detailsPackage.data()).details();
    }
}

PackageKit::Package *Package::updateBasicInfo() const
{
    if (m_updatePackage.isNull())
        return 0;
    else
        return m_updatePackage.data();
}

PackageKit::Package::Details *Package::updateDetails() const
{
    if (m_updateDetailsPackage.isNull()) {
        return 0;
    } else {
        PackageKit::Package *p = m_updateDetailsPackage.data();
        return p->details();
    }
}

PackageKit::Client::UpdateInfo *Package::updateInfo() const
{
    return m_updateInfo;
}

bool Package::isUpdateAvailable()
{
    return !m_updatePackage.isNull();
}

void Package::setPackage(QSharedPointer<PackageKit::Package> packagePtr)
{
//    qDebug() << Q_FUNC_INFO << m_name;
    m_package = packagePtr;
    emit changed();
}

void Package::setPackageDetails(QSharedPointer<PackageKit::Package> packagePtr)
{
//    qDebug() << Q_FUNC_INFO << m_name;
    m_detailsPackage = packagePtr;
    emit changed();
}

void Package::setUpdateDetails(QSharedPointer<PackageKit::Package> packagePtr)
{
//    qDebug() << Q_FUNC_INFO << m_name;
    m_updateDetailsPackage = packagePtr;
    emit changed();
}

void Package::setUpdateInfo(PackageKit::Client::UpdateInfo info)
{
//    qDebug() << Q_FUNC_INFO << m_name;
    if (m_updateInfo)
        delete m_updateInfo;

    m_updateInfo = new PackageKit::Client::UpdateInfo();
    memcpy(m_updateInfo, &info, sizeof(info));

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

    connect(m_updateInfoTransaction, SIGNAL(updateDetail(PackageKit::Client::UpdateInfo)),
            this, SLOT(setUpdateInfo(PackageKit::Client::UpdateInfo)));
    connect(m_updateInfoTransaction, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));
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

