import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G

Control {
	id: _control

    property var value: _spinbox.value / Math.pow(10, _control.decimals)
    property var min: 0
    property var max: 1
    property var decimals: 2

    property var label: "";
    property var doc: "";

    implicitHeight: _label.implicitHeight + _spinbox.implicitHeight

    hoverEnabled: true

    G.ToolTip {
        visible: _control.hovered
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

    SpinBox {
        id: _spinbox

        anchors.top: _label.bottom
        anchors.left: parent.left
        anchors.right: parent.right

		from: _control.min * Math.pow(10, _control.decimals)
		to: _control.max * Math.pow(10, _control.decimals)
		stepSize: 1
        value: _control.value * Math.pow(10, _control.decimals)
        editable: true

        font: G.Style.fonts.value

        contentItem: TextInput {
            z: 2
            text: _spinbox.textFromValue(_spinbox.value, _spinbox.locale)

            font: _spinbox.font
            color: G.Style.colors.textColorBase
            selectionColor: G.Style.colors.bgColor
            selectedTextColor: G.Style.colors.hoveredBaseColor
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter

            selectByMouse: true
            mouseSelectionMode: TextInput.SelectCharacters
            readOnly: !_spinbox.editable
            validator: _spinbox.validator

            onTextEdited: {
                _spinbox.value = _spinbox.valueFromText(text, _spinbox.locale)
            }
        }

        MouseArea {
            id: _wheel_area
            anchors.fill: parent
            propagateComposedEvents: true

            onWheel: (wheel)=> {
                if (wheel.angleDelta.y < 0) {
                    _spinbox.decrease()
                } else if (wheel.angleDelta.y > 0) {
                   _spinbox.increase()
                }
                wheel.accepted=true
            }
        }

        up.indicator: G.IconButton {
            height: parent.height
            anchors.right: parent.right
            anchors.top: parent.top

            iconName: G.Icons.icons["plus"]
            size: G.Style.iconMedium

            onClicked: {
                _spinbox.increase()
            }

            background: Rectangle {
                color: G.Style.colors.fgColor
                radius: G.Style.buttonRadius
              }
        }

        down.indicator: G.IconButton {
            height: parent.height
            anchors.left: parent.left
            anchors.top: parent.top

            iconName: G.Icons.icons["minus"]
            size: G.Style.iconMedium

            onClicked: {
                _spinbox.decrease()
            }

            background: Rectangle {
                color: G.Style.colors.fgColor
                radius: G.Style.buttonRadius
              }
        }

        background: Rectangle {
            color: G.Style.colors.gutterColor
            radius: G.Style.buttonRadius
        }

        validator: DoubleValidator {
            bottom: Math.min(_spinbox.from, _spinbox.to)
            top:  Math.max(_spinbox.from, _spinbox.to)
        }

        textFromValue: function(value, locale) {
            return Number(value / Math.pow(10, _control.decimals)).toLocaleString(locale, 'f',  _control.decimals)
        }

        valueFromText: function(text, locale) {
            return Number.fromLocaleString(locale, text) * Math.pow(10, _control.decimals)
        }
    }
}
