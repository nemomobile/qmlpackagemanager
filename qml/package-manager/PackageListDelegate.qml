import QtQuick 1.0
import com.nokia.meego 1.0
import "UIConstants.js" as UI

Item {
    id: packageItem

    property variant pkg
    property bool isCurrent: ListView.isCurrentItem
    property variant marker: Component { Rectangle { height:15; width: 15; color: "blue"} }

    signal showDetails
    signal showContextMenu(int x, int y)

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.rightMargin: 20

    height: (indexText.height*2 + 4)

    Component.onCompleted: {
        pkg = packageObject;
    }

    function mark(set) {
        ListView.view.model.mark(index, set);
    }

    MouseArea {
        id: mouseArea
        anchors.fill:  parent
        onClicked: {
            if (packageItem.isCurrent)
                showDetails();
            else
                ListView.view.currentIndex = index
        }
        onPressAndHold: {
            ListView.view.currentIndex = index
//            console.log(mouse.x + " " + mouse.y);
            showContextMenu(mouse.x, mouse.y);
        }
    }

    Item {
        anchors.fill: parent
        anchors.margins: 2

        BorderImage {
            id: background
            border.top: 20
            border.bottom: 20
            border.right: 20
            border.left: 20
            anchors.fill: parent
            source: isCurrent?
                        mouseArea.pressed?
                            "image://theme/meegotouch-button-background-pressed":
                            "image://theme/meegotouch-list-background-selected" :
                        mouseArea.pressed?
                            "image://theme/meegotouch-list-background-pressed" :
                            "image://theme/meegotouch-list-background"
        }

        Rectangle {
            x: parent.width - 25
            anchors.verticalCenter: parent.verticalCenter
            width: 20
            height: 20
            Loader { id: loader; sourceComponent: marker; anchors.fill:  parent }
            visible: pkg.isMarked
        }

        Image {
            id: iconRect
            source: packageIcon? "image://icons/" + packageIcon: ""
            width: 32
            height: 32
            anchors.left:  indexText.right
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: indexText
            text: index + 1
            color: "darkgrey"
            width: visible? 80: 0
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: UI.FONT_LARGE
            horizontalAlignment: Text.AlignRight
            visible: parent.width > 500
        }

        Text {
            id: nameText
            text: pkg.displayName
            anchors.left: iconRect.right
            anchors.leftMargin: 10
            anchors.right: parent.right
            elide: Text.ElideRight
            font.pixelSize: UI.FONT_LARGE
            anchors.top: isCurrent? parent.top: indexText.top
        }

       Text {
            id: versionText
            text: pkg.name + " " + pkg.version
            elide: Text.ElideRight
            visible: isCurrent
            anchors.left: nameText.left
            anchors.right: nameText.right
            anchors.top: nameText.bottom
            anchors.topMargin: 5
            font.pixelSize: UI.FONT_LSMALL
        }
    }
}
