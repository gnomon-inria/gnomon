import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G


ItemDelegate {
    id: self

    implicitHeight: G.Style.formDelegateHeight
    highlighted: false
    font: G.Style.fonts.formLabel

    property color color: G.Style.colors.fgColor
    property color hoverColor: G.Style.colors.neutralColor
    property color highlightColor: G.Style.colors.baseColor
    property color textColor: G.Style.colors.textColorNeutral

    property string tooltip: ""

    contentItem: Item {

        Label {
            anchors.fill: parent;
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft

            leftPadding: G.Style.smallPadding;

            text: self.text;
            visible: text;
            font: self.font
            color: self.textColor

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

    G.ToolTip {
        text: self.tooltip;
        visible: self.tooltip && self.hovered
   }

    function getEmbossColor() {
      if(down || highlighted) return G.Style.colors.embossColorBlue;
      return G.Style.colors.embossColorNeutral;
    }

    function getBgColor() {
      if(down || highlighted) return self.highlightColor;
      if(hovered) return self.hoverColor;
      return self.color;
    }
}
