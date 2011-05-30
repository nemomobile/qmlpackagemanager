#include "packagelist.h"

#include "package.h"

#include <QDebug>

PackageList::PackageList(QObject *parent) :
    QObject(parent)
{
}

void PackageList::operator<<(QSharedPointer<PackageKit::Package> packagePtr)
{
    m_list << new Package(packagePtr, false, this);
    emit changed();
}

QList<Package*> *PackageList::list()
{
    return &m_list;
}

void PackageList::clear()
{
    foreach(QObject *object, m_list)
        object->deleteLater();

    m_list.clear();

    emit changed();
}
