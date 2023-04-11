import QtQuick
import QtQuick.Controls

import gnomonQuick.Style as G
import gnomonQuick.Controls as G
import gnomonQuick.Icons as G

Control {
    id: _control

    required property var param
    property int decimals: 2

    implicitHeight: _label.implicitHeight + _switch.implicitHeight

    G.ToolTip {
        visible: _control.hovered && !_switch.pressed
        text: _control.param ? _control.param.doc : ""
    }

    Label {
        id: _label

        anchors.left: _control.left
        anchors.top: _control.top

        text: _control.param ? _control.param.label.toUpperCase() : ""
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
    }

    Label {
        id: _value

        anchors.left: _control.left
        anchors.top: _label.bottom
        width: G.Style.sizes.s6

        text: _switch.checked ? "TRUE" : "FALSE"
        color: G.Style.colors.hoveredBaseColor
        font: G.Style.fonts.value
    }

    G.Switch {

        id: _switch

        anchors.top: _label.bottom
        anchors.left: _value.right
        anchors.leftMargin: G.Style.sizes.s4

        onToggled: {
            if (_control.param)
                _control.param.value = _switch.checked ? 1 : 0
        }
    }

    Component.onCompleted: {
        if (_control.param)
            _switch.checked = _control.param.value
    }
}
