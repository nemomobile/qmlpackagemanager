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
    id: view

    property string operationText
    property string preparationTitle
    property string executionTitle

    property variant listModel
    property color markerColor
    property variant listTransaction
    property variant operationTransaction

    signal operationRequested
    signal operationConfirmed

    property variant currentPackage: packageslist.currentItem? packageslist.currentItem.packageData: null
    property variant currentListDelegate: packageslist.currentItem

    property alias emptyListNote: emptyListLabel.text

    property alias filterInputVisible: searchIcon.checked



    goButtonLongLabel: operationText + " selected (" + packageslist.model.markedcount + ")"
    goButtonShortLabel: operationText + " (" + packageslist.model.markedcount +")"

    onReset: { packageslist.model.resetMarkings(); }
    onGo: {
        prepareTransaction.open();
        view.operationRequested();
    }

    property Style platformStyle: LabelStyle { }

    Item {
        id: payloadArea

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: filter.visible? filter.bottom: titleArea.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 5
    }

    Item {
        id: emptyListNotification
        visible: packageslist.model.totalcount == 0 && view.listTransaction != undefined && view.listTransaction.state == "success"
        anchors.fill: payloadArea
        Label {
            id: emptyListLabel
            width:  parent * 0.8
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Column {
        id: transactionErrorNotification
        visible: view.listTransaction != undefined && view.listTransaction.state == "error"
        anchors.fill: payloadArea
        anchors.margins: 20
        anchors.topMargin: 50
        spacing: 30
        Label {
            id: errorLabel
            anchors.horizontalCenter: parent.horizontalCenter
            text: Utils.errorCodeName(view.listTransaction != undefined? view.listTransaction.errorCode: undefined)
        }
        Text {
            width: parent.width
            font.pixelSize: UI.FONT_SMALL
            color: platformStyle.textColor
            wrapMode: Text.WordWrap
            text: view.listTransaction? view.listTransaction.errorText: ""
            horizontalAlignment: Text.AlignHCenter
        }
    }

    BusyIndicator {
        platformStyle: BusyIndicatorStyle { size: "large" }
        running: visible
        anchors.centerIn: packageslist
        visible: listTransaction != undefined && listTransaction.state == "executing"
    }

    PackageList {
        id: packageslist

        model: view.listModel
        markerColor: view.markerColor

        anchors.fill:  payloadArea

        onShowDetails: {
            pageStack.push(packageDetailsComponent)
        }

        onShowContextMenu: {
            menu.open();
        }

    }

    ContextMenu {
        id: menu
        platformTitle: Item {
            width: parent.width
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            height: menuTitleLabel.height + 30

            Label {
                id: menuTitleLabel
                text: view.currentPackage.packageObject.displayName
                elide: Text.ElideRight
            }
        }

        visualParent: view
        property bool isMarked: false

        onStatusChanged: {
            if (status == DialogStatus.Opening)
                isMarked = view.currentPackage.packageIsMarked;
        }

        MenuItem {
            text: menu.isMarked? "Unselect" : "Select for " + view.operationText
            onClicked: {
                menu.close();
                view.currentListDelegate.mark(!menu.isMarked);
            }
        }
    }

    Rectangle {
        id: titleArea
        height:  80
        width: parent.width
        color: "#ff6600"
        Row{
            anchors.fill: parent
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 25
            spacing: 20
            Text {
                text: view.pageTitle
                font.pixelSize: UI.FONT_XLARGE
                color: "white"
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                id: statsText
                font.pixelSize: UI.FONT_DEFAULT
                text: "(" + (filterInputVisible? listModel.filteredcount + "/": "") + listModel.totalcount + ")"
                color: "white"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        ToolButton {
            id: searchIcon
            checkable: true
            checked: false
            height:  48
            width: 48
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 30

            iconSource: "../images/icon-m-toolbar-search-white-selected.png"
            onCheckedChanged: { listModel.setFilterString(checked? filter.text: ""); filter.focus() }
            platformStyle: ButtonStyle {
                                    background: "../images/void.png"
                                    pressedBackground: "../images/void.png"
                                    checkedBackground: "../images/void.png"
                                }
        }

    }

    FilterInput {
        id: filter
        anchors.left: payloadArea.left
        anchors.right:  payloadArea.right
        anchors.top: titleArea.bottom
        anchors.topMargin: 10
        anchors.leftMargin:  40
        anchors.rightMargin: 40
        total: listModel.totalcount
        filtered: listModel.filteredcount
        onChanged: listModel.setFilterString(text);
        visible: !emptyListNotification.visible && !transactionErrorNotification.visible && filterInputVisible
        Component.onCompleted: { listModel.setFilterString(""); }
    }

    ScrollDecorator { flickableItem: packageslist; __minIndicatorSize: 25 }

    Component { id: packageDetailsComponent;  PackageDetails { } }

    Transaction {
        id: prepareTransaction
        titleText: view.preparationTitle
        transaction: view.operationTransaction
        acceptButtonText: "Confirm"
        acceptButtonEnabled: state == "success"
        height: parent.height * 0.8
        showStatus: false

        onAccepted: {
            commitTransaction.open();
            view.operationConfirmed();
        }

        content: ModifiedPackagesViewer { }
    }

    Transaction {
        id: commitTransaction
        titleText: view.executionTitle
        transaction: view.operationTransaction

        acceptButtonEnabled: state == "error" || state == "success"
        rejectButtonEnabled: state == "executing" && transaction.allowCancel

        onRejected: { filter.text = ""; }
        onAccepted: { filter.text = ""; }
    }

    tools: currentTools
    ToolBarLayout {
        id: currentTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: pageStack.pop()
        }

            ToolButton {
                id: resetButton;
                enabled: packageslist.model.markedcount > 0
                width: isLong? resetButtonLongWidth: resetButtonShortWidth;
                text: isLong? resetButtonLongLabel: resetButtonShortLabel;
                onClicked: { view.reset(); }
            }
            ToolButton {
                id: goButton;
                enabled: packageslist.model.markedcount > 0
                width: isLong? goButtonLongWidth: goButtonShortWidth;
                text: isLong? goButtonLongLabel: goButtonShortLabel;
                onClicked: { view.go(); }
                anchors.right: parent.right
                anchors.rightMargin: 20
            }

    }
}
