import QtQuick 1.0
import "utils.js" as Utils
import MeeGo.Components 0.1


AppPage {
    id: mainview

    signal update
    signal install
    signal installedApps

    property int buttonWidth: 170
    property int buttonHeight: 170
    property int horizontalMargin: (parent.width - 3 * buttonWidth) / 4

    actionMenuModel: ["Exit"]
    actionMenuPayload: ["exit"]
    actionMenuTitle: ["Actions"]

    anchors.fill:  parent

    Item {
        id: buttonRect

        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 2 * horizontalMargin
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -40
        height: buttonHeight

        Button {
            id: installButton
            text: "Download\n&\nInstall"
            anchors.left:  parent.left
            height: buttonHeight
            width: buttonWidth
            onClicked: install()
        }

        Button {
            id:installedAppsButton
            text: "View\n&\nUninstall"
            anchors.horizontalCenter: parent.horizontalCenter
            height: buttonHeight
            width: buttonWidth
            onClicked:  installedApps()
        }

        Button {
            id: updateButton
            text: "Update"
            anchors.right: parent.right
            height: buttonHeight
            width: buttonWidth
            onClicked: update()
        }
    }

    Button {
        id: refreshCacheButton
        text: "Refresh Cache"
        anchors.left:  buttonRect.left
        anchors.right: buttonRect.right
        anchors.top:  buttonRect.bottom
        anchors.topMargin: 50

        onClicked: { packageManager.refreshCache(); refreshCacheTransactionDialog.show(); }
    }

    Transaction {
        id: refreshCacheTransactionDialog
        title: "Refreshing Cache"
        transaction: refreshCacheTransaction

        acceptButtonText: "OK"
        acceptButtonEnabled: transactionSet && transaction.state != "executing"
        cancelButtonEnabled: transactionSet
                             && (transaction.state == "executing" && transaction.allowCancel
                                 || transaction.state == "error")

    }

    Column {
        anchors.top:  parent.top
        Text {
            color: "grey"
            text: getUpdatesTransaction? ("getUpdatesTransaction: " + Utils.transactionInfo(getUpdatesTransaction)): ""
            visible: getUpdatesTransaction != undefined
        }

        Text {
            color: "grey"
            text: getPackagesTransaction? ("getPackagesTransaction: " + Utils.transactionInfo(getPackagesTransaction)): ""
            visible: getPackagesTransaction != undefined
        }
    }
}
