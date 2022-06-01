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

    G.RangeSlider {

        id: _range

        anchors.top: _label.bottom;
        anchors.left: parent.left
        anchors.right: parent.right

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
