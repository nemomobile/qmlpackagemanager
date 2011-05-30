#include "packagemarkings.h"

PackageMarkings::PackageMarkings(QObject *parent) :
    QObject(parent)
{
}

void PackageMarkings::mark(Package *package, bool set)
{
//    qDebug() << Q_FUNC_INFO << package->name() << set;

    package->mark(set);

    if (set) {
        if (!m_marked.contains(package)) {
            m_marked.append(package);
            emit changed();
        }
    } else {
        if (m_marked.removeAll(package))
            emit changed();
    }
}

void PackageMarkings::resetMarkings()
{
//    qDebug() << Q_FUNC_INFO;

    if (m_marked.count() == 0)
        return;

    foreach (Package *package, m_marked) {
        package->mark(false);
    }

    m_marked.clear();

    emit changed();
}

int PackageMarkings::count()
{
    return m_marked.count();
}

QList<Package*> PackageMarkings::packagesMarked()
{
    return m_marked;
}
