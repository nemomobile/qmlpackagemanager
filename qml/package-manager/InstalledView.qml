import QtQuick 1.0

PackageListView {
    pageTitle: "Installed" + (width > 500? " Packages": "")
    operationText: "Uninstall"
    preparationTitle: "Confirm Uninstallation"
    executionTitle: "Uninstalling Packages"
    listModel: installedPackagesModel
    markerColor: "#f45a5a"
    listTransaction: getPackagesTransaction
    operationTransaction: uninstallPackagesTransaction
    emptyListNote: "No installed packagages"

    onOperationRequested: packageManager.uninstallMarkedPackages(true, true)
    onOperationConfirmed: packageManager.uninstallMarkedPackages(false, true);
}
