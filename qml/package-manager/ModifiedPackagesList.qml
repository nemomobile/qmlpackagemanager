import QtQuick 1.0

Column {

    property string title
    property alias model: repeater.model

    Text {
        id: titleText
        text: title + " (" + repeater.model.length + ")"
        font.bold:  true
        visible: repeater.model.length > 0
    }

    Column {
        Repeater {
            id: repeater
            Text  {
                text: model.modelData.name + " " + model.modelData.version
            }
        }
    }

}
