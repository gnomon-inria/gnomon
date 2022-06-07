import QtQuick
import QtQuick.Controls

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Button
{
  id: _control;

  property alias iconName: _icon.icon;
  property alias size: _icon.size;
  property alias color: _icon.color;

  property string tooltip: "";
  property alias containsMouse: _icon_area.containsMouse

  implicitHeight: implicitContentHeight;
  implicitWidth: implicitContentWidth;

  background: Rectangle {
    visible: false;
  }

  contentItem: G.Icon {
    id: _icon;

    MouseArea {
      id: _icon_area;

      anchors.fill: parent;
      hoverEnabled: true;

      onClicked: {
        _control.clicked()
      }
    }
  }

  ToolTip.visible: _control.tooltip && _icon_area.containsMouse
  ToolTip.text: _control.tooltip;
  ToolTip.delay: 500;
}
