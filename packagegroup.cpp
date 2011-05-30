#include "packagegroup.h"

#include <QPackageKit>
#include <QDebug>

PackageGroup::PackageGroup(uint group, QObject *parent) :
    QObject(parent),
    m_group(group)
{
}

uint PackageGroup::group()
{
    return m_group;
}


// ------------------------------------

PackageGroupList::PackageGroupList(QObject *parent) :
    QObject(parent)
{
    PackageKit::Enum::Groups groups = PackageKit::Client::instance()->groups();

    foreach(PackageKit::Enum::Group group, groups) {
        m_list << new PackageGroup(group);
    }
}

QList<PackageGroup *> *PackageGroupList::list()
{
    return &m_list;
}

