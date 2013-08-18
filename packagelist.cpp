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

#include "packagelist.h"

#include "package.h"

#include <QDebug>

PackageList::PackageList(QObject *parent) :
    QObject(parent)
{
}

void PackageList::operator<<(QSharedPointer<PackageInfo> packagePtr)
{
    m_list << new Package(packagePtr, false, this);
    emit changed();
}

QList<Package*> *PackageList::list()
{
    return &m_list;
}

void PackageList::clear()
{
    foreach(QObject *object, m_list)
        object->deleteLater();

    m_list.clear();

    emit changed();
}
