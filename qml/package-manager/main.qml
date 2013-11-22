/*
 * This file is part of mg-package-manager
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * Copyright (C) 2013 Timo Hannukkala <timo.hannukkala@nomovok.com>
 *
 * Contact: Kyösti Ranto <kyosti.ranto@digia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

import QtQuick 2.0
import com.nokia.meego 2.0

PageStackWindow {
    id: window

//    platformStyle: PageStackWindowStyle { id: defaultStyle }

  //  anchors.fill: parent
    state: initialstate

    Component.onCompleted: {
        packageManager.refreshAll();
    }

    property string selectedGroupName

    property string currentView: "MainView"

    function changeView(view) {
    console.log("function changeView(view)")
    console.log(view)
        var component = loadComponent(view + ".qml");
        if (component != null) {
            pageStack.clear();
            pageStack.push(component);
            currentView = view;
        }
    }

    function pushPage(view) {
    console.log("function changeView(view)")
    console.log(view)
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
