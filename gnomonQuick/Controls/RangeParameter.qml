import QtQuick
import QtQuick.Controls

import gnomonQuick.Style as G
import gnomonQuick.Controls as G
import gnomonQuick.Icons as G

Control {
    id: _control

    required property var param
    property int decimals: 2

    implicitHeight: _label.implicitHeight + _range.implicitHeight + _left_value_input.implicitHeight

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

    TextField {
		id: _left_value_input;

        anchors.top: _range.bottom
        anchors.left: parent.left
        anchors.right: parent.horizontalCenter

        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        selectByMouse: true
        mouseSelectionMode: TextInput.SelectCharacters
        readOnly: false

		text: _range.first.value.toFixed(_control.decimals)
        font: G.Style.fonts.value
		color: _left_value_input.activeFocus? G.Style.colors.hoveredBaseColor : G.Style.colors.textColorBase
        selectionColor: G.Style.colors.fgColor
        selectedTextColor: G.Style.colors.hoveredBaseColor

		validator: DoubleValidator {
            bottom: _range.from
            top: _range.second.value
            decimals: _control.decimals
            notation: DoubleValidator.StandardNotation
        }
        errorText: "Enter a number"

		onEditingFinished: {
            if (_left_value_input.acceptableInput) {
                _control.param.rmin = parseFloat(_left_value_input.text);
            }
            if (_right_value_input.acceptableInput) {
                _control.param.rmax = parseFloat(_right_value_input.text);
            }
        }

        Keys.onReturnPressed: editingFinished()
	}

    TextField {
        id: _right_value_input;

        anchors.top: _range.bottom
        anchors.left: parent.horizontalCenter
        anchors.right: parent.right

        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        selectByMouse: true
        mouseSelectionMode: TextInput.SelectCharacters
        readOnly: false

        text: _range.second.value.toFixed(_control.decimals)
        font: G.Style.fonts.value
        color: _right_value_input.activeFocus? G.Style.colors.hoveredBaseColor : G.Style.colors.textColorBase
        selectionColor: G.Style.colors.fgColor
        selectedTextColor: G.Style.colors.hoveredBaseColor

        validator: DoubleValidator {
            bottom: _range.first.value
            top: _range.to
            decimals: _control.decimals
            notation: DoubleValidator.StandardNotation
        }
        errorText: "Enter a number"

        onEditingFinished: {
            if (_left_value_input.acceptableInput) {
                _control.param.rmin = parseFloat(_left_value_input.text);
            }
            if (_right_value_input.acceptableInput) {
                _control.param.rmax = parseFloat(_right_value_input.text);
            }
        }

        Keys.onReturnPressed: editingFinished()
    }

	Rectangle {
	    anchors.left: _left_value_input.left
	    anchors.top: _left_value_input.top
	    anchors.bottom: _right_value_input.bottom
	    anchors.right: _right_value_input.right

	    z: _left_value_input.z - 1

        color: G.Style.colors.gutterColor

	    Rectangle {
	        anchors.top: parent.top
	        anchors.horizontalCenter: parent.horizontalCenter
	        width: 1
	        height: parent.height - G.Style.smallPadding/2
	        radius: 1

            color: G.Style.colors.bgColor
	    }

	    Rectangle {
	        anchors.top: parent.top
	        anchors.horizontalCenter: parent.horizontalCenter
	        height: 1
	        width: parent.width - G.Style.smallPadding
	        radius: 1

            color: G.Style.colors.bgColor
	    }
    }
}
