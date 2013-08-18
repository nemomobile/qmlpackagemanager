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

#ifndef PACKAGEGROUP_H
#define PACKAGEGROUP_H

#include <QObject>


class PackageGroup : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint group READ group NOTIFY changed)

public:
    PackageGroup(uint group = 0, QObject *parent = 0);

    uint group();

signals:
    void changed();

public slots:

private:
    uint m_group;
};


class PackageGroupList : public QObject
{
    Q_OBJECT
public:
    explicit PackageGroupList(QObject *parent = 0);

    QList<PackageGroup*> *list();

private:
    QList<PackageGroup*> m_list;
};
#endif // PACKAGEGROUP_H
