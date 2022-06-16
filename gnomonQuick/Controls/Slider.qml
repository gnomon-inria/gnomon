import QtQuick
import QtQuick.Controls

import gnomonQuick.Style as G
import gnomonQuick.Controls as G
import gnomonQuick.Icons as G


Slider {
    id: _control

    readonly property alias gaugeWidth: _gauge.width

    implicitWidth: G.Style.controlWidth
    implicitHeight: G.Style.gutterHeight
    padding: 6

    background: Rectangle {
        id: _gutter

        color: G.Style.colors.gutterColor
    }

    contentItem: Rectangle {
        id: _gauge

        x: _control.leftPadding
        width: (_control.visualPosition) * _control.availableWidth
        height: _control.availableHeight

        radius: G.Style.panelRadius
        color: G.Style.colors.textColorBase
    }

    handle: Rectangle {
        x: _control.leftPadding + _control.visualPosition * (_control.availableWidth - width)
        y: _control.topPadding + _control.availableHeight / 2 - height / 2
        implicitHeight: _gauge.height
        implicitWidth: _gauge.height
        radius: _gauge.height / 2
        color: _control.pressed || _control.hovered ? G.Style.colors.hoveredBaseColor : G.Style.colors.textColorBase
    }

}
