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

import QtQuick 1.1
import "utils.js" as Utils
import "UIConstants.js" as UI
import com.nokia.meego 1.0

AppPageWithActionMenu {
    id: mainview

    property int buttonWidth: 200
    property int buttonHeight: 170
    property int horizontalMargin: (width - 3 * buttonWidth) / 4
    property int isPortrait: width < height

    pageTitle: "Packages"



    anchors.fill:  parent
    tools: mainViewTools

    property bool actionTriggered: false

    function pushPage(view) {
        actionTriggered = true;
        window.pushPage(view);
        actionTriggered = false;
    }
    Column {
        spacing: 20
        anchors.fill: parent

        Rectangle {
            width: parent.width
            height: 80


            color: "#ff6600"
            Text {
                anchors.left: parent.left
                anchors.leftMargin: 25
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: UI.FONT_XLARGE
                text: "Packages"
                color: "white"
            }
        }

        Grid {
            id: buttonRect

            columns: isPortrait? 1: 3
            spacing: 30

            width: parent.width - 2 * spacing
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.leftMargin: spacing
            anchors.rightMargin: spacing
//            anchors.verticalCenterOffset: -40

            ImageButton{
                btnText: "Download & Install";
                btnImage: "download.png"
                btnPage: "GroupView"
            }
            ImageButton{
                btnText: "View & Uninstall";
                btnImage: "uninstall.png"
                btnPage: "InstalledView"
                visible: isPortrait
            }
            ImageButton{
                btnText: "Update";
                btnImage: "update.png"
                btnPage: "UpdateView"
            }
            ImageButton{
                btnText: "View & Uninstall";
                btnImage: "uninstall.png"
                btnPage: "InstalledView"
                visible: !isPortrait
            }
            move: Transition {
                NumberAnimation {
                    properties: "x,y"
                    duration: 10
                }
            }

        }
    }

    Transaction {
        id: refreshTransaction
        titleText: "Refreshing Cache"
        transaction: refreshCacheTransaction

        acceptButtonEnabled: state != "executing"
        rejectButtonEnabled: state == "executing" && transaction.allowCancel
    }

    ToolBarLayout {
        id: mainViewTools
        visible: true
        ToolIcon {
            iconSource: "../images/help.png"
            onClicked: pushPage("HelpView")
        }
        ToolIcon{

            id: refreshCacheButton
            enabled: !actionTriggered
            platformIconId: "toolbar-refresh"

            onClicked: { actionTriggered = true; packageManager.refreshCache(); refreshTransaction.open(); actionTriggered = false; }
        }

        ToolIcon {
            platformIconId: "toolbar-view-menu"
//            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (mainMenu.status == DialogStatus.Closed) ? mainMenu.open() : mainMenu.close()
        }


    }

    Menu {
        id: mainMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTr("Repositories"); onClicked: pushPage("RepoListView")}
//            MenuItem { text: qsTr("Other") }
//            MenuItem { text: qsTr("Settings") }
        }
    }

}
