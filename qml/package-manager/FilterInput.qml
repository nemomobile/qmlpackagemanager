import QtQuick 1.0
import com.nokia.meego 1.0
import "UIConstants.js" as UI

Item {
    id: filterInput

    property alias text: input.text
    property int total
    property int filtered

    signal changed

    height: childrenRect.height

    Label {
        id: label
        anchors.left:  parent.left
        font.pixelSize: UI.FONT_LARGE
        text: "Find: "
    }

    TextField {
        id: input
        text: ""
        anchors.left: label.right
        anchors.right: clearButton.left
        height: label.height + 4
        onTextChanged: filterInput.changed(); // on meego textChanged does not get emitted automatically
    }


    Button {
        id: clearButton
        text: "Clear"
        height:  input.height
        enabled: input.text != ""
        width: 100
        anchors.right: parent.right
        onClicked: {
            input.text = "";
            filterInput.changed(); // on meego textChanged does not get emitted automatically
        }
    }

}
