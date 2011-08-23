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
