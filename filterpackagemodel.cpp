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

#include "filterpackagemodel.h"

#include "packagemodel.h"


FilterPackageModel::FilterPackageModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void FilterPackageModel::setSourceModel(QAbstractItemModel *sModel)
{
    PackageModel *packageModel = qobject_cast<PackageModel*>(sourceModel());

    if (packageModel) {
        disconnect(packageModel, SIGNAL(countChanged()), this, SIGNAL(totalCountChanged()));
        disconnect(packageModel, SIGNAL(markedCountChanged()), this, SIGNAL(markedCountChanged()));
        disconnect(packageModel, SIGNAL(countChanged()), this, SLOT(emitFilteredCountChanged()));
    }

    QSortFilterProxyModel::setSourceModel(sModel);

    packageModel = qobject_cast<PackageModel*>(sourceModel());

    if (packageModel) {
        connect(packageModel, SIGNAL(countChanged()), this, SIGNAL(totalCountChanged()));
        connect(packageModel, SIGNAL(markedCountChanged()), this, SIGNAL(markedCountChanged()));
        connect(packageModel, SIGNAL(countChanged()), this, SLOT(emitFilteredCountChanged()));
    }
}

void FilterPackageModel::mark(int row, bool set)
{
//    qDebug() << Q_FUNC_INFO << row << set;

    PackageModel *sModel = qobject_cast<PackageModel*>(sourceModel());
    sModel->mark(mapToSource(index(row,0)).row(), set);
}

void FilterPackageModel::resetMarkings()
{
    PackageModel *sModel = qobject_cast<PackageModel*>(sourceModel());
    sModel->resetMarkings();
}

void FilterPackageModel::setFilterString(const QString &text)
{
//    qDebug() << Q_FUNC_INFO << text;
    setFilterFixedString(text);
    emit filteredCountChanged();
}

int FilterPackageModel::filteredCount()
{
    return rowCount();
}

int FilterPackageModel::totalCount()
{
    return sourceModel()->rowCount();
}

int FilterPackageModel::markedCount()
{
    PackageModel *sModel = qobject_cast<PackageModel*>(sourceModel());
    sModel->markedCount();
}


void FilterPackageModel::emitFilteredCountChanged()
{
    QTimer::singleShot(50, this, SIGNAL(filteredCountChanged()));
}
