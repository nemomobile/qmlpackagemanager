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

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include "transaction.h"


class Repository : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id NOTIFY changed)
    Q_PROPERTY(QString description READ description NOTIFY changed)
    Q_PROPERTY(bool isEnabled READ isEnabled WRITE setEnabled NOTIFY changed)
    Q_PROPERTY(bool isChanging READ isChanging NOTIFY changed)

public:
    explicit Repository(QString id = QString(), QString description = QString(), bool isEnabled = false, QObject *parent = 0);

    QString id() const;
    QString description() const;
    bool isEnabled() const;
    bool isChanging() const;

    void setId(const QString &id);
    void setDescription(const QString &description);
    void setIsEnabled(bool isEnabled);

signals:
    void changed();

public slots:
    void setEnabled(bool enabled);

private slots:
    void onFinished(PackageKit::Transaction::Exit status, uint runtime);

private:
    QString m_id;
    QString m_description;
    bool m_isEnabled;

    PackageKit::Transaction *m_enableTransaction;
};


class RepositoryList : public QObject
{
    Q_OBJECT
public:
    explicit RepositoryList(QObject *parent = 0);

    QList<Repository*> *list();

    void operator<<(Repository *repository);

signals:
    void changed();

public slots:
    void clear();

private:
    QList<Repository*> m_list;
};

#endif // REPOSITORY_H
