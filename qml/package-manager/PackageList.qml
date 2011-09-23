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

ListView {
    id:view

    property color markerColor

    spacing: 0

    signal showDetails
    signal showContextMenu(int x, int y)

    focus: true

    delegate: PackageListDelegate {
        id: delegate
        property variant packageData: model
        marker: Component {
            Rectangle {
                radius: 2
                color: view.markerColor
            }
        }

        onShowDetails: { view.showDetails() }

        onShowContextMenu: {
//            console.log("opencontextmenu " + (x + delegate.x) + " " + (y + delegate.y));
            view.showContextMenu(x + delegate.x, y + delegate.y);
        }
    }

    Item {
        property int count: view.model.totalcount
        onCountChanged:  {
            if (count == 0)
                currentIndex = 0
        }
    }
}
