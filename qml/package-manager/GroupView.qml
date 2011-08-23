import QtQuick 1.0
import "utils.js" as Utils
import com.nokia.meego 1.0

AppPageWithActionMenu {
    id: view

    signal groupSelected(string groupName)

    pageTitle: "Available Package Groups"

    property int buttonWidth: 200

    onGroupSelected: {
        selectedGroupName = groupName;
        window.pushPage("AvailableView");
    }

    Flickable {
        contentHeight: buttonGrid.height
        contentWidth: buttonGrid.width
        width: buttonGrid.width
        anchors.centerIn: view
        height: Math.min(buttonGrid.height, parent.height)

        Grid {
            id: buttonGrid
            anchors.horizontalCenter: parent.horizontalCenter
            columns: Math.min(view.width / buttonWidth, 3)
            spacing:  10

            Repeater {
                model: packageGroups

                Button {
                    height: 50
                    width: buttonWidth
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
