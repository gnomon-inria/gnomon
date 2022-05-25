// Panel.qml ---
//
// Filename: Panel.qml
// Description:
// Author: Romain TETLEY
// Maintainer:
// Created: ven. mai 13 07:53:20 2022 (+0200)
// Version:
// Package-Requires: ()
// Last-Updated:
//           By:
//     Update #: 0
// URL:
// Doc URL:
// Keywords:
// Compatibility:
//
//

// Commentary:
// This control is a panel in a gutter with emboss. Useful for making slick
// interfaces
//
//
//

// Change Log:
//
//
//
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <https://www.gnu.org/licenses/>.
//
//

// Code:


//Qt
import QtQuick
import QtQuick.Controls

//Gnomon
import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {

    id: _control

    default property alias content: _panel.children

    //this comes in handy to size a panel wrt to the
    //its children, or any other thing that requires
    //to access the actual panel
    property Control panel: _panel;

    //padding: 2
    implicitWidth: G.Style.smallPanelWidth;
    implicitHeight: G.Style.smallPanelHeight;
    clip: true;

    background: Rectangle {
        radius: G.Style.panelRadius;
        color: G.Style.colors.gutterColor;
    }

    Control {

        id: _panel

        anchors.fill: parent;
        clip: true;

        leftInset: 2
        rightInset: 2
        topInset: 2
        bottomInset: 2


        background: Rectangle {
            color: G.Style.colors.bgColor;
            border.width: 0.5;
            border.color: G.Style.colors.embossColor;
            radius: G.Style.panelRadius;
        }

    }

}

//
// Panel.qml ends here
