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

import QtQuick 2.0
import "utils.js" as Utils
import com.nokia.meego 2.0
import "UIConstants.js" as UI


Page {
    id: page

    property string pageTitle: ""
    tools: buttonTools

    signal deactivating
    signal go
    signal reset

    property bool isLong: page.width > 500

    property string resetButtonLongLabel: "Reset"
    property string resetButtonShortLabel: "Reset"
    property int resetButtonLongWidth: 150
    property int resetButtonShortWidth: 100
    property string goButtonLongLabel: "Go"
    property string goButtonShortLabel: "Go"
    property int goButtonLongWidth: 330
    property int goButtonShortWidth: 200

}
