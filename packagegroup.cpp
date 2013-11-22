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

#include "packagegroup.h"
#include "packageinfo.h"
//#include <QPackageKit>
#include <QDebug>

//#define PACKAGEGROUP_LOG		


PackageGroup::PackageGroup(uint group, QObject *parent) :
    QObject(parent),
    m_group(group)
{
#ifdef PACKAGEGROUP_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif	
}

uint PackageGroup::group()
{
#ifdef PACKAGEGROUP_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif
    return m_group;
}


// ------------------------------------

PackageGroupList::PackageGroupList(QObject *parent) :
    QObject(parent)
{
#ifdef PACKAGEGROUP_LOG	
    qDebug() << Q_FUNC_INFO;	
#endif

    int i;
    for (i=0;i<PackageKit::Transaction::GroupNewest+1;i++) {
        PackageGroup *p = new PackageGroup((PackageKit::Transaction::Group)i);
        m_list << p;
    }
}

QList<PackageGroup *> *PackageGroupList::list()
{
#ifdef PACKAGEGROUP_LOG	
    qDebug() << Q_FUNC_INFO  << m_list.size();	
#endif
    return &m_list;
}

