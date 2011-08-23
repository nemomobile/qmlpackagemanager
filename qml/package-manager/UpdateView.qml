import QtQuick 1.0

PackageListView {
    pageTitle: "Available Updates"
    operationText: "Update"
    preparationTitle: "Confirm Update"
    executionTitle: "Updating Packages"
    listModel: updateAvailablePackagesModel
    markerColor: "yellow"
    listTransaction: getUpdatesTransaction
    operationTransaction: updatePackagesTransaction
    emptyListNote: "No updates available"

    onOperationRequested: packageManager.updateMarkedPackages(true, true)
    onOperationConfirmed: packageManager.updateMarkedPackages(false, true);
}
