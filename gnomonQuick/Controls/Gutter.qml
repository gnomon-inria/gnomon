import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G


Control {
    id: _control

    clip: true;

    background: Rectangle {
      radius: G.Style.panelRadius;
      color: G.Style.colors.gutterColor;
    }

    layer.enabled: true
    layer.effect: OpacityMask
    {
      maskSource: Rectangle
      {
        width: _control.width
        height: _control.height
        radius: G.Style.panelRadius;
      }
    }
}
