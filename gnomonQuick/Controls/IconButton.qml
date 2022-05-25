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

  implicitHeight: implicitContentHeight;
  implicitWidth: implicitContentWidth;

  background: Rectangle {
    visible: false;
  }
  contentItem: G.Icon {
    id: _icon;
  }
}
