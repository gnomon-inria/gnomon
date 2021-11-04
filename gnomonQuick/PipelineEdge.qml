import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

Shape { id: _edge;

    property Item src;
    property Item tgt;

    property point stt: Qt.point((src.x + src.width), (src.y + src.height/2));
    property point end: Qt.point( tgt.x,              (tgt.y + tgt.height/2));
    property point mid: Qt.point((stt.x + end.x)/2,   (stt.y + end.y)/2);

    property real delt: (mid.x - stt.x)/2;

     width: end.x - stt.x;
    height: end.y - stt.y;

    ShapePath {
        fillColor: "transparent";

        strokeWidth: 3
        strokeColor: "red"

        startX: _edge.stt.x;
        startY: _edge.stt.y;

        PathCubic {
            control1X: _edge.end.x - _edge.delt;
            control1Y: _edge.stt.y;
            control2X: _edge.stt.x + _edge.delt;
            control2Y: _edge.end.y;
                    x: _edge.end.x;
                    y: _edge.end.y
        }
    }
}