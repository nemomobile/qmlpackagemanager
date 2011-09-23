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
import "UIConstants.js" as UI

Item {
    id: repoItem

    property bool isCurrent: ListView.isCurrentItem

    anchors.left: parent.left
    anchors.right:  parent.right
    height: (indexText.height*2 + 4)

    Item {
        anchors.fill: parent
        anchors.margins: 2

        BorderImage {
            id: background
            border.top: 20
            border.bottom: 20
            border.right: 20
            border.left: 20
            anchors.fill: parent
            source: "image://theme/meegotouch-list-background"
        }

        Text {
            id: indexText
            text: index + 1
            x: 0
            color: "darkgrey"
            width: 35
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: UI.FONT_LARGE
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: descriptionText
            text: description
            font.pixelSize: UI.FONT_DEFAULT
            anchors.verticalCenter: parent.verticalCenter
            anchors.left:  indexText.right
            anchors.leftMargin: 30
            anchors.right: enabledToggle.left
            elide:  Text.ElideRight
            horizontalAlignment: "AlignLeft"
        }

        Switch {
            id: enabledToggle
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 20
            checked: isEnabled
            onCheckedChanged: {
                isEnabled = checked
            }
        }

    }
}
