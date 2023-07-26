import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {
  id: _control

    property var value: 1
    property var min: 0
    property var max: 1
    property var decimals: 2

    property var label: "";
    property var doc: "";

    implicitHeight: _label.implicitHeight + _slider.implicitHeight + _value_input.implicitHeight

    hoverEnabled: true

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

  TextField {
    id: _value_input;

    anchors.top: _slider.bottom
    anchors.left: parent.left
    anchors.right: parent.right

    horizontalAlignment: Qt.AlignHCenter
    verticalAlignment: Qt.AlignVCenter

    selectByMouse: true
    mouseSelectionMode: TextInput.SelectCharacters
    readOnly: false

    text: _slider.value.toFixed(_control.decimals)
    font: G.Style.fonts.value
    color: _value_input.activeFocus? G.Style.colors.hoveredBaseColor : G.Style.colors.textColorBase
    selectionColor: G.Style.colors.fgColor
    selectedTextColor: G.Style.colors.hoveredBaseColor

    validator: DoubleValidator {
      bottom: _control.min
      top: _control.max
      decimals: _control.decimals
      notation: DoubleValidator.StandardNotation
    }
    //errorText: "Enter a number between "+_control.min+" and "+_control.max;

    onEditingFinished: {
      _control.value = parseFloat(text);
    }

    Keys.onReturnPressed: editingFinished()
  }

  Rectangle {
      anchors.fill: _value_input
      z: _value_input.z - 1

        color: G.Style.colors.gutterColor

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
