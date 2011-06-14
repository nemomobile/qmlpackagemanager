import QtQuick 1.0
import "utils.js" as Utils
import MeeGo.Components 0.1

AppPageWithActionMenu {
    id: view

    property variant listTransaction: refreshReposTransaction

    Component.onCompleted: {
        packageManager.refreshRepos();
    }

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

    Label {
        text: "Loading..."
        color: "darkgrey"
        anchors.horizontalCenter: repolist.horizontalCenter
        anchors.verticalCenter: repolist.verticalCenter
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

    Text {
        id: statusText
        color: "grey"
        visible: view.listTransaction != undefined && view.listTransaction.state != "success"
        anchors.bottom: parent.bottom
        text: Utils.transactionInfo(view.listTransaction)
    }

}
