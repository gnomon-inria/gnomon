import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G


ItemDelegate {

  id: _control;
  property int textStyle: Text.Normal;

  implicitHeight: G.Style.comboBoxHeight;

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
    color: style === Text.Outline ? G.Style.colors.textColorLightNeutral : G.Style.colors.textColorNeutral;
    text: _control.text;
    style: _control.textStyle;
    styleColor: G.Style.colors.textColorDarkNeutral;
    verticalAlignment: Text.AlignVCenter;
    elide: Text.ElideRight
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
