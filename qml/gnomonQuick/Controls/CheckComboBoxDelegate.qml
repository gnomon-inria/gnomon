import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Item {
  id: _control;

  property alias checked: _check.checked;
  property bool highlighted: _check.highlighted;
  property bool hovered: _check.hovered;
  property bool down: _check.down;
  property alias text: _check.text;

  implicitHeight: G.Style.comboBoxHeight;

  CheckDelegate {
    id: _check
    anchors.fill: parent

    font: G.Style.fonts.formLabel;

    background: Rectangle {
      opacity: enabled ? 0.8 : 0.1
      color: getBgColor()
      border.width: 0.5
      border.color: getEmbossColor()
    }

    contentItem: Label {
      anchors.leftMargin: 8;

      font: G.Style.fonts.formLabel;
      color: G.Style.colors.textColorBase;
      text: _control.text;
      verticalAlignment: Text.AlignVCenter;
      elide: Text.ElideRight
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
