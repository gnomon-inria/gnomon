import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G

Control {
	id: _control

	required property var param;
    //for crossParameters
    readonly property var paramType: _control.param ? _control.param.type : undefined

    implicitHeight: _label.implicitHeight + _slider.implicitHeight + _value.implicitHeight
    width: G.Style.smallPanelWidth
    background: Rectangle{
        color: "transparent"
    }

    hoverEnabled: true

    QtObject {
        id: _internal;

        property bool textEdit: false;
    }

    G.ToolTip {
        visible: _control.hovered && !_slider.pressed
        text: param ? param.doc : ""
    }

	Label {

        id: _label

        anchors.left: parent.left
        anchors.top: parent.top

        text: param ? param.label.toUpperCase() : ""
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
    }

    G.IconButton {

        id: _enable_text;

        anchors.right: parent.right
        anchors.top: parent.top
        enabled: !_internal.textEdit

        size: G.Style.iconSmall;
        color: G.Style.colors.textColorBase

        iconName: G.Icons.icons["pencil"];

        onClicked: {
            _internal.textEdit = !_internal.textEdit
        }
    }

    G.Slider {
        id: _slider

        anchors.top: _label.bottom
        anchors.left: parent.left
        anchors.right: parent.right

		from: _control.param ? _control.param.min : 0
		to: _control.param ? _control.param.max : 0
		// decimals: _control.param.decimals
		//increment: 1.0/Math.pow(10, _control.param.decimals)
        value: _control.param ? _control.param.value : 0

        onValueChanged: {
			if(_control.param)
            	_control.param.value = _slider.value
        }

    }

    Label {
        id: _value

        anchors.topMargin: G.Style.sizes.s1
        anchors.top: _slider.bottom
        x: 0 //_slider.gaugeWidth
        visible: !_internal.textEdit

        text: _control.param ? _slider.value.toFixed(_control.param.decimals) : "0"
        font: G.Style.fonts.value
        color: G.Style.colors.hoveredBaseColor
    }

	TextInput {
		id: _value_input;


        anchors.topMargin: G.Style.sizes.s1
        anchors.top: _slider.bottom
        x: 0 //_slider.gaugeWidth
        visible: _internal.textEdit

		text: _control.param ? _slider.value.toFixed(_control.param.decimals) : "0"
        font: G.Style.fonts.value
		color: G.Style.colors.textColorBase

		validator: DoubleValidator{
            bottom: param ? param.min : 0
            top: param ? param.max : 0
            decimals: (param && param.decimals) ? param.decimals : 0
            notation: DoubleValidator.StandardNotation
        }

		onEditingFinished: {
            _slider.value = parseFloat(text);
            _internal.textEdit = false
        }

        Keys.onReturnPressed: editingFinished()
	}

    G.IconButton {

        id: _validate_text;

        anchors.right: parent.right
        anchors.topMargin: G.Style.sizes.s1
        anchors.top: _slider.bottom

        visible: _internal.textEdit
        enabled: _internal.textEdit

        size: G.Style.iconSmall;
        color: G.Style.colors.textColorBase

        iconName: G.Icons.icons["check"];

        onClicked: _value_input.editingFinished()
    }
}
