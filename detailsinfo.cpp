#include "detailsinfo.h"
#include "daemon.h"
#include <QDebug>

//#define DETAILSINFO_LOG	


DetailsInfo::DetailsInfo(QSharedPointer<PackageInfo> p,
    const QString& license,
    const QString& group,
    const QString& detail,
    const QString& url,
    qulonglong size)
    //:
//        d_ptr(new DetailsPrivate)
{
#ifdef DETAILSINFO_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif
    m_package = p;
    m_license = license;
  //  m_group = QString(); //)(PackageKit::Transaction::Group)PackageKit::Daemon::enumFromString<PackageKit::Transaction::Group>(group, "Group", "Group");
    m_description = detail;
    m_url = url;
    m_size = size;
}

DetailsInfo::~DetailsInfo()
{
#ifdef DETAILSINFO_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif
   // delete d_ptr;
}

QSharedPointer<PackageInfo> DetailsInfo::package() const
{
#ifdef DETAILSINFO_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif

    return m_package;
}

QString DetailsInfo::license() const
{
#ifdef DETAILSINFO_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif

    return m_license;
}

PackageKit::Transaction::Group DetailsInfo::group() const
{
#ifdef DETAILSINFO_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif

    return m_group;
}

QString DetailsInfo::description() const
{
#ifdef DETAILSINFO_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif

    return m_description;
}

QString DetailsInfo::url() const
{
#ifdef DETAILSINFO_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif

    return m_url;
}

qulonglong DetailsInfo::size() const
{
#ifdef DETAILSINFO_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif

    return m_size;
}
