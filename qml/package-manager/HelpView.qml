import QtQuick 1.0
import MeeGo.Components 0.1

AppPageWithActionMenu {
    id: view

    Theme { id: theme }

    Flickable {
        anchors.fill:  parent
        contentHeight: helpText.height

        TextEdit {
            id: helpText
            width: parent.width
            readOnly: true
            wrapMode: "WordWrap"
            font.pixelSize: theme.fontPixelSizeLarge
            text:
                "
                <h2>Package Manager</h2>
                Package Manager is a simple application for managing software packages in the installation.
            It's built on top of the PackageKit library.
            <p>
            In the Page menu (the folder icon in the title bar), you can navigate between Packages, Repositories, and Help.
            <p>
            In the Action menu (the list icon in the title bar), you can find \"Exit\".

            <h3>Packages</h3>

            In the Packages main page, you can find buttons to managing installed software packages.

            <h4>Download and Install</h4>

            Select first the package group to browse. The list of available packages in the group is opened.
            <p>
            You can scroll through the list to find the package you want to install. You can also use
            the filter text field to limit the list of the shown packages. The filter applies to the
            name and summary (the short description) fields of the packages and it's case insensitive.
            You can reset the filter by clicking \”Clear\"
            <p>
            Click on a package you want to find more info about. The detail page is opened. If you want to
            install the package, check \"Install\" check box on the right. When you are finished, click
            \"back arrow\" in the upper left corner. Now you can see the green dot on the package to
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
            essentially the same is in installation described above. However, all packages are
            selected by default.

            <h4>Refresh Cache</h4>

            Refresh Cache reads the latest package catalogs from the repositories and ensures that the
            catalogs cached locally in the device are up-to-date.

            <h3>Repositories</h3>

            In the Repositories page, you can see the list of currently configured software repositories.
            You can enable and disable a repository by clicking the checkbox in the list.
            <p>
            <i>Note</i>: It's not possible to add or remove repositories in Package Manager because PackageKit
            does not support it. Use <i>zypper</i> command line utility in a terminal window instead (requires root priviledges):<pre>
            $ su
            password: meego
            $ zypper ar &lt;repo_url&gt;
            </pre>

            <h3>Other notes</h3>

            <h4>Performance</h4>

            Package Manager requires a substantial amount of free RAM. The main reason for the memory need is in
            the meego-ux-components and PackageKit libraries. For now, the best performance is achieved, if some of the constantly
            running processes are killed before launching Package Manager. For example, these processes
            are not essential for Package Manager: sysuid, meego-im-uiserver, meego-volume-control, pulseaudio, dialer.

            <h4>Error situations</h4>

            Package management is sensitive to many kinds of error situations. First of all, ensure that the
            network connection is functional when you try to install or update packages or refresh the cache.

            <p>
            <i>Signature verification for repostiory xyz failed</i>: This  occurs when the contents of a repository has
            changed since the last refresh. You can accept the new signature simply by refreshing the cache.
            "
        }
        }

}
