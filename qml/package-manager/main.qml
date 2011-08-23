import QtQuick 1.0
import "utils.js" as Utils
import com.nokia.meego 1.0

PageStackWindow {
    id: window

    platformStyle: PageStackWindowStyle { id: defaultStyle }

    anchors.centerIn: parent
    //state: initialstate

    Component.onCompleted: {
        // screen.allowedOrientations = Screen.Landscape | Screen.Portrait;
    }

    property string selectedGroupName

    property string currentView: "MainView"

    function changeView(view) {
        var component = loadComponent(view + ".qml");
        if (component != null) {
            pageStack.clear();
            pageStack.push(component);
            currentView = view;
        }
    }

    function pushPage(view) {
        var component = loadComponent(view + ".qml");
        if (component != null) {
            pageStack.push(component);
        }
    }

    function loadComponent(file) {
        var component = Qt.createComponent(file)

        if (component.status == Component.Ready)
            return component;
        else
            console.log("Error loading component:", component.errorString());
        return null;
    }

    initialPage: MainView { }

}
