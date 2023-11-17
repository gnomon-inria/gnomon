import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import gnomonQuick.Style  1.0 as G
import gnomonQuick.Controls  1.0 as G

Shape {

    id: _self;

    property var edge;

    property Item src;
    property Item tgt;
    property Item src_component;
    property Item tgt_component;

    property int inputWorkspaceIndex;
    property int outputWorkspaceIndex;
    property int formIndex: edge ? edge.formIndex : -1;

    property point stt: Qt.point(
        (src_component.x + src.parent.x + src.x + src.width),
        (src_component.y + src.parent.y + src.y + src.height / 2)
    );
    property point end: Qt.point(
        (tgt_component.x + tgt.parent.x + tgt.x),
        (tgt_component.y + tgt.parent.y + tgt.y + tgt.height/2)
    );
    property point mid: Qt.point((stt.x + end.x)/2,   (stt.y + end.y)/2);

    property real delt: (mid.x - stt.x)/2;

    width: end.x - stt.x;
    height: end.y - stt.y;

    containsMode: Shape.FillContains

    ShapePath {
        fillColor: "transparent";

        strokeWidth: G.Style.borderWidth
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


    G.Thumbnail {
        id: _thumbnail

        x: _self.stt.x + _self.width / 2 -_thumbnail.width /2
        y: _self.stt.y + _self.height / 2 -_thumbnail.height /2

        bgColor: window.world.currentIndex === _self.formIndex
                ? G.Style.colors.highlightColor
                : G.Style.colors.neutralColor
        url: "image://thumbnails/" + _self.formIndex
        onDoubleClicked: window.world.currentIndex = _self.formIndex;
    }

    // Control {

    //     id: _thumbnail

    //     x: _self.stt.x + _self.width / 2 -_thumbnail.width /2
    //     y: _self.stt.y + _self.height / 2 -_thumbnail.height /2

    //     height: G.Style.thumbnailLarge + _thumbnail.padding
    //     width: _thumbnail.height
    //     padding: G.Style.tinyPadding

    //     background: Rectangle {
    //         color: window.world.currentIndex === _self.formIndex
    //             ? G.Style.colors.highlightColor
    //             : G.Style.colors.neutralColor
    //         radius: G.Style.borderWidth;
    //     }

    //     contentItem: Image {
    //         fillMode: Image.PreserveAspectFit;
    //         source: "image://thumbnails/" + _self.formIndex
    //     }

    //     MouseArea {
    //         id: _form_selector_area

    //         anchors.fill: parent

    //         onDoubleClicked: window.world.currentIndex = _self.formIndex;
    //     }
    // }

}
