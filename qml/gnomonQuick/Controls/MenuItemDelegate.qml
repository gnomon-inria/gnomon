import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls as G
import gnomonQuick.Style as G


MenuItem {
    id: self

    implicitHeight: G.Style.formDelegateHeight
    highlighted: false
    font: G.Style.fonts.menu

    contentItem: Item {

        Label {
            anchors.fill: parent;
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft

            leftPadding: G.Style.tinyPadding;

            text: self.text;
            visible: text;
            font: self.font
            color: getTextColor()

        }
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 60
        opacity: enabled ? 0.8 : 0.3
        color: getBgColor()
    }

    function getTextColor() {
      if(!enabled) return G.Style.colors.textColorDeEmphasize;
      return G.Style.colors.textColorNeutral;
    }

    function getBgColor() {
      if(enabled & hovered) return G.Style.colors.neutralColor;
      return G.Style.colors.fgColor;
    }
}
