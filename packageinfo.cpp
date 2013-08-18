#include "packageinfo.h"

PackageInfo::PackageInfo(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary, QObject *parent) :
    QObject(parent)
{
    m_info = info;
    m_packageID = packageID;
    m_summary = summary;
}

QString PackageInfo::id()
{
    return m_packageID;
}

QString PackageInfo::summary()
{
    return m_summary
}

PackageKit::Transaction::Info PackageInfo::info()
{
    return m_info;
}
