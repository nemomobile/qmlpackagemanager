import QtQuick 1.0
import MeeGo.Components 0.1

Rectangle {
    id: filterInput
    height: label.height

    property alias text: input.text
    property int total
    property int filtered

    signal changed

    Label {
        id: label
        x:10
        text: "Filter: "
        anchors.left:  parent.left
    }

    TextEntry {
        id: input
        text: ""
        anchors.left: label.right
        anchors.right: statsText.left
        height: label.height
        onTextChanged: filterInput.changed(); // on meego textChanged does not get emitted automatically
    }

    Label {
        id: statsText
        anchors.right: clearButton.left
        width: 200
        height: label.height
        text: "(" + (input.text != ""? filtered + "/": "") + total + ")"
    }

    Button {
        id: clearButton
        text: "clear"
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
