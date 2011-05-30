#ifndef PACKAGEGROUP_H
#define PACKAGEGROUP_H

#include <QObject>
#include <QPackageKit>

class PackageGroup : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint group READ group NOTIFY changed)

public:
    PackageGroup(uint group = 0, QObject *parent = 0);

    uint group();

signals:
    void changed();

public slots:

private:
    uint m_group;
};


class PackageGroupList : public QObject
{
    Q_OBJECT
public:
    explicit PackageGroupList(QObject *parent = 0);

    QList<PackageGroup*> *list();

private:
    QList<PackageGroup*> m_list;
};
#endif // PACKAGEGROUP_H
