import QtQuick 1.0
import "UIConstants.js" as UI
import com.nokia.meego 1.0

AppPageWithActionMenu {
    id: view

    property Style platformStyle: LabelStyle{}

    Flickable {
        id: flick
        anchors.fill:  parent
        anchors.margins: 20
        contentHeight: helpText.height

        TextEdit {
            id: helpText
            width: parent.width
            readOnly: true
            wrapMode: "WordWrap"
            font.pixelSize: UI.FONT_LARGE
            color: platformStyle.textColor

            text:
                "
                <h2>Package Manager</h2>
                Package Manager is an application for managing software packages in the installation.
            It's built on top of the PackageKit library.
            <p>
            In the application menu (the list icon in the tool bar), you can navigate between Packages, Repositories, and Help.
            You can also find \"Exit\" in the menu.

            <h3>Packages</h3>

            In the Packages main page, you can find buttons to managing installed software packages.

            <h4>Download and Install</h4>

            Select first the package group to browse. The list of available packages in the group is opened.
            <p>
            You can scroll through the list to find the package you want to install. You can also use
            the filter text field to limit the list of the shown packages. The filter text field is opened with
            the magnifying glass button in the top-right corner. The filter applies to the
            name and summary (the short description) fields of the packages and it's case insensitive.
            You can reset the filter by clicking \”Clear\"
            <p>
            Click on a package to find more info about it. The detail page is opened. If you want to
            install the package, check \"Install\" check box on the right. When you are finished, click
            \"back arrow\" in the lower left corner. Now you can see the green dot on the package to
            indicate that the package is selected to be installed.
            <p>
            When you have selected all packages to install, you can trigger the installation process by
            clicking \"Install selected\". Next, the list of modified packages is shown and confirmation is
            requested. You can still cancel the installation during execution if the back-end allows it
            and the \"Cancel\" button is enabled.
            <p>
            If you choose not to install the selected packages, you can reset the selection by clicking \"Reset selection\".

            <h4>View and Uninstall</h4>

            In this list, you can see all installed software packages. You can select packages for
            uninstallation and then trigger the uninstallation process. The procedure is essentially the
            same as in installation described above.

            <h4>Update</h4>

            In this list you can see all installed software packages that have an update available.
            You can select packages for update and then trigger the update process. The procedure is
            essentially the same is in installation described above. However, all packages available are
            selected for update by default.

            <h4>Refresh Cache</h4>

            Refresh Cache reads the latest package catalogs from the repositories and ensures that the
            catalogs cached locally in the device are up-to-date.

            <h3>Repositories</h3>

            In the Repositories page, you can see the list of currently configured software repositories.
            You can enable and disable a repository with the repository toggle button in the list.
            <p>
            <i>Note</i>: It's not possible to add or remove repositories in Package Manager because PackageKit
            does not support it. Instead, use <i>zypper</i> command line utility in a terminal window (requires
            root priviledges):<pre>
    $ su
    password: meego
    $ zypper ar &lt;repo_url&gt;
    </pre>

            <h3>Other notes</h3>

            <h4>Error situations</h4>

            Package management is sensitive to many kinds of error situations. First of all, ensure that the
            network connection is functional when you try to install or update packages or refresh the cache.

            "
        }

    }

    ScrollDecorator { flickableItem: flick }
}
