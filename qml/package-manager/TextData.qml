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

import QtQuick 2.0
import com.nokia.meego 2.0

Item {

    height: valueField.height
    width:  parent.width

    property alias label: labelField.text
    property alias value: valueField.setValue
    property alias indentation: valueField.x

    Text {
        id: labelField
        font.pixelSize: UI.FONT_LSMALL
        color: platformStyle.textColor
        x: 0
    }

    Text {
        id: valueField
        anchors.left: parent.left
        anchors.leftMargin: 220
        anchors.right: parent.right
        anchors.rightMargin: 10
        // width: parent.width - x - 10
        property variant setValue
        text: (setValue != undefined? setValue: "-")
        elide: Text.ElideRight
        font.pixelSize: UI.FONT_LSMALL
        color: platformStyle.textColor
        horizontalAlignment: Text.AlignLeft
        wrapMode: Text.WordWrap
    }

}
