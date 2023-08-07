import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Row {

  id: _control;

  property int formId;
  property string formName;

  spacing: G.Style.smallRowSpacing;

  Rectangle {
    id: _thumbnail;

    anchors.verticalCenter: parent.verticalCenter;
    height: G.Style.thumbnailMedium
    width: G.Style.thumbnailMedium
    radius: G.Style.panelRadius
    color: G.Style.colors.lightBlue

    Image {
      anchors.fill: _thumbnail;
      fillMode: Image.PreserveAspectFit
      source: formId != -1 ? "image://thumbnails/" + formId : null
    }

  }

  Label {

    anchors.verticalCenter: parent.verticalCenter;

    font: G.Style.fonts.formLabel;
    color: G.Style.colors.textColorBase;
    text: _control.formName;
    verticalAlignment: Text.AlignVCenter;
  }


}
