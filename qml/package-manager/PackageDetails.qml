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
import "UIConstants.js" as UI

AppPageWithActionMenu {
    id: packagedetails
    anchors.fill:  parent
    pageTitle: "Details of " + item.packageName

    property variant item: view.currentPackage
    property variant delegate: view.currentListDelegate
    property variant pkg: item.packageObject

    property alias selected: markBox.checked

    signal close

    onItemChanged: {
        flick.contentY = 0;
        selected = item.packageIsMarked;

    }

//    onDeactivating: {
//        delegate.mark(selected);
//    }

    property Style platformStyle: LabelStyle { }

    Flickable {
        id: flick
        anchors.fill: parent
        anchors.margins: 20
        contentWidth:  width
        contentHeight:  textColumn.y + textColumn.height

        Item {
            id: headerRow
            anchors.left:  parent.left
            anchors.right: parent.right
            height: childrenRect.height

            Image {
                id: icon
                source: item != undefined && item.packageIcon? "image://icons/" + item.packageIcon: ""
                visible: source != ""
                width: 48
                height: 48
            }

            Text {
                height: icon.height
                font.pixelSize: UI.FONT_XLARGE
                color: platformStyle.textColor
                text: (pkg? pkg.displayName: "")
                elide: Text.ElideRight
                anchors.left:  icon.right
                anchors.leftMargin: 10
                anchors.right:  parent.right
            }
        }

        Column {
            id: textColumn
            width: parent.width
            anchors.top: headerRow.bottom
            anchors.topMargin: 10

//            TextData{ label: "status"; value:item?  item.packageDetailsAvailable + " "
//                                                   + item.packageUpdateDetailsAvailable
//                                                   + " " + item.packageUpdateInfoAvailable: "" }

            TextData { label: "name"; value: item != undefined? item.packageName: "" }
//            TextData { label: "icon"; value: item != undefined? item.packageIcon: "" }
            TextData { label: "version"; value: item != undefined? item.packageVersion: "" }
            TextData { label: "arch"; value: item != undefined? item.packageArch: "" }
            TextData { label: "data"; value: item != undefined? item.packageData: "" }
            TextData { label: "summary"; value: item != undefined? item.packageSummary: "" }
            TextData { label: "info"; value: item != undefined? Utils.infoText(item.packageInfo): "" }

            Text {
                height:  30
                font.pixelSize: UI.FONT_LARGE
                visible:  item != undefined && item.packageDetailsAvailable == 1
                text: "Loading Details..."
            }

            Column {
                visible: item != undefined && item.packageDetailsAvailable == 2
                width: parent.width

                TextData { label: "license"; value: item != undefined? item.packageDetailsLicense: "" }
                TextData { label: "group"; value: item != undefined? Utils.groupName(item.packageDetailsGroup): "" }
                TextData { label: "description"; value: item != undefined? item.packageDetailsDescription: "" }
                TextData { label: "url"; value: item != undefined? item.packageDetailsUrl: "" }
                TextData { label: "size"; value: item != undefined? item.packageDetailsSize: "" }
            }

            Column {
                visible: item != undefined && item.packageIsUpdateAvailable
                width: parent.width

                Image {
                    source: item != undefined? "image://icons/" + item.packageUpdateIcon: ""
                    width: 32
                    height: 32
                }

                TextData { label: "update name"; value: item != undefined? item.packageUpdateName: "" }
//                TextData { label: "update icon"; value: item != undefined? item.packageUpdateIcon: "" }
                TextData { label: "update version"; value: item != undefined? item.packageUpdateVersion: "" }
                TextData { label: "update arch"; value: item != undefined? item.packageUpdateArch: "" }
                TextData { label: "update data"; value: item != undefined? item.packageUpdateData: "" }
                TextData { label: "update summary"; value: item != undefined? item.packageUpdateSummary: "" }
                TextData { label: "update info"; value: item != undefined? Utils.infoText(item.packageUpdateInfo): "" }

                Text {
                    height:  30
                    visible:  item != undefined && item.packageUpdateDetailsAvailable == 1
                    text: "Loading Update Details..."
                }

                Column {
                    visible: item != undefined && item.packageUpdateDetailsAvailable == 2
                    width: parent.width
                    TextData { label: "update license"; value: item != undefined? item.packageUpdateDetailsLicense: "" }
                    TextData { label: "update group"; value: item != undefined? Utils.groupName(item.packageUpdateDetailsGroup): "" }
                    TextData { label: "update description"; value: item != undefined? item.packageUpdateDetailsDescription: "" }
                    TextData { label: "update url"; value: item != undefined? item.packageUpdateDetailsUrl: "" }
                    TextData { label: "update size"; value: item != undefined? item.packageUpdateDetailsSize: "" }
                }

                Text {
                    height:  30
                    visible:  item != undefined && item.packageUpdateInfoAvailable == 1
                    text: "Loading Update Info..."
                }

                Column {
                    visible: item != undefined && item.packageUpdateInfoAvailable == 2
                    width: parent.width

                    TextData { label: "update text"; value: item != undefined? item.packageUpdateText: "" }
                    TextData { label: "update change log"; value: item != undefined? item.packageUpdateChangeLog: "" }
                    TextData { label: "update vendor url"; value: item != undefined? item.packageUpdateVendorUrl: "" }
                    TextData { label: "update bugzilla url"; value: item != undefined? item.packageUpdateBugzillaUrl: "" }
                    TextData { label: "update cve url"; value: item != undefined? item.packageUpdateCveUrl: "" }
                    TextData { label: "update state"; value: item != undefined? Utils.updateStateName(item.packageUpdateState): "" }
                    TextData { label: "update issued"; value: item != undefined? item.packageUpdateIssued: "" }
                    TextData { label: "update updated"; value: item != undefined? item.packageUpdateUpdated: "" }
                    TextData { label: "update needs restart"; value: item != undefined? Utils.needsRestartText(item.packageUpdateRestart): "" }
                }
            }
        }
    }

    ScrollDecorator { flickableItem: flick }

    tools: currentTools
    ToolBarLayout {
        id: currentTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: pageStack.pop()
        }
        CheckBox {
            id: markBox;
            text: view.operationText;
            anchors.right: parent.right;
            anchors.rightMargin: 20
            onCheckedChanged: delegate.mark(selected);
        }
        ToolButton {
            visible: false
        }
    }
}
