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
    id: packageItem

    property variant pkg
    property bool isCurrent: ListView.isCurrentItem
    property variant marker: Component { Rectangle { height:15; width: 15; color: "blue"} }

    signal showDetails
    signal showContextMenu(int x, int y)

    anchors.left: parent.left
    anchors.right: parent.right

    height: 70

    Component.onCompleted: {
        pkg = packageObject;
    }

    function mark(set) {
        ListView.view.model.mark(index, set);
        cbMarked.checked = set;
    }

    MouseArea {
        id: mouseArea
        anchors.fill:  parent
        onClicked: {
            view.currentIndex = index
            showDetails();
        }
        onPressAndHold: {
            view.currentIndex = index
//            console.log(mouse.x + " " + mouse.y);
            showContextMenu(mouse.x, mouse.y);
        }
    }

    Item {
        anchors.fill: parent
        Rectangle {
            anchors.fill: parent
            color: mouseArea.pressed?Qt.rgba(0,0,0,0.1):"transparent"

            Row{

                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 5

                spacing: 10
                Image {
                    id: iconRect
                    source: packageIcon? "image://icons/" + packageIcon: ""
                    width: 32
                    height: 32
                    anchors.verticalCenter: parent.verticalCenter
                }
                Column {
                    height: parent.height
                    width: parent.width - 32 - 40 - 2*parent.spacing - parent.anchors.leftMargin - parent.anchors.rightMargin
                    Text {
                        width: parent.width
                        id: nameText
                        text: pkg.displayName
                        elide: Text.ElideRight
                        font.pixelSize: UI.FONT_LARGE
                    }
                   Text {
                        width: parent.width
                        id: versionText
                        text: pkg.name + " " + pkg.version
                        elide: Text.ElideRight
                        color: "#777"
                        font.pixelSize: UI.FONT_LSMALL
                    }
                }
                CheckBox {
                    id: cbMarked
                    checked: pkg.isMarked
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: mark(checked);
                }
            }
        }
    }
}
