#include "transactionwrapper.h"

#include <QDebug>

TransactionWrapper::TransactionWrapper(PackageKit::Transaction *transaction, bool readPackages, QObject *parent) :
    QObject(parent),
    m_transaction(transaction),
    m_errorCode(PackageKit::Enum::UnknownError)
{
//    qDebug() << Q_FUNC_INFO;

    if (m_transaction) {
        connect(m_transaction, SIGNAL(changed()), this, SLOT(onChanged()));

        if (readPackages)
            connect(m_transaction, SIGNAL(package(QSharedPointer<PackageKit::Package>)),
                    this, SLOT(onPackage(QSharedPointer<PackageKit::Package>)));

        connect(m_transaction, SIGNAL(message(PackageKit::Enum::Message,QString)),
                this, SLOT(onMessage(PackageKit::Enum::Message,QString)));
        connect(m_transaction, SIGNAL(errorCode(PackageKit::Enum::Error,QString)),
                this, SLOT(onErrorCode(PackageKit::Enum::Error,QString)));
        connect(m_transaction, SIGNAL(repoSignatureRequired(PackageKit::Client::SignatureInfo)),
                this, SLOT(onRepoSignatureRequired(PackageKit::Client::SignatureInfo)));
        connect(m_transaction, SIGNAL(finished(PackageKit::Enum::Exit,uint)),
                this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));
        connect(m_transaction, SIGNAL(destroyed()), this, SLOT(onDestroyed()));
    }

    m_state = "initialized";
    m_currentPackage = QString();
}

void TransactionWrapper::cancel()
{
//    qDebug() << Q_FUNC_INFO;
    if (m_transaction) {
        m_transaction->cancel(); // not sure if this really cancels the transaction...
    }
}

void TransactionWrapper::onChanged()
{
    PackageKit::Transaction *t = m_transaction;
//    qDebug() << Q_FUNC_INFO << t->status() << t->percentage() << t->subpercentage() << t->elapsedTime() << t->remainingTime() << t->speed() << t->allowCancel() << t->error() << t->role() << t->succeeded() << t->tid();

    if (m_state == "initialized")
        m_state = "executing";

    emit changed();
}

void TransactionWrapper::onPackage(QSharedPointer<PackageKit::Package> packagePtr)
{
//    qDebug() << Q_FUNC_INFO << (*packagePtr).name();
    if (packagePtr.isNull())
        m_currentPackage = QString();
    else
        m_currentPackage = (*packagePtr).name();

    emit changed();
}

void TransactionWrapper::onMessage(PackageKit::Enum::Message type, const QString &message)
{
//    qDebug() << Q_FUNC_INFO << message;

    emit changed();
}

void TransactionWrapper::onErrorCode(PackageKit::Enum::Error error, const QString &details)
{
//    qDebug() << Q_FUNC_INFO;
    m_errorCode = error;
    m_errorText = details;
    m_state = "error";

    emit changed();
}

void TransactionWrapper::onRepoSignatureRequired(PackageKit::Client::SignatureInfo info)
{
//    qDebug() << Q_FUNC_INFO;
//    qDebug() << (*info.package).id();
//    qDebug() << info.keyId << info.keyUrl << info.keyFingerprint << info.type << info.repoId;
}

void TransactionWrapper::onFinished(PackageKit::Enum::Exit status, uint runtime)
{
//    qDebug() << Q_FUNC_INFO << status << runtime;

    if (status == PackageKit::Enum::ExitSuccess)
        m_state = "success";

    emit changed();
}

void TransactionWrapper::onDestroyed()
{
//    qDebug() << Q_FUNC_INFO;
    m_transaction = 0;
}

void TransactionWrapper::setState(QString state)
{
    m_state = state;
    emit changed();
}

QString TransactionWrapper::state()
{
//    qDebug() << Q_FUNC_INFO;
    return m_state;
}

uint TransactionWrapper::errorCode()
{
    return m_errorCode;
}

void TransactionWrapper::setErrorText(const QString &error)
{
    m_errorText = error;
    emit changed();
}

QString TransactionWrapper::errorText()
{
//    qDebug() << Q_FUNC_INFO;
    return m_errorText;
}

bool TransactionWrapper::allowCancel()
{
//    qDebug() << Q_FUNC_INFO;
    return m_transaction? m_transaction->allowCancel(): false;
}

int TransactionWrapper::percentage()
{
//    qDebug() << Q_FUNC_INFO;
    return m_transaction? m_transaction->percentage(): 101;
}

int TransactionWrapper::subpercentage()
{
//    qDebug() << Q_FUNC_INFO;
    return m_transaction? m_transaction->subpercentage(): 101;
}

uint TransactionWrapper::status()
{
    return m_transaction? m_transaction->status(): PackageKit::Enum::UnknownStatus;
}

QString TransactionWrapper::currentPackage()
{
    return m_transaction && m_state == "executing" ? m_currentPackage: QString();
}
