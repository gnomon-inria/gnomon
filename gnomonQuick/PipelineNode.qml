import QtQuick 2.15
import QtQuick.Shapes 1.15

Rectangle {
    width: 100;
    height: 48;
    radius: 6;
    color: "#E4A065";

    Drag.active: dragArea.drag.active

    MouseArea {
        id: dragArea
        anchors.fill: parent

        drag.target: parent
    }
}