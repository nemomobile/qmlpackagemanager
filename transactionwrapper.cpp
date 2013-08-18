/*
 * This file is part of mg-package-manager
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: Kyösti Ranto <kyosti.ranto@digia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

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
            connect(m_transaction, SIGNAL(package(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)),
                    this, SLOT(onPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)));

        connect(m_transaction, SIGNAL(message(PackageKit::Transaction::Message type, const QString &message)),
                this, SLOT(onMessage(PackageKit::Transaction::Message type, const QString &message)));
        connect(m_transaction, SIGNAL(errorCode(PackageKit::Transaction::Error error, const QString &details)),
                this, SLOT(onErrorCode(PackageKit::Transaction::Error error, const QString &details)));
        connect(m_transaction, SIGNAL(repoSignatureRequired(const QString &packageID,
                                                            const QString &repoName,
                                                            const QString &keyUrl,
                                                            const QString &keyUserid,
                                                            const QString &keyId,
                                                            const QString &keyFingerprint,
                                                            const QString &keyTimestamp,
                                                            PackageKit::Transaction::SigType type)),
                this, SLOT(onRepoSignatureRequired(const QString &packageID,
                                                   const QString &repoName,
                                                   const QString &keyUrl,
                                                   const QString &keyUserid,
                                                   const QString &keyId,
                                                   const QString &keyFingerprint,
                                                   const QString &keyTimestamp,
                                                   PackageKit::Transaction::SigType type)));
        connect(m_transaction, SIGNAL(finished(PackageKit::Transaction::Exit status, uint runtime)),
                this, SLOT(onFinished(PackageKit::Transaction::Exit status, uint runtime)));
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

void TransactionWrapper::onPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
//    qDebug() << Q_FUNC_INFO << (*packagePtr).name();
    if (info.isNull())
        m_currentPackage = QString();
    else
        m_currentPackage = (*info).name();

    emit changed();
}

void TransactionWrapper::onMessage(PackageKit::Enum::Message type, const QString &message)
{
//    qDebug() << Q_FUNC_INFO << message;

    emit changed();
}

void TransactionWrapper::onErrorCode(PackageKit::Transaction::Error error, const QString &details)
{
//    qDebug() << Q_FUNC_INFO;
    m_errorCode = error;
    m_errorText = details;
    m_state = "error";

    emit changed();
}

void TransactionWrapper::onRepoSignatureRequired(const QString &packageID,
                                                 const QString &repoName,
                                                 const QString &keyUrl,
                                                 const QString &keyUserid,
                                                 const QString &keyId,
                                                 const QString &keyFingerprint,
                                                 const QString &keyTimestamp,
                                                 PackageKit::Transaction::SigType type)
{
//    qDebug() << Q_FUNC_INFO;
//    qDebug() << (*info.package).id();
//    qDebug() << info.keyId << info.keyUrl << info.keyFingerprint << info.type << info.repoId;
}

void TransactionWrapper::onFinished(PackageKit::Transaction::Exit status, uint runtime)
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
