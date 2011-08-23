import QtQuick 1.0
import com.nokia.meego 1.0

Flickable {
    id: flick
    contentHeight:  listColumn.height
    anchors.fill:  parent

    Column {
        id: listColumn
        spacing: 10

        ModifiedPackagesList {
            id: toInstalls
            title: "Packages to be installed"
            model: packagesToBeInstalled
        }

        ModifiedPackagesList {
            id: toUpdates
            title: "Packages to be updated"
            model: packagesToBeUpdated
        }

        ModifiedPackagesList {
            id:toRemove
            title: "Packages to be removed"
            model: packagesToBeRemoved
        }
    }
}
