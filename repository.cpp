#include "repository.h"

Repository::Repository(QString id, QString description, bool isEnabled, QObject *parent) :
    QObject(parent),
    m_id(id),
    m_description(description),
    m_isEnabled(isEnabled),
    m_enableTransaction(0)
{
}

QString Repository::id() const
{
    return m_id;
}

QString Repository::description() const
{
    return m_description;
}

bool Repository::isEnabled() const
{
    return m_isEnabled;
}

bool Repository::isChanging() const
{
    return m_enableTransaction != 0;
}

void Repository::setEnabled(bool enable)
{
    qDebug() << Q_FUNC_INFO << description() << enable;
    if (m_enableTransaction || m_isEnabled == enable)
        return;

    m_enableTransaction = new PackageKit::Transaction(0, this);

    emit changed();

    connect(m_enableTransaction, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
            this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));

    m_enableTransaction->repoEnable(id(), enable);
}

void Repository::onFinished(PackageKit::Enum::Exit exitCode, uint duration)
{
    qDebug() << Q_FUNC_INFO << exitCode << duration;

    m_enableTransaction = 0;

    if (exitCode == PackageKit::Enum::ExitSuccess) {
        m_isEnabled = !m_isEnabled;
    }

    emit changed();
}


RepositoryList::RepositoryList(QObject *parent) :
    QObject(parent)
{
}

void RepositoryList::operator<<(Repository *repository)
{
    m_list << repository;
    emit changed();
}

QList<Repository*> *RepositoryList::list()
{
    return &m_list;
}

void RepositoryList::clear()
{
    foreach(QObject *object, m_list)
        object->deleteLater();

    m_list.clear();

    emit changed();
}
