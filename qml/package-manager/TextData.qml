import QtQuick 1.0
import MeeGo.Components 0.1

Item {

    height: valueField.height
    width: parent.width
    x: parent.x

    property alias label: labelField.text
    property alias value: valueField.setValue

    Theme  {id: theme}

    Text {
        id: labelField
        font.pixelSize: theme.fontPixelSizeLarge
        x: 0
    }
    Text {
        id: valueField
        x: 150
        width: 400
        //anchors.right: parent.right
        property variant setValue
        text: (setValue != undefined? setValue: "-")
        font.pixelSize: theme.fontPixelSizeLarge
        horizontalAlignment: Text.AlignLeft
        wrapMode: Text.WordWrap
    }
}
