import QtQuick 1.0
import MeeGo.Components 0.1

Item {
    id: packageItem

    property variant pkg
    property bool isCurrent: ListView.isCurrentItem
    property variant marker: Component { Rectangle { height:15; width: 15; color: "blue"}}

    signal showDetails

    anchors.left: parent.left
    anchors.right:  parent.right
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
    }

    Theme { id: theme }

    Item {
        anchors.fill: parent
        anchors.margins: 2

        Rectangle {
            anchors.fill: parent
            radius: 4
            // the colors should be fetched from theme. BorderImages are too slow.
            color: isCurrent? (mouseArea.pressed? "steelblue": "lightsteelblue"): "white"
            border.width: 1
        }

        Rectangle {
            x: parent.width - 20
            y: 4
            width: 15
            height: 15
            Loader { id: loader; sourceComponent: marker; anchors.fill:  parent }
            visible: pkg.isMarked
        }

        Image {
            source: packageIcon? "image://icons/" + packageIcon: ""
            width: 32
            height: 32
            x: 40
            y: 2
        }

        Text {
            id: indexText
            text: index + 1
            x: 0
            color: "darkgrey"
            width: 35
            anchors.left: parent.left
            anchors.top: parent.top
            font.pixelSize: theme.fontPixelSizeLarge
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: nameText
            text: pkg.displayName
            x: 80
            font.pixelSize: theme.fontPixelSizeLarge
            anchors.top: parent.top
        }

       Text {
            id: versionText
            text: pkg.name + " " + pkg.version
            visible: isCurrent
            x: 80
            font.pixelSize: theme.fontPixelSizeMedium
            y: indexText.height + 2
        }

    }
}
