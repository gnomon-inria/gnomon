import QtQuick
import QtQuick.Controls

import Qt.labs.platform  1.0 as P

import gnomonQuick.Style as G
import gnomonQuick.Controls as G

Control {
    id: _control

    required property var param

    implicitHeight: _label.implicitHeight + _value.implicitHeight + 2*G.Style.smallPadding

    G.ToolTip {
        visible: _control.hovered
        text: param.doc
    }

    Label {
        id: _label

        anchors.left: _control.left
        anchors.top: _control.top

        text: _control.param.label.toUpperCase()
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
    }

    G.TextField {
        id: _value

        anchors.right: _control.right
        anchors.top: _label.bottom
        anchors.left: parent.left
        anchors.margins: G.Style.smallPadding

        text: _control.param ? _control.param.value : ""
        color: G.Style.colors.hoveredBaseColor
        font: G.Style.fonts.value

        onTextChanged: {
            if(_control.param) {
                _control.param.value = text
            }
        }
    }
}
