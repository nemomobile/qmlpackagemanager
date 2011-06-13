import QtQuick 1.0
import MeeGo.Components 0.1

Item {
    id: repoItem

    property bool isCurrent: ListView.isCurrentItem

    anchors.left: parent.left
    anchors.right:  parent.right
    height: (indexText.height*2 + 4)

    Theme { id: theme }

    Item {
        anchors.fill: parent
        anchors.margins: 2

        Rectangle {
            anchors.fill: parent
            radius: 4
            color: "white"
            border.width: 1
        }

        Text {
            id: indexText
            text: index + 1
            x: 0
            color: "darkgrey"
            width: 35
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: theme.fontPixelSizeLarge
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: descriptionText
            text: description
            font.pixelSize: theme.fontPixelSizeLarge
            anchors.verticalCenter: parent.verticalCenter
            anchors.left:  indexText.right
            anchors.leftMargin: 30
            anchors.right: enabledCheckBoxArea.left
            horizontalAlignment: "AlignLeft"
        }

        ToggleButton {
            id: enabledToggle
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 20
            on: isEnabled
            onLabel: qsTr("Enabled")
            offLabel: qsTr("Disabled")
            onToggled: {
                isEnabled = on
            }
        }
    }
}
