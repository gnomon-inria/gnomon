import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G


CheckBox {
    id: _control

    implicitHeight: _control.indicator.height
    implicitWidth: _control.indicator.width + _control.contentItem.implicitWidth
    spacing: G.Style.smallPadding
    property string tooltip: "";
    hoverEnabled: enabled

    indicator: Rectangle {

        height: G.Style.sizes.s5
        width: G.Style.sizes.s5
        color: G.Style.colors.gutterColor
        radius: G.Style.panelRadius

        Rectangle {

            height: G.Style.sizes.s4
            width: G.Style.sizes.s4
            anchors.centerIn: parent;

            color: _control.down ? G.Style.colors.hoveredBaseColor : G.Style.colors.textColorBase
            radius: G.Style.panelRadius
            visible: _control.checked

        }
    }

    contentItem: Label {
        id: _content

        text: _control.text
        font: G.Style.fonts.label
        color: _control.down ? G.Style.colors.hoveredBaseColor : G.Style.colors.textColorBase
        verticalAlignment: Text.AlignVCenter
        leftPadding: _control.indicator.width + _control.spacing
    }


    G.ToolTip {
        text: _control.tooltip;
        visible: _control.tooltip && _control.hovered
    }

}
