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

#include "packagegroup.h"

#include <QPackageKit>
#include <QDebug>

PackageGroup::PackageGroup(uint group, QObject *parent) :
    QObject(parent),
    m_group(group)
{
}

uint PackageGroup::group()
{
    return m_group;
}


// ------------------------------------

PackageGroupList::PackageGroupList(QObject *parent) :
    QObject(parent)
{
    PackageKit::Enum::Groups groups = PackageKit::Client::instance()->groups();

    foreach(PackageKit::Enum::Group group, groups) {
        m_list << new PackageGroup(group);
    }
}

QList<PackageGroup *> *PackageGroupList::list()
{
    return &m_list;
}

