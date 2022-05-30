import QtQuick
import QtQuick.Controls

import gnomonQuick.Style as G
import gnomonQuick.Controls as G
import gnomonQuick.Icons as G

Control {
    id: _control

    required property var param
    property int decimals: 2

    implicitHeight: _label.implicitHeight + _range.implicitHeight + _left_value.implicitHeight

    Label {
        id: _label

        anchors.left: _control.left
        anchors.top: _control.top

        text: _control.param.label.toUpperCase()
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
    }

    RangeSlider {
        id: _range

        anchors.top: _label.bottom;
        anchors.left: parent.left
        anchors.right: parent.right

        implicitHeight: G.Style.gutterHeight
        padding: 6

        from: _control.param.min
        to: _control.param.max
        first.value: _control.param.rmin
        second.value: _control.param.rmax

        first.onMoved: {
			_control.param.rmin = first.value
	    }
		second.onMoved: {
			_control.param.rmax = second.value
		}


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
            x: _range.leftPadding + _range.first.visualPosition * _range.availableWidth
            y: _range.topPadding + _range.availableHeight / 2 - height / 2
            implicitHeight: _gauge.height
            implicitWidth: _gauge.height
            radius: _gauge.height / 2
            color: _range.first.pressed || _range.first.hovered ? G.Style.colors.hoveredBaseColor : G.Style.colors.textColorBase
        }

        second.handle: Rectangle {
            x: _range.second.visualPosition * _range.availableWidth - width / 2
            y: _range.topPadding + _range.availableHeight / 2 - height / 2
            implicitHeight: _gauge.height
            implicitWidth: _gauge.height
            radius: _gauge.height / 2
            color: _range.second.pressed || _range.second.hovered ? G.Style.colors.hoveredBaseColor : G.Style.colors.textColorBase
        }
    }

    Label {

        id: _left_value

        anchors.topMargin: G.Style.sizes.s1
        anchors.top: _range.bottom

        x: _range.first.visualPosition * _gutter.width - width

        visible: _range.first.hovered || _range.first.pressed

        text: _range.first.value.toFixed(decimals)
        font: G.Style.fonts.value
        color: G.Style.colors.hoveredBaseColor
    }


    Label {

        id: _right_value

        anchors.topMargin: G.Style.sizes.s1
        anchors.top: _range.bottom

        x: _range.second.visualPosition * _gutter.width - width

        visible: _range.second.hovered || _range.second.pressed

        text: _range.second.value.toFixed(decimals)
        font: G.Style.fonts.value
        color: G.Style.colors.hoveredBaseColor
    }

}
