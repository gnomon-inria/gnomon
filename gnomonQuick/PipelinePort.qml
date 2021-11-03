import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

Rectangle { id: port
    color: "#CCCCCC"
    height: 10;
    width: 10;
    radius: 5;

    property string name: "";

    ToolTip {
         text: port.name
         visible: _mouse_area.containsMouse
    }

    MouseArea { id: _mouse_area;
        anchors.fill: parent;
        hoverEnabled: true;
    }
}