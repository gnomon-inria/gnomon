// Style.qml ---
//
// Filename: Style.qml
// Description:
// Author: Romain TETLEY
// Maintainer:
// Created: lun. mai 16 10:15:44 2022 (+0200)
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


pragma Singleton

import QtQuick          2.15
import QtQuick.Controls 2.15

// js style
import "./style.qml.js" as S


QtObject {
    id: _self

    enum Mode {
        Dark,
        Light
    }

    enum ButtonType {
        Neutral,
        Base,
        OK,
        Warning,
        Danger
    }

    enum LabelType {
        Header,
        SubHeader,
        Label,
        Value,
        Button
    }

    // TODO: Define light theme and make the colors in colorTheme object conditional
    property int mode: Style.Mode.Dark
    // property alias colors: _colorTheme
    // property alias fonts: _fonts
    property int panelRadius: S.spacing["1"]

    property int buttonRadius: S.spacing["1"]
    property int buttonPadding: S.spacing["2"]
    property int buttonWidth: S.spacing["10"]

    property int smallColumnSpacing: S.spacing["2"]
    property int mediumColumnSpacing: S.spacing["4"]
    property int largeColumnSpacing: S.spacing["6"]

    property int smallRowSpacing: S.spacing["2"]
    property int mediumRowSpacing: S.spacing["4"]
    property int largeRowSpacing: S.spacing["6"]

    property int smallPadding: S.spacing["2"]
    property int mediumPadding: S.spacing["4"]
    property int largePadding: S.spacing["6"]

    property int scrollBarWidth: S.spacing["2"]
    property int scrollBarRadius: S.spacing["1"]

    property int toolBarHeight: S.spacing["6"]

    property int formSelectorHeight: S.spacing["12"]
    property int formDelegateHeight: S.spacing["7"]

    property FontLoader regular: FontLoader {
        id: _poppinsRegular
        source: "qrc:/ttf/Poppins-Regular.ttf"
        Component.onCompleted: {
            console.log("NAME POPPINS REGULAR", _poppinsRegular.name)
            console.log("STATUS", _poppinsRegular.status)

        }
    }

    property FontLoader medium: FontLoader {
        id: _poppinsMedium
        source: "qrc:/ttf/Poppins-Medium.ttf"
        Component.onCompleted: {
            console.log("NAME POPPINS MEDIUM", _poppinsMedium.name)
            console.log("STATUS", _poppinsMedium.status)

        }
    }

    property FontLoader light: FontLoader {
        id: _poppinsLight
        source: "qrc:/ttf/Poppins-Light.ttf"
        Component.onCompleted: {
            console.log("NAME POPPINS LIGHT", _poppinsMedium.name)
            console.log("STATUS", _poppinsMedium.status)

        }
    }

    property FontLoader bold: FontLoader {
        id: _poppinsBold
        source: "qrc:/ttf/Poppins-Bold.ttf"
        Component.onCompleted: {
            console.log("NAME POPPINS BOLD", _poppinsMedium.name)
            console.log("STATUS", _poppinsMedium.status)

        }
    }


    // /////////////////////////////////////////////////////////////////////////////
    // Color definitions
    // /////////////////////////////////////////////////////////////////////////////

    property QtObject colors: QtObject {
        id: _colorTheme;

        //From previous style (gnomon)
        property color highlightColor: "#20E8C0"

        //panel colors (neutral)
        property color bgColor: S.colors.gray["700"]
        property color gutterColor: S.colors.gray["900"]
        property color fgColor: S.colors.gray["600"]
        property color embossColor: S.colors.gray["600"]

        //basic color theme
        property color baseColor: S.colors.blue["500"]
        property color neutralColor: S.colors.gray["500"]
        property color dangerColor: S.colors.red["500"]
        property color warningColor: S.colors.orange["500"]
        property color okColor: S.colors.green["500"]

        //hovered color theme
        property color hoveredBaseColor: S.colors.blue["400"]
        property color hoveredNeutralColor: S.colors.gray["400"]
        property color hoveredDangerColor: S.colors.red["400"]
        property color hoveredWarningColor: S.colors.orange["400"]
        property color hoveredOkColor: S.colors.green["400"]

        //text colors
        property color textColorBase: S.colors.blue["100"]
        property color textColorOk: S.colors.green["100"]
        property color textColorDanger: S.colors.red["100"]
        property color textColorWarning: S.colors.orange["100"]
        property color textColorNeutral: S.colors.gray["100"]
        property color textColorFaded: S.colors.gray["200"]

        //emboss for different colors
        property color embossColorBlue: S.colors.blue["400"]
        property color embossColorNeutral: S.colors.gray["500"]

        //otherColors
        property color lightBlue: S.colors.blue["200"]
    }

    // /////////////////////////////////////////////////////////////////////////////
    // Font definitions
    // /////////////////////////////////////////////////////////////////////////////

    property QtObject fonts: QtObject {
        id: _fonts;

        property font header: Qt.font({
            family: "Poppins",
            weight: "Regular",
            pointSize: S.fontScale["200"],
        })

        property font subHeader: Qt.font({
            family: "Poppins",
            weight: "Regular",
            pointSize: S.fontScale["100"],
        })

        property font label: Qt.font({
            family: "Poppins",
            weight: "Light",
            pointSize: S.fontScale["50"],
        })

        property font value: Qt.font({
            family: "Poppins",
            weight: "Regular",
            pointSize: S.fontScale["100"],
        })

        property font button: Qt.font({
            family: "Poppins",
            weight: Font.Bold,
            pointSize: S.fontScale["200"],
        })

        property font buttonHovered: Qt.font({
            family: "Poppins",
            weight: Font.Bold,
            pointSize: S.fontScale["200"],
            underline: true
        })

        property font h1: Qt.font({
            family: "Poppins",
            weight: Font.Medium,
            pointSize: S.fontScale["700"],
        })


        property font h2: Qt.font({
            family: "Poppins",
            weight: Font.Medium,
            pointSize: S.fontScale["400"],
        })

        property font h3: Qt.font({
            family: "Poppins",
            weight: Font.Normal,
            pointSize: S.fontScale["200"],
        })

        property font formLabel: Qt.font({
            family: "Poppins",
            weight: Font.Medium,
            pointSize: S.fontScale["100"],
        })

    }

}

//
// Style.qml ends here
