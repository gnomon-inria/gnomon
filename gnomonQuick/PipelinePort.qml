import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import xQuick.Style       1.0 as X

Rectangle {

    id: _self

    property string name: "";
    property bool highlighted: false;

    color: _self.highlighted ? X.Style.accentColor : "#CCCCCC"
    height: 10;
    width: 10;
    radius: 5;

    ToolTip {
         text: _self.name
         visible: _mouse_area.containsMouse
    }

    MouseArea {
        id: _mouse_area;
        anchors.fill: parent;
        hoverEnabled: true;
    }

}
