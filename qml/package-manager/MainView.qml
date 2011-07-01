import QtQuick 1.0
import "utils.js" as Utils
import MeeGo.Components 0.1


AppPageWithActionMenu {
    id: mainview

    signal update
    signal install
    signal installedApps

    property int buttonWidth: 170
    property int buttonHeight: 170
    property int horizontalMargin: (parent.width - 3 * buttonWidth) / 4

    anchors.fill:  parent

    property bool actionTriggered: false

    function actionFinished() {
        actionTriggered = false;
    }

    Item {
        id: buttonRect

        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 2 * horizontalMargin
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -40
        height: buttonHeight

        Button {
            id: installButton
            enabled: !actionTriggered
            text: "Download\n&\nInstall"
            anchors.left:  parent.left
            height: buttonHeight
            width: buttonWidth
            onClicked: { actionTriggered = true; install(); }
        }

        Button {
            id:installedAppsButton
            enabled: !actionTriggered
            text: "View\n&\nUninstall"
            anchors.horizontalCenter: parent.horizontalCenter
            height: buttonHeight
            width: buttonWidth
            onClicked: { actionTriggered = true; installedApps(); }
        }

        Button {
            id: updateButton
            enabled: !actionTriggered
            text: "Update"
            anchors.right: parent.right
            height: buttonHeight
            width: buttonWidth
            onClicked: { actionTriggered = true; update(); }
        }
    }

    Button {
        id: refreshCacheButton
        enabled: !actionTriggered
        text: "Refresh Cache"
        anchors.left:  buttonRect.left
        anchors.right: buttonRect.right
        anchors.top:  buttonRect.bottom
        anchors.topMargin: 50

        onClicked: { actionTriggered = true; packageManager.refreshCache(); refreshCacheTransactionDialog.show(); actionTriggered = false; }
    }

    Transaction {
        id: refreshCacheTransactionDialog
        title: "Refreshing Cache"
        transaction: refreshCacheTransaction

        acceptButtonText: "OK"
        acceptButtonEnabled: transactionSet && transaction.state != "executing"
        cancelButtonEnabled: transactionSet
                             && (transaction.state == "executing" && transaction.allowCancel)
    }

    /*
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
    */
}
