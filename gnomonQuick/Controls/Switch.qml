import QtQuick
import QtQuick.Controls

import gnomonQuick.Style as G
import gnomonQuick.Controls as G
import gnomonQuick.Icons as G

Switch {
    id: _control

    implicitHeight: G.Style.gutterHeight
    implicitWidth: _control.availableHeight * 2 + _control.leftPadding + _control.rightPadding
    padding: 6

    background: Rectangle {
        id: _gutter

        radius: parent.height
        color: G.Style.colors.gutterColor
    }

    indicator: Rectangle {
        id: _gauge

        anchors.verticalCenter: parent.verticalCenter
        x: _control.leftPadding
        width: _control.checked ? _control.availableWidth : _control.availableWidth / 2
        height: _control.availableHeight
        radius: _control.availableHeight
        color: G.Style.colors.textColorBase

        Rectangle {
            id: _indicator
            width: _gauge.height
            height: _gauge.height
            x: _control.checked ? _control.availableWidth - width : 0

            radius: _gauge.height
            color: _control.hovered || _control.pressed ? G.Style.colors.hoveredBaseColor : G.Style.colors.textColorBase
        }
    }

}
