import QtQuick
import QtQuick.Controls

import gnomonQuick.Style as G

Control
{
  id: _control;

  property string icon: "";
  property int size: G.Style.iconMedium;
  property color color: G.Style.colors.textColorBase;

  readonly property real _implicitSize: icon.toString() ? size : 0

  implicitWidth: _implicitSize
  implicitHeight: _implicitSize

  FontLoader {
    id: _loader;
    source: "qrc:/ttf/materialdesignicons-webfont.ttf";
  }

  Text {
    anchors.fill: _control;

    color: _control.color;
    text: _control.icon;
    font.pixelSize: _control.size;
    font.family: _loader.name;
    verticalAlignment: Text.AlignVCenter;
    horizontalAlignment: Text.AlignHCenter;
  }
}
