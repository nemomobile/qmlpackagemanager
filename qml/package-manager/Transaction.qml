import QtQuick 1.0
import "utils.js" as Utils
import MeeGo.Components 0.1

ModalDialog {
    id: dialog

    property bool showStatus: true

    property variant transaction
    property bool transactionSet : transaction != undefined

    property alias content: innerSpace.children
    height: 400

    content: [
        Item {
            id: stateItem
            property string state: transactionSet? transaction.state: ""
            onStateChanged:
                if (transactionSet && transaction.state == "error")
                    errorBox.show();
        },

        Column {
            anchors.fill:  parent

            Label {
                id: statusText
                width: parent.width
                text: transactionSet? Utils.statusText(transaction.status): "-"
                visible: showStatus
            }

            ProgressBar {
                id: progressBar
                anchors.left:  parent.left
                anchors.right:  parent.right
                anchors.margins: 20
                percentage:  transactionSet && transaction.state == "executing"? transaction.percentage: -1
                visible: percentage >= 0 && percentage <= 100
            }

            Row {
                Label {
                    id: packageLabel
                    font.pixelSize: theme.fontPixelSizeMedium
                    elideText: true
                    width: parent.parent.width * 0.4
                    text: transactionSet? transaction.currentPackage: ""
                    visible: transactionSet && transaction.currentPackage != undefined && transaction.currentPackage != ""
                }

                ProgressBar {
                    id: subprogressBar
                    anchors.verticalCenter: packageLabel.verticalCenter
                    height:  20
                    width: parent.parent.width * 0.55
                    percentage:  transactionSet && transaction.state == "executing"? transaction.subpercentage: -1
                    visible: percentage > 0 && percentage <= 100
                }
            }

            Rectangle {
                id: innerSpace
                width: parent.width
                Rectangle { id: defaultFill; height: 20 }
            }
        }]

     ModalMessageBox {
         id: errorBox

         text: transactionSet? transaction.errorText: ""

         title: "Transaction error"

         showAcceptButton: true
         showCancelButton: false

         fogClickable: false

         onAccepted:  { dialog.hide(); }
     }
}
