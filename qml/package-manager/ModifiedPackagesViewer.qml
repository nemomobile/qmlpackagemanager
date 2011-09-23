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

import QtQuick 1.0
import com.nokia.meego 1.0

Flickable {
    id: flick
    contentHeight:  listColumn.height
    anchors.fill:  parent

    Column {
        id: listColumn
        spacing: 10

        ModifiedPackagesList {
            id: toInstalls
            title: "Packages to be installed"
            model: packagesToBeInstalled
        }

        ModifiedPackagesList {
            id: toUpdates
            title: "Packages to be updated"
            model: packagesToBeUpdated
        }

        ModifiedPackagesList {
            id:toRemove
            title: "Packages to be removed"
            model: packagesToBeRemoved
        }
    }
}
