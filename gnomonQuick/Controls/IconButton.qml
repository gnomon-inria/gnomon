import QtQuick
import QtQuick.Controls

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Button
{
  id: _control;

  property alias iconName: _icon.icon;
  property alias size: _icon.size;

  property color color : G.Style.colors.textColorBase;
  property color hoverColor : G.Style.colors.hoveredBaseColor;

  property string tooltip: "";
  property alias containsMouse: _icon_area.containsMouse

  property bool shift_click: false;
  property bool ctrl_click: false;

  implicitHeight: implicitContentHeight;
  implicitWidth: implicitContentWidth;

  background: Rectangle {
    visible: false;
  }

  contentItem: G.Icon {
    id: _icon;

    color: _control.hoverColor && _icon_area.containsMouse ? _control.hoverColor : _control.color

    MouseArea {
      id: _icon_area;

      anchors.fill: parent;
      hoverEnabled: true;

      onClicked: {
        _control.ctrl_click = (mouse.modifiers & Qt.ControlModifier) != 0
        _control.shift_click = (mouse.modifiers & Qt.ShiftModifier) != 0
        _control.clicked()
      }
    }
  }

  G.ToolTip {
    text: _control.tooltip;
    visible: _control.tooltip && _icon_area.containsMouse
  }
}
