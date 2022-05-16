// ToolBar.qml ---
//
// Filename: ToolBar.qml
// Description:
// Author: Romain Tetley
// Maintainer:
// Created: lun. mai 16 14:15:43 2022 (+0200)
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
//
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


import QtQuick          2.15
import QtQuick.Controls 2.15

import gnomonQuick.Style 1.0 as G

ToolBar {

    id: _control;

    background: Rectangle {
        implicitHeight: G.Style.toolBarHeight
        color: G.Style.colors.bgColor;
    }

}

//
// ToolBar.qml ends here
