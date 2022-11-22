import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G

Control {
    id: _control

    required property var param

    implicitWidth: G.Style.smallPanelWidth
    implicitHeight: _label.implicitHeight + _colors.implicitHeight

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
        anchors.right: _current_color.left
        anchors.leftMargin: G.Style.smallPadding

        model: param.colorIndexCount

        delegate: G.ComboBoxDelegate {
            width: _colors.width
            text: "Color " + param.colorIndexAt(index)

            background: Rectangle {
               color: param.color(param.colorIndexAt(index))
            }
        }

        background: Rectangle {
            color: G.Style.colors.bgColor
        }
    }

    Control {
        id: _current_color

        anchors.top: _label.bottom
        anchors.right: _control.right
        anchors.rightMargin: G.Style.smallPadding

        implicitWidth: G.Style.buttonWidth
        implicitHeight: _colors.implicitHeight

        background: Rectangle{
            color: param.color(param.colorIndexAt(_colors.currentValue))

            radius: G.Style.buttonRadius
            border.color: G.Style.colors.gutterColor
            border.width: 1
        }
    }
}