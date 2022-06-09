import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import xQuick.Style       1.0 as X

import gnomonQuick.Style  1.0 as G

Shape {

    id: _self;

    property var edge;

    property Item src;
    property Item tgt;

    property int inputWorkspaceIndex;
    property int outputWorkspaceIndex;
    property int formIndex: edge ? edge.formIndex : -1;

    property point stt: Qt.point((src.x + src.width), (src.y + src.height/2));
    property point end: Qt.point( tgt.x,              (tgt.y + tgt.height/2));
    property point mid: Qt.point((stt.x + end.x)/2,   (stt.y + end.y)/2);

    property real delt: (mid.x - stt.x)/2;

    width: end.x - stt.x;
    height: end.y - stt.y;

    containsMode: Shape.FillContains

    ShapePath {
        fillColor: "transparent";

        strokeWidth: 3
        strokeColor: window.world.currentIndex === _self.formIndex
            ? G.Style.colors.highlightColor
            : G.Style.colors.neutralColor

        startX: _self.stt.x;
        startY: _self.stt.y;

        PathCubic {
            control1X: _self.end.x - _self.delt;
            control1Y: _self.stt.y;
            control2X: _self.stt.x + _self.delt;
            control2Y: _self.end.y;
                    x: _self.end.x;
                    y: _self.end.y
        }
    }

    TapHandler {
        onDoubleTapped: window.world.currentIndex = _self.formIndex;
    }

    Component.onCompleted: {
        console.log("in: ", inputWorkspaceIndex)
        console.log("out: ", outputWorkspaceIndex)
    }
}
