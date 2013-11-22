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
import "utils.js" as Utils

Sheet {
    id: sheet

    acceptButtonText: "OK"
    rejectButtonText: "Cancel"

    property variant transaction
    property bool showStatus: true
    property bool transactionSet : transaction != undefined
    property string state: transactionSet? transaction.state: ""

    property bool acceptButtonEnabled: true
    property bool rejectButtonEnabled: true

    property alias titleText: titleLabel.text
    property alias content: contentArea.children

    onAcceptButtonEnabledChanged: {
        if (sheet.getButton("acceptButton")) sheet.getButton("acceptButton").enabled = acceptButtonEnabled
    }

    onRejectButtonEnabledChanged: {
        if (sheet.getButton("rejectButton")) sheet.getButton("rejectButton").enabled = rejectButtonEnabled
    }

    anchors.fill: parent

    title: BusyIndicator {
        anchors.centerIn: parent; visible: sheet.state == "executing"; running: visible
    }

    content: Item {
        anchors.fill: parent

        LabelStyle { id: labelStyle }

        Label {
            id: titleLabel
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 10
            elide: Text.ElideRight
            font.capitalization: Font.MixedCase
            font.pixelSize: UI.FONT_LARGE
            font.bold: true
        }

        Item {
            id: statusSpace
            anchors.top: titleLabel.bottom
            anchors.left:  parent.left
            anchors.right:  parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 10
            anchors.leftMargin: 30
        }

        Column {
            id: statusArea

            anchors.fill:  statusSpace
            anchors.topMargin: content.length > 0? 0: 100
            spacing: 20

            Column {
                id: errorDisplay
                width: parent.width
                visible: sheet.state == "error"

                Label {
                    width: parent.width
                    text: "Transaction Error"
                    font.pixelSize: UI.FONT_LARGE
                }

                Text {
                    width: parent.width
                    text: transactionSet? transaction.errorText: ""
                    wrapMode: Text.WordWrap
                    font.pixelSize: UI.FONT_LSMALL
                    color: labelStyle.textColor
                }
            }

            Column {
                id: progressArea
                anchors.left: parent.left
                anchors.right:  parent.right
                visible: !errorDisplay.visible
                spacing: 20

                Label {
                    id: statusText
                    anchors.left:  parent.left
                    anchors.right: parent.right
                    text: transactionSet? Utils.statusText(transaction.status): "-"
                    visible: showStatus
                }

                ProgressBar {
                    id: progressBar
                    anchors.left:  parent.left
                    anchors.right:  parent.right
                    minimumValue: 0
                    maximumValue: 100
                    value: sheet.state == "executing"? transaction.percentage: -1
                    visible: value > 0 && value <= 100
                }

                Column {
                    id: substatus
                    anchors.left:  parent.left
                    anchors.right:  parent.right
                    anchors.leftMargin: parent.width * 0.1
                    spacing: 10

                    Label {
                        id: packageLabel
                        width: parent.width
                        text: transactionSet? transaction.currentPackage: ""
                        elide: Text.ElideRight
                        visible: transactionSet && transaction.currentPackage != undefined && transaction.currentPackage != ""
                    }

                    ProgressBar {
                        id: subprogressBar
                        width: packageLabel.width
                        minimumValue: 0
                        maximumValue: 100
                        value: sheet.state == "executing"? transaction.subpercentage: -1
                        visible: value > 0 && value <= 100 && packageLabel.text != ""
                    }
                }
            }

            Item {
                id: contentArea
                anchors.left:  parent.left
                anchors.right: parent.right
                height: statusSpace.height - progressArea.bottom
            }
        }
    }

    onRejected: {
        if (transactionSet)
            transaction.cancel();
    }
}
