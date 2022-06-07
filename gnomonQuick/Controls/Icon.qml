import QtQuick
import QtQuick.Controls

import gnomonQuick.Style as G

Control
{
  id: _control;

  property string icon: "";
  property int size: G.Style.iconMedium;
  property color color: G.Style.colors.textColorBase;

  property bool clickable: false;
  property string tooltip: "";

  readonly property real _implicitSize: icon.toString() ? size : 0

  signal clicked

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
  }

  ToolTip.visible: _control.tooltip && _mouse_area.containsMouse;
  ToolTip.text: _control.tooltip;
  ToolTip.delay: 500;

  MouseArea {
    id: _mouse_area;

    anchors.fill: _control;
    hoverEnabled: true;

    onClicked: {
      if (_control.clickable)
        _control.clicked()
    }
  }
}
