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
//    qDebug() << Q_FUNC_INFO << description() << enable;
    if (m_enableTransaction || m_isEnabled == enable)
        return;

    m_enableTransaction = new PackageKit::Transaction(0, this);

    emit changed();

    connect(m_enableTransaction, SIGNAL(finished(PackageKit::Transaction::Exit status, uint runtime)),
            this, SLOT(onFinished(PackageKit::Enum::Exit,uint)));

    m_enableTransaction->repoEnable(id(), enable);
}

void Repository::onFinished(PackageKit::Transaction::Exit status, uint runtime)
{
//    qDebug() << Q_FUNC_INFO << exitCode << duration;

    m_enableTransaction = 0;

    if (status == PackageKit::Enum::ExitSuccess) {
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
