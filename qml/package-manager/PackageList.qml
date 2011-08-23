import QtQuick 1.0
import com.nokia.meego 1.0

ListView {
    id:view

    property color markerColor

    spacing: 0

    signal showDetails
    signal showContextMenu(int x, int y)

    focus: true

    delegate: PackageListDelegate {
        id: delegate
        property variant packageData: model
        marker: Component {
            Rectangle {
                radius: 2
                color: view.markerColor
            }
        }

        onShowDetails: { view.showDetails() }

        onShowContextMenu: {
//            console.log("opencontextmenu " + (x + delegate.x) + " " + (y + delegate.y));
            view.showContextMenu(x + delegate.x, y + delegate.y);
        }
    }

    Item {
        property int count: view.model.totalcount
        onCountChanged:  {
            if (count == 0)
                currentIndex = 0
        }
    }
}
