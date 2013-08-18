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

#include "iconprovider.h"

#include <QIcon>
#include <QFileInfo>
#include <QDebug>

IconProvider::IconProvider() :
    QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap IconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
//    qDebug() << Q_FUNC_INFO << id << requestedSize;

    if (size)
        *size = QSize(32, 32);

    if (id.isEmpty())
        return QPixmap(1,1);

    QSize targetSize = requestedSize.isValid()? requestedSize: QSize(32, 32);

    QIcon icon = QIcon::fromTheme(id);
    if (icon.isNull())
        icon = QIcon(id);

    if (!icon.isNull()) {
        return icon.pixmap(targetSize);
    }

    return QPixmap(1,1);
}
