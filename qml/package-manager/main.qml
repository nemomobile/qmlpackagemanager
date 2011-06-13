import QtQuick 1.0
import "utils.js" as Utils
import MeeGo.Components 0.1

Window {
    id: window

    anchors.centerIn: parent
    state: initialstate
    fullScreen: false

    bookMenuModel: [ "Packages", "Repositories", "Help" ]
    bookMenuPayload: [ mainView, repoView, helpView ]
    bookMenuTitle: "Package Manager"

    onActionMenuTriggered: { if (selectedItem == "exit") Qt.quit(); }

    Component.onCompleted: { switchBook(mainView); packageManager.refreshRepos();} //  packageManager.refreshUpdate(); }

    property string selectedGroupName

    Component { id: mainView
        MainView {
            pageTitle: "Packages"
            onInstall: { addPage(groupView); }
            onInstalledApps: { addPage(installedView); }
            onUpdate: { addPage(updateView); }
        }
    }

    Component { id: updateView
        PackageListView {
            pageTitle: "Available Updates"
            operationText: "Update"
            listModel: updateAvailablePackagesModel
            markerColor: "yellow"
            markerText: "u"
            listTransaction: getUpdatesTransaction
            operationTransaction: updatePackagesTransaction
            emptyListNote: "No updates available"

            onOperationRequested: packageManager.updateMarkedPackages(true, true)
            onOperationConfirmed: packageManager.updateMarkedPackages(false, true);
        }
    }

    Component { id: groupView
        GroupView {
            pageTitle: "Available Package Groups"
            onGroupSelected: {
                console.log(groupName);
                selectedGroupName = groupName;
                addPage(availableView);
            }
        }
    }

    Component { id: availableView
        PackageListView {
            pageTitle: "Available Packages in " + window.selectedGroupName
            operationText: "Install"
            listModel: availablePackagesModel
            markerColor: "green"
            markerText: "i"
            listTransaction: searchGroupsTransaction
            operationTransaction: installPackagesTransaction
            emptyListNote: "No packages available in " + window.selectedGroupName
            onOperationRequested: packageManager.installMarkedPackages(true, true)
            onOperationConfirmed: packageManager.installMarkedPackages(false, true);
        }
    }

    Component { id: installedView
        PackageListView {
            pageTitle: "Installed Packages"
            operationText: "Uninstall"
            listModel: installedPackagesModel
            markerColor: "#f45a5a"
            markerText: "u"
            listTransaction: getPackagesTransaction
            operationTransaction: uninstallPackagesTransaction
            emptyListNote: "No installed packagages"

            onOperationRequested: packageManager.uninstallMarkedPackages(true, true)
            onOperationConfirmed: packageManager.uninstallMarkedPackages(false, true);
        }
    }

    Component { id: repoView
        RepoListView {
            pageTitle: "Repositories"
        }
    }

    Component { id: helpView
        HelpView {
            pageTitle: "Packages Help"
        }
    }
}
