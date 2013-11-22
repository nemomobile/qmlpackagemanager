#include "packageinfo.h"
#include <QDebug>

PackageInfo::PackageInfo(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary, QObject *parent) :
    QObject(parent)
{
//    qDebug() << Q_FUNC_INFO;	
	
    m_info = info;
    m_packageID = packageID;
    m_summary = summary;
}

PackageInfo::PackageInfo(QSharedPointer<PackageInfo> pPackagInfo, QObject *parent) :
    QObject(parent)
{
    m_info = pPackagInfo.data()->info();
    m_packageID = pPackagInfo.data()->id();
    m_summary = pPackagInfo.data()->summary();
}


QString PackageInfo::id()
{
//    qDebug() << Q_FUNC_INFO;	
    return m_packageID;
}

QString PackageInfo::summary()
{
//    qDebug() << Q_FUNC_INFO;	
    return m_summary;
}

PackageKit::Transaction::Info PackageInfo::info()
{
//    qDebug() << Q_FUNC_INFO;	
    return m_info;
}
