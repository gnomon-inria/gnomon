import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G

Control {
	id: _control

    property var value: 1
    property var min: 0
    property var max: 1
    property var decimals: 2

    property var label: "";
    property var doc: "";

    implicitHeight: _label.implicitHeight + _slider.implicitHeight + _value.implicitHeight

    hoverEnabled: true

    QtObject {
        id: _internal;

        property bool textEdit: false;
    }

    G.ToolTip {
        visible: _control.hovered && !_slider.pressed
        text: _control.doc
    }

	Label {

        id: _label

        anchors.left: parent.left
        anchors.top: parent.top

        text: _control.label.toUpperCase()
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

		from: _control.min
		to: _control.max
        value: _control.value

        onValueChanged: {
            if (_slider.value != _control.value)
                _control.value = _slider.value
        }
    }

    Label {
        id: _value

        anchors.topMargin: G.Style.sizes.s1
        anchors.top: _slider.bottom
        x: 0 //_slider.gaugeWidth
        visible: !_internal.textEdit

        text: _slider.value.toFixed(_control.decimals)
        font: G.Style.fonts.value
        color: G.Style.colors.hoveredBaseColor
    }

	TextInput {
		id: _value_input;

        anchors.topMargin: G.Style.sizes.s1
        anchors.top: _slider.bottom
        anchors.left: _slider.left
        anchors.right: _slider.horizontalCenter
        x: 0 //_slider.gaugeWidth
        visible: _internal.textEdit

		text: _slider.value.toFixed(_control.decimals)
        font: G.Style.fonts.value
		color: G.Style.colors.textColorBase

		validator: DoubleValidator{
            bottom: _control.min
            top: _control.max
            decimals: _control.decimals
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
