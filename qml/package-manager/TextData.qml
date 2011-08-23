import QtQuick 1.0
import "UIConstants.js" as UI
import com.nokia.meego 1.0

Item {

    height: valueField.height
    width:  parent.width

    property alias label: labelField.text
    property alias value: valueField.setValue
    property alias indentation: valueField.x

    property Style platformStyle: LabelStyle{}

    Text {
        id: labelField
        font.pixelSize: UI.FONT_LSMALL
        color: platformStyle.textColor
        x: 0
    }

    Text {
        id: valueField
        anchors.left: parent.left
        anchors.leftMargin: 220
        anchors.right: parent.right
        anchors.rightMargin: 10
        // width: parent.width - x - 10
        property variant setValue
        text: (setValue != undefined? setValue: "-")
        elide: Text.ElideRight
        font.pixelSize: UI.FONT_LSMALL
        color: platformStyle.textColor
        horizontalAlignment: Text.AlignLeft
        wrapMode: Text.WordWrap
    }

}
