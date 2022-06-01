import QtQuick
import QtQuick.Controls

import gnomonQuick.Style as G
import gnomonQuick.Controls as G
import gnomonQuick.Icons as G


RangeSlider {
    id: _range

    implicitWidth: G.Style.controlWidth
    implicitHeight: G.Style.gutterHeight
    padding: 6

    background: Rectangle {
        id: _gutter

        color: G.Style.colors.gutterColor
    }

    contentItem: Rectangle {
        id: _gauge

        x: _range.leftPadding + _range.first.visualPosition * _range.availableWidth
        width: (_range.second.visualPosition - _range.first.visualPosition) * _range.availableWidth
        height: _range.availableHeight

        radius: G.Style.panelRadius
        color: G.Style.colors.textColorBase
    }

    first.handle: Rectangle {
        x: _range.leftPadding + _range.first.visualPosition * (_range.availableWidth - width)
        y: _range.topPadding + _range.availableHeight / 2 - height / 2
        implicitHeight: _gauge.height
        implicitWidth: _gauge.height
        radius: _gauge.height / 2
        color: _range.first.pressed || _range.first.hovered ? G.Style.colors.hoveredBaseColor : G.Style.colors.textColorBase
    }

    second.handle: Rectangle {
        x: _range.leftPadding + _range.second.visualPosition * (_range.availableWidth - width)
        y: _range.topPadding + _range.availableHeight / 2 - height / 2
        implicitHeight: _gauge.height
        implicitWidth: _gauge.height
        radius: _gauge.height / 2
        color: _range.second.pressed || _range.second.hovered ? G.Style.colors.hoveredBaseColor : G.Style.colors.textColorBase
    }
}
