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

#ifndef FILTERPACKAGEMODEL_H
#define FILTERPACKAGEMODEL_H

#include <QSortFilterProxyModel>
class Package;

class FilterPackageModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(int totalcount READ totalCount NOTIFY totalCountChanged)
    Q_PROPERTY(int filteredcount READ filteredCount NOTIFY filteredCountChanged)
    Q_PROPERTY(int markedcount READ markedCount NOTIFY markedCountChanged)

public:
    explicit FilterPackageModel(QObject *parent = 0);

    int totalCount();
    int filteredCount();
    int markedCount();

    void setSourceModel(QAbstractItemModel *sourceModel);

    Q_INVOKABLE QString displayName(int row);
    Q_INVOKABLE QString name(int row);
    Q_INVOKABLE QString version(int row);
    Q_INVOKABLE Package *packageByRow(int row) const;

signals:
    void totalCountChanged();
    void filteredCountChanged();
    void markedCountChanged();

public slots:
    void mark(int row, bool set);
    void resetMarkings();
    void setFilterString(const QString &text);

private slots:
    void emitFilteredCountChanged();
};

#endif // FILTERPACKAGEMODEL_H
