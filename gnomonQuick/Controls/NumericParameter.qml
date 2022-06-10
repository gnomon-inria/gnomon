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

        value: _control.param.value
        decimals: 2

        onValueChanged: {
            _control.param.value = _slider.value
            // _value.text = _slider.value.toFixed(_control.param.decimals)
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

}
