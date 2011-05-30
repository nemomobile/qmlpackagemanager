#ifndef PACKAGEMARKINGS_H
#define PACKAGEMARKINGS_H

#include "package.h"

#include <QObject>
#include <QList>

class PackageMarkings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY changed)

public:
    explicit PackageMarkings(QObject *parent = 0);

    void mark(Package *package, bool set);
    void resetMarkings();

    int count();

    QList<Package*> packagesMarked();

signals:
    void changed();

public slots:

private:
    QList<Package*> m_marked;
};

#endif // PACKAGEMARKINGS_H
