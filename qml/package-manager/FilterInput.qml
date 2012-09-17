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
    id: filterInput

    property alias text: input.text
    property int total
    property int filtered
    function focus() {
        input.forceActiveFocus();
    }

    signal changed

    height: childrenRect.height


    TextField {
        id: input
        text: ""
        placeholderText: "search"
        width: parent.width
        height: 50
        onTextChanged: filterInput.changed(); // on meego textChanged does not get emitted automatically
        Image {
                anchors { top: parent.top; right: parent.right; margins: 7 }
                id: clearText
                fillMode: Image.PreserveAspectFit
                smooth: true;
                visible: input.text
                source: "../images/clear.svg"
                height: parent.height - 14
                width: parent.height - 14
                MouseArea {
                    id: clear
                    anchors.fill: parent
                    onClicked: {
                        input.text = ""
                        filterInput.changed();
                    }
                }
            }
    }

}
