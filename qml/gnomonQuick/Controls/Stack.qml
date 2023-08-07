import QtQuick 2.15
import QtQuick.Shapes 1.15

Item {

    id: stack;

    layer.enabled: true
    layer.samples: 8

    property int padding: 30;
    property int currentIndex: 0;
    property int size: 0;
    property alias count: stack.size;

    function push() {
        stack.size += 1;
    }

    function pop() {
        if (stack.size > 0) {
            stack.size -= 1;

            if (stack.currentIndex > stack.size)
                stack.currentIndex = stack.size;
        }
    }

    Shape {

        id: base;

        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 20;

        height: 100;
        width: 200;

        property int perspective: 30;

        ShapePath { // Floor
            strokeWidth: 0
            strokeColor: "#00000000";
              fillColor: "#bebebe";
            strokeStyle: ShapePath.SolidLine;
              joinStyle: ShapePath.RoundJoin;

            startX: 0; startY: base.height;
            PathLine { x: base.width; y: base.height; }
            PathLine { x: base.width - base.perspective; y: 0; }
            PathLine { x: 0 + base.perspective; y: 0; }
            PathLine { x: 0; y: base.height; }
        }
    }

    Repeater {

        model: stack.size;

        Shape {

            id: item;

            property int   itemIndex: model.index + 1;
            property int   itemHeight: 10;
            property color itemShade: {
                if(_area.containsMouse) {
                    item.itemIndex <= stack.currentIndex ? Qt.darker("#E95420") : Qt.darker("#CCD0A3BF");
                } else {
                    item.itemIndex <= stack.currentIndex ? "#E95420" : "#CCD0A3BF";
                }
            }

            width: 200;
            height: 100;

            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.bottom: parent.bottom;
            anchors.bottomMargin: -5 + stack.padding*item.itemIndex;

            ShapePath { // Ceiling
                strokeWidth: 1
                strokeColor: "#666666";
                  fillColor: item.itemShade;
                strokeStyle: ShapePath.SolidLine;
                  joinStyle: ShapePath.RoundJoin;

                startX: item.width/2;  startY: 0 + 0;
                PathLine { x: 0;            y: 0 + (item.height - item.itemHeight)/2; }
                PathLine { x: item.width/2; y: 0 + (item.height - item.itemHeight);   }
                PathLine { x: item.width;   y: 0 + (item.height - item.itemHeight)/2; }
                PathLine { x: item.width/2; y: 0 + 0; }
            }

            ShapePath { // Left edge

                strokeWidth: 1
                strokeColor: "#666666";
                  fillColor: Qt.lighter(item.itemShade, 1.2);
                strokeStyle: ShapePath.SolidLine;
                  joinStyle: ShapePath.RoundJoin;

                startX: item.width/2; startY: item.itemHeight + (item.height - item.itemHeight);
                PathLine { x: item.width/2; y: 0 + (item.height - item.itemHeight);   }
                PathLine { x: 0;            y: 0 + (item.height - item.itemHeight)/2; }
                PathLine { x: 0;            y: item.itemHeight + (item.height - item.itemHeight)/2; }
                PathLine { x: item.width/2; y: item.itemHeight + (item.height - item.itemHeight); }
            }

            ShapePath { // Right edge

                strokeWidth: 1
                strokeColor: "#666006";
                  fillColor: Qt.darker(item.itemShade, 1.1);
                strokeStyle: ShapePath.SolidLine;
                  joinStyle: ShapePath.RoundJoin;

                startX: item.width/2; startY: item.itemHeight + (item.height - item.itemHeight);
                PathLine { x: item.width/2; y: 0 + (item.height - item.itemHeight);   }
                PathLine { x: item.width;   y: 0 + (item.height - item.itemHeight)/2; }
                PathLine { x: item.width;   y: item.itemHeight + (item.height - item.itemHeight)/2; }
                PathLine { x: item.width/2; y: item.itemHeight + (item.height - item.itemHeight); }
            }

            Rectangle {
                anchors.fill: parent;

                border.width: 1;
                border.color: "blue";

                color: "transparent";

                visible: false;
            }

            MouseArea {
                id: _area;

                anchors.fill: parent;

                hoverEnabled: true;

                onClicked: stack.currentIndex = item.itemIndex;
            }
        }
    }
}
