import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {
	id: _control

	required property var param;
    //for crossParameters
    readonly property var paramType: _control.param.type

    implicitHeight: _label.implicitHeight + _slider.implicitHeight + _value.implicitHeight
    width: G.Style.smallPanelWidth
    background: Rectangle{
        color: "transparent"
    }

    hoverEnabled: true

    G.ToolTip {
        visible: _control.hovered && !_slider.pressed
        text: param.doc
    }

	Label {

        id: _label

        anchors.left: parent.left
        anchors.top: parent.top

        text: param.label.toUpperCase()
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
    }

    G.Slider {
        id: _slider

        anchors.top: _label.bottom
        anchors.left: parent.left
        anchors.right: parent.right

		from: _control.param.min
		to: _control.param.max
		decimals: _control.param.decimals
		//increment: 1.0/Math.pow(10, _control.param.decimals)
        value: _control.param.value

        onValueChanged: {
            _control.param.value = _slider.value
        }

    }

    Label {
        id: _value

        anchors.topMargin: G.Style.sizes.s1
        anchors.top: _slider.bottom
        x: _slider.gaugeWidth

        text: _slider.value.toFixed(_control.param.decimals)
        font: G.Style.fonts.value
        color: G.Style.colors.hoveredBaseColor
    }

    Component.onCompleted: {
        console.log("MIN: " + _control.param.min)
        console.log("MAX: " + _control.param.max)
        console.log("VAL: " + _control.param.value)
        console.log("DECIMALS: " + _control.param.decimals)
        console.log("INC: " + _slider.increment)
    }
}
