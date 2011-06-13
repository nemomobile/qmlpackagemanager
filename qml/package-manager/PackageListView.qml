import QtQuick 1.0
import "utils.js" as Utils
import MeeGo.Components 0.1

AppPageWithActionMenu {
    id: view

    property string operationText: ""
    property variant listModel
    property string markerColor
    property string markerText
    property variant listTransaction
    property variant operationTransaction

    signal operationRequested
    signal operationConfirmed

    property variant currentPackage: packageslist.currentItem? packageslist.currentItem.packageData: null
    property variant currentListDelegate: packageslist.currentItem

    property alias emptyListNote: emptyListLabel.text

    Rectangle {
        id: emptyListNotification
        visible: packageslist.model.totalcount == 0 && view.listTransaction != undefined && view.listTransaction.state == "success"
        anchors.fill: parent
        Label {
            id: emptyListLabel
            width:  500
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Label {
        text: "Loading..."
        color: "darkgrey"
        anchors.horizontalCenter: packageslist.horizontalCenter
        anchors.verticalCenter: packageslist.verticalCenter
        visible: listTransaction != undefined && listTransaction.state == "executing"
    }

    PackageList {
        id: packageslist

        model: view.listModel
        markerColor: view.markerColor
        markerText: view.markerText

        width: parent.width - 100
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: filter.bottom
        anchors.bottom: buttonArea.top
        anchors.topMargin: 10

        onShowDetails: {
            window.addPage(packageDetailsComponent)
        }
    }

    FilterInput {
        id: filter
        anchors.left: packageslist.left
        anchors.right: packageslist.right
        anchors.top: parent.top
        anchors.topMargin: 20
        total: listModel.totalcount
        filtered: listModel.filteredcount
        onChanged: packageslist.model.setFilterString(text);
        visible: !emptyListNotification.visible
        Component.onCompleted: { listModel.setFilterString(""); }
    }

    Rectangle {
        id: buttonArea
        visible:  !emptyListNotification.visible
        width: resetButton.width + goButton.width + resetButton.anchors.rightMargin
        anchors.right: packageslist.right
        anchors.bottom: parent.bottom
        height: 50

        Button {
            id: resetButton
            text: "Reset selection"
            enabled: packageslist.model.markedcount > 0
            height: goButton.height
            width: goButton.width
            anchors.right: goButton.left
            anchors.rightMargin: 20
            onClicked: { packageslist.model.resetMarkings(); }
        }

        Button {
            id: goButton
            text: view.operationText + " selected (" + packageslist.model.markedcount + ")"
            height: parent.height
            width: 300
            anchors.right: parent.right
            enabled: packageslist.model.markedcount > 0
            onClicked: {
                prepareTransaction.show();
                view.operationRequested();
            }
        }
    }

    Text {
        id: statusText
        color: "grey"
        visible: view.listTransaction != undefined && view.listTransaction.state != "success"
        anchors.bottom: parent.bottom
        text: Utils.transactionInfo(view.listTransaction)
    }


    Component { id: packageDetailsComponent;  PackageDetails { } }

    Transaction {
        id: prepareTransaction
        title: "Preparing operation " + view.operationText
        transaction: view.operationTransaction
        acceptButtonText: "Confirm"
        acceptButtonEnabled: transactionSet && transaction.state == "success"
        height: parent.height * 0.8
        showStatus: false

        onAccepted: {
            commitTransaction.show();
            view.operationConfirmed();
        }

        onRejected: {
        }

        content: ModifiedPackagesViewer { }
    }

    Transaction {
        id: commitTransaction
        title: "Executing operation " + view.operationText
        transaction: view.operationTransaction

        acceptButtonEnabled: transactionSet && (transaction.state == "error" || transaction.state == "success")
        cancelButtonEnabled: transactionSet && transaction.state == "executing" && transaction.allowCancel

        onRejected: {
            filter.text = "";
        }
        onAccepted: {
            filter.text = "";
        }

//        content:  Text { text:  view.operationTransaction != undefined && view.operationTransaction.state == "executing"? "Executing...": "Done" }
    }
}
