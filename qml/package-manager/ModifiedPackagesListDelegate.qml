/*
 * This file is part of mg-package-manager
 *
 * Copyright (C) 2013 Nomovok.
 *
 * Contact: Timo Hannukkala <timo.hannukkala@nomovok.com>
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

Item {
    id: packageItem
    property QtObject pkgs: null

   // Item {
   //     anchors.fill: parent
        Text {
           // width: parent.width
            id: versionText
            text: "" // pkgs.name(index) + " - " + pkgs.version(index)
        }
 //   }

}