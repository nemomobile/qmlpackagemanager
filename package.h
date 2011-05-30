#ifndef __PACKAGE_H
#define __PACKAGE_H

#include <QObject>
#include <QPackageKit>

class Package : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY changed)
    Q_PROPERTY(QString displayName READ displayName NOTIFY changed)
    Q_PROPERTY(QString version READ version NOTIFY changed)
    Q_PROPERTY(bool isMarked READ isMarked NOTIFY changed)

public:
    enum DataAvailability { NotApplicable, Fetching, Available, NotAvailable };
    enum Marking { NoOperation = 0, Install = 1, Update = 2, Uninstall = 4 };
        Q_DECLARE_FLAGS(Markings, Marking)

    explicit Package(QSharedPointer<PackageKit::Package> package = QSharedPointer<PackageKit::Package>(), bool isUpdatePackage = false, QObject *parent = 0);

    QString displayName() const;
    QString filterName() const;

    QString name() const;
    QString version() const;

    QSharedPointer<PackageKit::Package> package() const;
    PackageKit::Package *basicInfo() const;
    PackageKit::Package *updateBasicInfo() const;
    PackageKit::Package::Details *details() const;
    PackageKit::Package::Details *updateDetails() const;
    PackageKit::Client::UpdateInfo *updateInfo() const;

    bool isUpdateAvailable();

    void mark(bool set);
    bool isMarked();

    void setPackage(QSharedPointer<PackageKit::Package>);

    DataAvailability detailsAvailability();
    DataAvailability updateDetailsAvailability();
    DataAvailability updateInfoAvailability();

    bool operator==(Package other) const;

signals:
    void changed();

public slots:
    void fetchPackageDetails();
    void fetchUpdateDetails();
    void fetchUpdateInfo();

private slots:
    void setPackageDetails(QSharedPointer<PackageKit::Package> packagePtr);
    void setUpdateDetails(QSharedPointer<PackageKit::Package> packagePtr);
    void setUpdateInfo(PackageKit::Client::UpdateInfo info);
    void onFinished(PackageKit::Enum::Exit, uint);

private:
    QString m_name;
    bool m_mark;

    QSharedPointer<PackageKit::Package> m_package;
    QSharedPointer<PackageKit::Package> m_detailsPackage;
    QSharedPointer<PackageKit::Package> m_updatePackage;
    QSharedPointer<PackageKit::Package> m_updateDetailsPackage;
    PackageKit::Client::UpdateInfo *m_updateInfo;

    PackageKit::Transaction *m_basicDetailsTransaction;
    PackageKit::Transaction *m_updateDetailsTransaction;
    PackageKit::Transaction *m_updateInfoTransaction;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Package::Markings)

#endif // PACKAGE_H
