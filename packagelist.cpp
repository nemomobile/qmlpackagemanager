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

#include "packagelist.h"

#include "package.h"

#include <QDebug>


// #define PACKAGELIST_LOG	

PackageList::PackageList(QObject *parent) :
    QObject(parent)
{
    qDebug() << Q_FUNC_INFO;	
	
}

void PackageList::operator<<(QSharedPointer<PackageInfo> packagePtr)
{
#ifdef PACKAGELIST_LOG		
    qDebug() << Q_FUNC_INFO;	
#endif
    m_list << new Package(packagePtr, false, this);
    emit changed();
}

Package* PackageList::get(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
#ifdef PACKAGELIST_LOG		
    qDebug() << Q_FUNC_INFO;	
#endif
    int i;

    for (i=0;i<m_list.size();i++) {
        Package *packagePtr = m_list[i];

        if (packagePtr->equals(info, packageID, summary)) {
            return packagePtr;
        }
    }

    return NULL;
}

QList<Package*> *PackageList::list()
{
#ifdef PACKAGELIST_LOG		
    qDebug() << Q_FUNC_INFO;	
#endif
    return &m_list;
}

void PackageList::clear()
{
#ifdef PACKAGELIST_LOG		
    qDebug() << Q_FUNC_INFO;	
#endif
    foreach(QObject *object, m_list)
        object->deleteLater();

    m_list.clear();

    emit changed();
}
