import QtQuick 1.0
import MeeGo.Components 0.1

ListView {
    id:view

    model: installedPackagesModel
    property string markerColor: "blue"
    property string markerText: "."

    spacing: 0

    signal showDetails

    focus: true

    delegate: PackageListDelegate {
        id: delegate
        property variant packageData: model
        marker: Component {
            Rectangle {
                radius: 2
                color: markerColor
//                Text { text: markerText; anchors.fill: parent; horizontalAlignment: "AlignHCenter"}
            }
        }

        onShowDetails: { view.showDetails() }
    }

}
