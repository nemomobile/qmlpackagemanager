import QtQuick 1.0

PackageListView {
    pageTitle: (width > 500? "Available Packages in ": "") + window.selectedGroupName
    operationText: "Install"
    preparationTitle: "Confirm Installation"
    executionTitle: "Installing Packages"
    listModel: availablePackagesModel
    markerColor: "green"
    listTransaction: searchGroupsTransaction
    operationTransaction: installPackagesTransaction
    emptyListNote: "No packages available in " + window.selectedGroupName
    onOperationRequested: packageManager.installMarkedPackages(true, true)
    onOperationConfirmed: packageManager.installMarkedPackages(false, true);
}
