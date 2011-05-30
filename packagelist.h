#ifndef PACKAGELIST_H
#define PACKAGELIST_H

#include <QObject>
#include <QList>
#include <QPackageKit>

class Package;

class PackageList : public QObject
{
    Q_OBJECT
public:
    explicit PackageList(QObject *parent = 0);

    QList<Package*> *list();

    void operator<<(QSharedPointer<PackageKit::Package> packagePtr);

signals:
    void changed();

public slots:
    void clear();

private:
    QList<Package*> m_list;
};

#endif // PACKAGELIST_H
