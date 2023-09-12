import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Imagine.impl

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

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
