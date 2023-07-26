import QtQuick
import QtQuick.Controls

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Button
{
  id: _control;

  required property string iconName;
  property alias size: _icon.size;
  property alias rotation: _icon.rotation;
  property alias flip: _icon.flip

  property color color : G.Style.colors.textColorBase;
  property color hoverColor : G.Style.colors.hoveredBaseColor;

  property string tooltip: "";
  property alias containsMouse: _icon_area.containsMouse

  implicitHeight: implicitContentHeight;
  implicitWidth: implicitContentWidth;

  background: Rectangle {
    visible: false;
  }

  contentItem: G.Icon {
    id: _icon;

    icon : _control.iconName
    color: _control.hoverColor && _icon_area.containsMouse ? _control.hoverColor : _control.color

    MouseArea {
      id: _icon_area;

      anchors.fill: parent;
      hoverEnabled: true;

      onClicked: {
        _control.clicked()
      }
    }
  }

  G.ToolTip {
    text: _control.tooltip;
    visible: _control.tooltip && _icon_area.containsMouse
  }
}
