import QtQuick 1.0
import "UIConstants.js" as UI
import com.nokia.meego 1.0

Column {

    property string title
    property alias model: repeater.model

    property Style platformStyle: LabelStyle { }

    Text {
        id: titleText
        text: title + " (" + repeater.model.length + ")"
        font.bold:  true
        font.pixelSize: UI.FONT_DEFAULT
        color: platformStyle.textColor
        visible: repeater.model.length > 0
    }

    Column {
        Repeater {
            id: repeater
            Text  {
                text: model.modelData.name + " " + model.modelData.version
                font.pixelSize: UI.FONT_LSMALL
                color: platformStyle.textColor
            }
        }
    }

}
