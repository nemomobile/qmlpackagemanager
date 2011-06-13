#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include <QPackageKit>

class Repository : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id NOTIFY changed)
    Q_PROPERTY(QString description READ description NOTIFY changed)
    Q_PROPERTY(bool isEnabled READ isEnabled NOTIFY changed)

public:
    explicit Repository(QString id = QString(), QString description = QString(), bool isEnabled = false, QObject *parent = 0);

    QString id() const;
    QString description() const;
    bool isEnabled() const;

    void setId(const QString &id);
    void setDescription(const QString &description);
    void setIsEnabled(bool isEnabled);

signals:
    void changed();

public slots:
    void setEnabled(bool enabled);

private slots:
    void onFinished(PackageKit::Enum::Exit exitCode, uint duration);

private:
    QString m_id;
    QString m_description;
    bool m_isEnabled;

    PackageKit::Transaction *m_enableTransaction;
};


class RepositoryList : public QObject
{
    Q_OBJECT
public:
    explicit RepositoryList(QObject *parent = 0);

    QList<Repository*> *list();

    void operator<<(Repository *repository);

signals:
    void changed();

public slots:
    void clear();

private:
    QList<Repository*> m_list;
};

#endif // REPOSITORY_H
