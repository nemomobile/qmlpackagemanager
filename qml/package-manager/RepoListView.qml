import QtQuick 1.0
import "utils.js" as Utils
import com.nokia.meego 1.0
import "UIConstants.js" as UI

AppPageWithActionMenu {
    id: view

    property variant listTransaction: refreshReposTransaction
    pageTitle: "Repositories"

    Component.onCompleted: {
        packageManager.refreshRepos();
    }

    property Style platformStyle: LabelStyle{}

    Rectangle {
        id: emptyListNotification
        visible: repolist.model.totalcount == 0 && view.listTransaction != undefined && view.listTransaction.state == "success"
        anchors.fill: parent
        Label {
            id: emptyListLabel
            text: "No repositories found"
            width:  500
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Column {
        id: transactionErrorNotification
        visible: view.listTransaction != undefined && view.listTransaction.state == "error"
        anchors.fill: parent
        anchors.margins: 20
        anchors.topMargin: 100
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
            wrapMode: "WordWrap"
            text: view.listTransaction? view.listTransaction.errorText: ""
            horizontalAlignment: Text.AlignHCenter
        }
    }

    BusyIndicator {
        platformStyle: BusyIndicatorStyle { size: "large" }
        running: visible
        anchors.centerIn: repolist
        visible: listTransaction != undefined && listTransaction.state == "executing"
    }

    ListView {
        id: repolist

        model: repositories

        width: parent.width - 100
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
        spacing: 0

        focus: true

        delegate: RepoListDelegate {
            id: delegate
            property variant repoData: model

        }
    }

}
