#include "detailsinfo.h"



DetailsInfo::DetailsInfo(QSharedPointer<PackageInfo> p, const QString& license, const QString& group, const QString& detail, const QString& url, qulonglong size) : d_ptr(new DetailsPrivate)
{

    m_package = p;
    m_license = license;
    m_group = (Enum::Group)Util::enumFromString<Enum>(group, "Group", "Group");
    m_description = detail;
    m_url = url;
    m_size = size;
}

DetailsInfo::~DetailsInfo()
{
    delete d_ptr;
}

QSharedPointer<PackageInfo> DetailsInfo::package() const
{

    return m_package;
}

QString DetailsInfo::license() const
{

    return m_license;
}

PackageKit::Transaction::Group DetailsInfo::group() const
{

    return m_group;
}

QString DetailsInfo::description() const
{

    return m_description;
}

QString DetailsInfo::url() const
{

    return m_url;
}

qulonglong DetailsInfo::size() const
{

    return m_size;
}
