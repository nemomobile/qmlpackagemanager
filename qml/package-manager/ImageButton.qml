import QtQuick 1.1

Rectangle{
    property string btnText
    property string btnImage
    property string btnPage

    width: isPortrait?parent.parent.width-2*parent.spacing:(parent.parent.width-4*parent.spacing)/3
    height: isPortrait?(parent.parent.height-4*parent.spacing-80)/3:170
    color: "transparent"
    Column {
        width: parent.width
        height: parent.height
        Image {
            fillMode: Image.PreserveAspectFit
            width: parent.width
            height: parent.height - label.height
            id: name
            source: "../images/" + btnImage
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text{
            id: label
            anchors.horizontalCenter: parent.horizontalCenter
            text: btnText
            color: "#b3b3b3"
            font.pixelSize: 30
        }
    }
    MouseArea{
        anchors.fill: parent
        enabled: !actionTriggered
        onClicked: {pushPage(btnPage);}
    }
}
