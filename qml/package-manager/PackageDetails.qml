import QtQuick 1.0
import "utils.js" as Utils
import MeeGo.Components 0.1

AppPageWithActionMenu {
    id: packagedetails
    anchors.fill:  parent
    pageTitle: "Details of " + item.packageName

//    property alias operationText: markOpButton.text

    property variant item: view.currentPackage
    property variant delegate: view.currentListDelegate
    property variant pkg: item.packageObject

//    property alias selected: markOpButton.active
    property alias selected: markBox.isChecked

    signal close

    onItemChanged: {
        flick.contentY = 0;
        selected = item.packageIsMarked;
    }

    onDeactivating: {
        delegate.mark(selected);
    }

    Component.onCompleted: {
//        pkg = item.packageObject
//        spinner.show()
    }

    Flickable {
        id: flick
        anchors.fill: parent
        contentWidth:  width
        contentHeight:  textColumn.height

//        Spinner {
//            id: spinner
//        }

        Column {
            id: textColumn

            Text { height: 30; text: "package: " + (pkg? pkg.displayName: "") }

            TextData{ label: "status"; value:item?  item.packageDetailsAvailable + " "
                                              + item.packageUpdateDetailsAvailable
                                                   + " " + item.packageUpdateInfoAvailable: "" }

            Image {
                source: item != undefined && item.packageIcon? "image://icons/" + item.packageIcon: ""
                width: 32
                height: 32
            }

            TextData { label: "icon"; value: item != undefined? item.packageIcon: "" }
            TextData { label: "name"; value: item != undefined? item.packageName: "" }
            TextData { label: "version"; value: item != undefined? item.packageVersion: "" }
            TextData { label: "arch"; value: item != undefined? item.packageArch: "" }
            TextData { label: "data"; value: item != undefined? item.packageData: "" }
            TextData { label: "summary"; value: item != undefined? item.packageSummary: "" }
            TextData { label: "info"; value: item != undefined? Utils.infoText(item.packageInfo): "" }

            Text {
                height:  30
                visible:  item != undefined && item.packageDetailsAvailable == 1
                text: "Loading Details..."
            }

            Column {
                visible: item != undefined && item.packageDetailsAvailable == 2

                TextData { label: "license"; value: item != undefined? item.packageDetailsLicense: "" }
                TextData { label: "group"; value: item != undefined? Utils.groupName(item.packageDetailsGroup): "" }
                TextData { label: "description"; value: item != undefined? item.packageDetailsDescription: "" }
                TextData { label: "url"; value: item != undefined? item.packageDetailsUrl: "" }
                TextData { label: "size"; value: item != undefined? item.packageDetailsSize: "" }
            }

            TextData { label:  "is update available"; value: item != undefined? item.packageIsUpdateAvailable: "" }

            Column {
                visible: item != undefined && item.packageIsUpdateAvailable

                Image {
                    source: item != undefined? "image://icons/" + item.packageUpdateIcon: ""
                    width: 32
                    height: 32
                }

                TextData { label: "update icon"; value: item != undefined? item.packageUpdateIcon: "" }
                TextData { label: "update name"; value: item != undefined? item.packageUpdateName: "" }
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

    Theme { id: theme }

    Rectangle {
        anchors.fill:  markBoxArea
        anchors.margins: -15
        radius: 2
        color: "lightgrey"

        MouseArea {
            anchors.fill:  parent
            onClicked: markBox.isChecked = !markBox.isChecked
        }
    }

    Column {
        id: markBoxArea
        spacing: 8

        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 30

        Text {
            id: markText
            font.pixelSize: theme.fontPixelSizeLarger
            text: view.operationText
        }

        CheckBox {
            id: markBox
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

//    Rectangle {
//        id: buttonArea
//        color: "white"
//        anchors.right: parent.right
//        anchors.rightMargin: 50
//        anchors.bottom: parent.bottom
//        height: 50

//        Button {
//            id: markNoOpButton
//            text: "No operation"

//            height: markOpButton.height
//            width: markOpButton.width
//            anchors.right: markOpButton.left
//            anchors.rightMargin: 20

//            active: !markOpButton.active
//            onClicked: { markOpButton.active = false; }
//        }

//        Button {
//            id: markOpButton
//            text: view.operationText
//            height: parent.height
//            width: 300
//            anchors.right: parent.right
//            onClicked: { active = true; }
//        }
//    }

}
