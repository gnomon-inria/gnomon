import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15
import Qt.labs.platform  1.0 as P

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G

Control {
    id: _control

    required property var param

    implicitWidth: G.Style.smallPanelWidth
    implicitHeight: _label.implicitHeight + _colors.implicitHeight + _value.implicitHeight + 2*G.Style.smallPadding

    Label {
        id: _label

        anchors.left: parent.left
        anchors.top: parent.top

        text: param.label.toUpperCase()
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
    }

    G.ComboBox {
        id: _colors

        anchors.top: _label.bottom
        anchors.left: _control.left
        anchors.right: _edit_color.left
        anchors.rightMargin: G.Style.smallPadding

        model: param.colorIndexCount
        displayText: ""

        delegate: G.ComboBoxDelegate {
            width: _colors.width
            text: "Color " + param.colorIndexAt(index)

            background: Rectangle {
               color: param.color(param.colorIndexAt(index))
            }
        }

        background: Rectangle {
            id: _color_bg

            color: param.color(param.colorIndexAt(_colors.currentValue))

            radius: G.Style.buttonRadius
            border.color: G.Style.colors.gutterColor
            border.width: 1
        }
    }

    G.IconButton {
        id: _edit_color

        anchors.verticalCenter: _colors.verticalCenter
        anchors.right: _add_color.left
        anchors.rightMargin: G.Style.smallPadding
        size: G.Style.iconSmall;
        iconName: G.Icons.icons["pencil"]

        onClicked: {
            _color_dialog.color = param.color(param.colorIndexAt(_colors.currentValue))
            _color_dialog.open();
        }
    }

    G.IconButton {
        id: _add_color

        anchors.verticalCenter: _colors.verticalCenter
        anchors.right: _control.right
        anchors.rightMargin: G.Style.smallPadding
        size: G.Style.iconSmall;
        iconName: G.Icons.icons["plus"]

        onClicked: {
            console.log("Add color!")
        }
    }

    Label {
        id: _value

        anchors.topMargin: G.Style.sizes.s1
        anchors.top: _colors.bottom
        anchors.left: parent.left

        text: "Color " + param.colorIndexAt(_colors.currentValue)
        font: G.Style.fonts.value
        color: G.Style.colors.hoveredBaseColor
    }

    P.ColorDialog {
        id: _color_dialog
        onAccepted: {
            param.setColor(param.colorIndexAt(_colors.currentValue), _color_dialog.color)
        }
    }

    Connections {
        target: _control.param
        function onColorTableChanged(color_table) {
            _color_bg.color = Qt.binding(function() {return param.color(param.colorIndexAt(_colors.currentValue))})
        }
    }
}