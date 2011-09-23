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
import "utils.js" as Utils
import com.nokia.meego 1.0

AppPageWithActionMenu {
    id: mainview

    property int buttonWidth: 170
    property int buttonHeight: 170
    property int horizontalMargin: (width - 3 * buttonWidth) / 4

    pageTitle: "Packages"

    anchors.fill:  parent

    property bool actionTriggered: false

    function pushPage(view) {
        actionTriggered = true;
        window.pushPage(view);
        actionTriggered = false;
    }

    Grid {
        id: buttonRect

        columns: parent.width > 3 * buttonWidth + 2 * spacing? 3: 1
        spacing: 30

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -40

        Button {
            enabled: !actionTriggered
            text: "Download\n&\nInstall"
            height: buttonHeight
            width: buttonWidth
            onClicked: { pushPage("GroupView"); }
        }

        Button {
            enabled: !actionTriggered
            text: "View\n&\nUninstall"
            height: buttonHeight
            width: buttonWidth
            onClicked: { pushPage("InstalledView"); }
        }

        Button {
            enabled: !actionTriggered
            text: "Update"
            height: buttonHeight
            width: buttonWidth
            onClicked: { pushPage("UpdateView"); }
        }
    }

    Button {
        id: refreshCacheButton
        enabled: !actionTriggered
        text: "Refresh Cache"
        anchors.horizontalCenter: buttonRect.horizontalCenter
        width: Math.max(250, buttonRect.width)
        anchors.top:  buttonRect.bottom
        anchors.topMargin: 50

        onClicked: { actionTriggered = true; packageManager.refreshCache(); refreshTransaction.open(); actionTriggered = false; }
    }

    Transaction {
        id: refreshTransaction
        titleText: "Refreshing Cache"
        transaction: refreshCacheTransaction

        acceptButtonEnabled: state != "executing"
        rejectButtonEnabled: state == "executing" && transaction.allowCancel
    }
}
