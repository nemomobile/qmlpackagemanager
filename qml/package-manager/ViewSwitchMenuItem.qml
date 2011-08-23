import QtQuick 1.0
import com.nokia.meego 1.0

MenuItem {
    property string view
    // enabled: window.currentView != view
    platformStyle: MenuItemStyle { fontWeight: view != window.currentView? Font.Bold: Font.Normal }
    onClicked: {
        if (view != window.currentView)
            window.changeView(view);
    }
}
