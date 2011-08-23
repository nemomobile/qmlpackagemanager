import QtQuick 1.0
import "utils.js" as Utils
import com.nokia.meego 1.0
import "UIConstants.js" as UI


Page {
    id: page

    property string pageTitle: ""
    tools: buttonTools

    signal deactivating
    signal go
    signal reset

    property bool toolButtonsVisible
    property alias resetButtonEnabled: resetButton.enabled
    property alias goButtonEnabled: goButton.enabled
    property alias backButtonEnabled: backButton.enabled

    property bool isLong: page.width > 500

    property string resetButtonLongLabel: "Reset"
    property string resetButtonShortLabel: "Reset"
    property int resetButtonLongWidth: 150
    property int resetButtonShortWidth: 100
    property string goButtonLongLabel: "Go"
    property string goButtonShortLabel: "Go"
    property int goButtonLongWidth: 330
    property int goButtonShortWidth: 200

//    Label {
//        id: titleLabel
//        text: pageTitle
//        font.pixelSize: UI.FONT_LARGE
//        anchors.top: parent.top
//    }

    ToolBarLayout {
        id: buttonTools

        ToolIcon { id: backButton; visible: pageStack != null && pageStack.depth > 1; iconId: "toolbar-back"; onClicked: { actionMenu.close(); deactivating(); pageStack.pop(); }  }
        ToolButtonRow {
            id: toolButtonRow
            ToolButton {
                id: resetButton;
                width: isLong? resetButtonLongWidth: resetButtonShortWidth;
                text: isLong? resetButtonLongLabel: resetButtonShortLabel;
                visible: toolButtonsVisible;
                onClicked: { page.reset(); }
            }
            ToolButton {
                id: goButton;
                width: isLong? goButtonLongWidth: goButtonShortWidth;
                text: isLong? goButtonLongLabel: goButtonShortLabel;
                visible: toolButtonsVisible;
                onClicked: { page.go(); }
            }
        }
        ToolIcon { iconId: "toolbar-view-menu" ; onClicked: { actionMenu.open(); } }
    }

    Menu {
        id: actionMenu
        visualParent: pageStack

        MenuLayout {
            ViewSwitchMenuItem { view: "MainView"; text: "Packages"; }
            ViewSwitchMenuItem { view: "RepoListView"; text: "Repositories"; }
            ViewSwitchMenuItem { view: "HelpView"; text: "Help"; }

            MenuItem { text: "Exit"; onClicked: { Qt.quit() } }
        }
    }
}
