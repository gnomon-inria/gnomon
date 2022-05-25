import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import xQuick.Style       1.0 as X

import gnomonQuick.Style  1.0 as G

Rectangle {

    id: _self

    property var port;

    property bool highlighted: port ? port.formIndex === window.world.currentIndex : false;

    color: _self.highlighted
        ? G.Style.colors.highlightColor
        : "#CCCCCC"
    height: 10;
    width: 10;
    radius: 5;

    X.ToolTip {
         text: port.name + ": " + port.label
         visible: _mouse_area.containsMouse
    }

    MouseArea {
        id: _mouse_area;
        anchors.fill: parent;
        hoverEnabled: true;

        onDoubleClicked: {
            if(port.formIndex >= 0) {
                window.world.currentIndex = port.formIndex
            }
        }
    }
}
