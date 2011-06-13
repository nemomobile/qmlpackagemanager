import QtQuick 1.0
import "utils.js" as Utils
import MeeGo.Components 0.1

AppPageWithActionMenu {

    signal groupSelected(string groupName)

    Flickable {
        contentHeight: buttonGrid.height
        contentWidth: buttonGrid.width
        width: buttonGrid.width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        height: Math.min(buttonGrid.height, parent.height)

        Grid {
            id: buttonGrid
            columns:  3
            spacing:  10

            Repeater {
                model: packageGroups

                Button {
                    height: 50
                    width: 200
                    text: Utils.groupName(modelData.group)
                    onClicked: {
                        groupSelected(text);
                        packageManager.refreshAvailable(modelData.group);
                    }
                }
            }
        }
    }

}
