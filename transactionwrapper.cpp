/*
 * This file is part of mg-package-manager
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * Copyright (C) 2013 Timo Hannukkala <timo.hannukkala@nomovok.com>
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

// #define TRANSACTIONWRAPPER_LOG

TransactionWrapper::TransactionWrapper(PackageKit::Transaction *transaction, bool readPackages, QObject *parent) :
    QObject(parent),
    m_transaction(transaction),
    m_errorCode(PackageKit::Transaction::ErrorUnknown)
{
//#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO;
//#endif

    if (m_transaction) {
        connect(m_transaction, SIGNAL(changed()), this, SLOT(onChanged()));

        if (readPackages)
            connect(m_transaction, SIGNAL(package(PackageKit::Transaction::Info, const QString &, const QString &)),
                    this, SLOT(onPackage(PackageKit::Transaction::Info, const QString &, const QString &)));

        connect(m_transaction, SIGNAL(message(PackageKit::Transaction::Message, const QString &)),
                this, SLOT(onMessage(PackageKit::Transaction::Message, const QString &)));
        connect(m_transaction, SIGNAL(errorCode(PackageKit::Transaction::Error, const QString &)),
                this, SLOT(onErrorCode(PackageKit::Transaction::Error, const QString &)));
        connect(m_transaction, SIGNAL(repoSignatureRequired(const QString &,
                                                            const QString &,
                                                            const QString &,
                                                            const QString &,
                                                            const QString &,
                                                            const QString &,
                                                            const QString &,
                                                            PackageKit::Transaction::SigType)),
                this, SLOT(onRepoSignatureRequired(const QString &,
                                                   const QString &,
                                                   const QString &,
                                                   const QString &,
                                                   const QString &,
                                                   const QString &,
                                                   const QString &,
                                                   PackageKit::Transaction::SigType)));
        connect(m_transaction, SIGNAL(finished(PackageKit::Transaction::Exit, uint)),
                this, SLOT(onFinished(PackageKit::Transaction::Exit, uint)));
        connect(m_transaction, SIGNAL(destroyed()), this, SLOT(onDestroyed()));
    }


    m_state = "initialized";
    m_currentPackage = QString();
}

void TransactionWrapper::cancel()
{
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif
    if (m_transaction) {
        m_transaction->cancel(); // not sure if this really cancels the transaction...
    }
}

void TransactionWrapper::onChanged()
{
    PackageKit::Transaction *t = m_transaction;
//    qDebug() << Q_FUNC_INFO << t->status() << t->percentage() << t->subpercentage() << t->elapsedTime() << t->remainingTime() << t->speed() << t->allowCancel() << t->error() << t->role() << t->succeeded() << t->tid();
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif

    if (m_state == "initialized")
        m_state = "executing";

    emit changed();
}

void TransactionWrapper::onPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif
	
	
//    qDebug() << Q_FUNC_INFO << (*packagePtr).name();
/*    if (info.isNull()) {
        m_currentPackage = QString();
    }
    else { */
    m_currentPackage = packageID; // (*info).name();
  //  }

    emit changed();
}

void TransactionWrapper::onMessage(PackageKit::Transaction::Message type, const QString &message)
{
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO << message;
#endif

    emit changed();
}

void TransactionWrapper::onErrorCode(PackageKit::Transaction::Error error, const QString &details)
{
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO << error <<  details;
#endif
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
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif
//    qDebug() << (*info.package).id();
//    qDebug() << info.keyId << info.keyUrl << info.keyFingerprint << info.type << info.repoId;
}

void TransactionWrapper::onFinished(PackageKit::Transaction::Exit status, uint runtime)
{
#ifdef TRANSACTIONWRAPPER_LOG	
	    qDebug() << Q_FUNC_INFO;	
#endif	

//    qDebug() << Q_FUNC_INFO << status << runtime;

    if (status == PackageKit::Transaction::ExitSuccess)
        m_state = "success";

    emit changed();
}

void TransactionWrapper::onDestroyed()
{
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif
    m_transaction = 0;
}

void TransactionWrapper::setState(QString state)
{
#ifdef TRANSACTIONWRAPPER_LOG	
	    qDebug() << Q_FUNC_INFO;	
#endif	

    m_state = state;
    emit changed();
}

QString TransactionWrapper::state()
{
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif
    return m_state;
}

uint TransactionWrapper::errorCode()
{
#ifdef TRANSACTIONWRAPPER_LOG	
qDebug() << Q_FUNC_INFO;	
#endif

    return m_errorCode;
}

void TransactionWrapper::setErrorText(const QString &error)
{
#ifdef TRANSACTIONWRAPPER_LOG	
qDebug() << Q_FUNC_INFO;	
#endif

    m_errorText = error;
    emit changed();
}

QString TransactionWrapper::errorText()
{
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO << m_errorText;
#endif
    return m_errorText;
}

bool TransactionWrapper::allowCancel()
{
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif
    return m_transaction? m_transaction->allowCancel(): false;
}

int TransactionWrapper::percentage()
{
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif
    return m_transaction? m_transaction->percentage(): 101;
}

int TransactionWrapper::subpercentage()
{
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO;
#endif
    return 101;
}

uint TransactionWrapper::status()
{
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif
	
    return m_transaction? m_transaction->status(): PackageKit::Transaction::StatusUnknown;
}

QString TransactionWrapper::currentPackage()
{
#ifdef TRANSACTIONWRAPPER_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif
	
    return m_transaction && m_state == "executing" ? m_currentPackage: QString();
}
