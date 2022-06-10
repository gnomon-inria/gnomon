import QtQuick          2.15
import QtQuick.Controls 2.15

import gnomonQuick.Style 1.0 as G

ToolTip {
    id: _control;

    contentItem: Text{
        anchors.fill:parent

        verticalAlignment: Text.AlignVCenter;
        horizontalAlignment: Text.AlignHCenter;

        color: G.Style.colors.neutralColor
        font: G.Style.fonts.value
        text: _control.text
    }

    background: Rectangle {
        color: G.Style.colors.fgColor
        radius: G.Style.panelRadius

        opacity: 0.9
    }

    delay: 500

    implicitHeight: G.Style.sizes.s5
    implicitWidth: G.Style.sizes.s11
}

//
// ToolTip.qml ends here
