import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import xQuick.Style       1.0 as X

Shape { id: self;

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
        strokeColor: X.Style.border.Color

        startX: self.stt.x;
        startY: self.stt.y;

        PathCubic {
            control1X: self.end.x - self.delt;
            control1Y: self.stt.y;
            control2X: self.stt.x + self.delt;
            control2Y: self.end.y;
                    x: self.end.x;
                    y: self.end.y
        }
    }
}