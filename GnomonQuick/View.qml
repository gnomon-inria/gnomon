import QtQuick            2.15
import QtQuick.Controls   2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts    1.15

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

Rectangle {

    id: _control;

    color: X.Style.linkColor;

    XVis.Viewer {

        id: _viewer;

        anchors.fill: parent;

        mouseEnabled: true;
    }

    layer.enabled: true
    layer.effect: OpacityMask
    {
        maskSource: Rectangle
        {
            width: _control.width
            height: _control.height
            radius: 4;
        }
    }
}
