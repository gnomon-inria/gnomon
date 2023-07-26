import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls as G
import gnomonQuick.Style as G


ItemDelegate {
    id: self

    implicitHeight: G.Style.formDelegateHeight
    highlighted: false
    font: G.Style.fonts.formLabel

    contentItem: Item {

        Label {
            anchors.fill: parent;
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft

            leftPadding: G.Style.smallPadding;

            text: self.text;
            visible: text;
            font: self.font
            color: G.Style.colors.textColorNeutral

        }
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 60
        opacity: enabled ? 0.8 : 0.3
        color: getBgColor()

        Rectangle {
            width: parent.width
            height: 1
            color: getEmbossColor();
            anchors.bottom: parent.bottom
        }
    }

    function getEmbossColor() {
      if(down || highlighted) return G.Style.colors.embossColorBlue;
      return G.Style.colors.embossColorNeutral;
    }

    function getBgColor() {
      if(down || highlighted) return G.Style.colors.baseColor;
      if(hovered) return G.Style.colors.neutralColor;
      return G.Style.colors.fgColor;
    }
}
