import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15
import Qt.labs.platform  1.0 as P

import Qt5Compat.GraphicalEffects

import xQuick.Vis         1.0 as XVis

import gnomonQuick.Controls  1.0 as G
import gnomonQuick.Style  1.0 as G

import gnomon.Visualization 1.0 as GV
import gnomon.MetaData    1.0 as GM

Rectangle {

    id: self;

    color: G.Style.colors.bgColor;

    focus: true;

    property alias view: _view;
    property var viewLogic;
    property var visualizations;

    property bool shift_pressed: false;
    property bool ctrl_pressed: false;
    property bool export_enabled: false;

    signal droppedFromFile(string path)
    signal droppedFromManager(int index)

    signal transmit();

    signal switchTo2D();
    signal switchTo2DXY();
    signal switchTo2DXZ();
    signal switchTo2DYZ();
    signal switchTo3D();
    signal sliceChange(int value);
    signal clear();


    XVis.Viewer {

        id: _view;

        anchors.fill: parent;

        mouseEnabled: false;
        enabled: false;

        // onCaptured: {
        //     self.transmit();
        // }
    }

    layer.enabled: true
    layer.effect: OpacityMask
    {
        maskSource: Rectangle
        {
             width: self.width
            height: self.height
            radius: G.Style.panelRadius;
        }
    }
}
