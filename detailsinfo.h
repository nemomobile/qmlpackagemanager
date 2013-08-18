#ifndef DETAILSINFO_H
#define DETAILSINFO_H

#include <QObject>
#include "transaction.h"


class DetailsInfo
{
public:
    DetailsInfo(QSharedPointer<PackageInfo> p, const QString& license, const QString& group, const QString& detail, const QString& url, qulonglong size);
    /**
         * Destructor
         */
    ~DetailsInfo();

    /**
         * Returns the package these details are linked to
         * \return the Package object to which these details are related
         */
    QSharedPointer<PackageInfo> package() const;

    /**
         * Returns the package's license
         */
    QString license() const;

    /**
         * Returns the package's group (for example Multimedia, Editors...)
         */
    PackageKit::Transaction::Group group() const;

    /**
         * Returns the package's long description
         */
    QString description() const;

    /**
         * Returns the software's homepage url
         */
    QString url() const;

    /**
         * Returns the package's size
         */
    qulonglong size() const;

private:
    QSharedPointer<PackageInfo> m_package;
    QString m_license;
    PackageKit::Transaction::Group m_group;
    QString m_description;
    QString m_url;
    uint m_size;

};


#endif // DETAILSINFO_H
