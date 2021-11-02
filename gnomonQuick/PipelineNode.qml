import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X

Rectangle { id: node;

    width: 160;
    height: 40;
    radius: 8;
    color: "#E4A065";

    property string algorithmClass: "";
    property string algorithmPlugin: "";

    Drag.active: dragArea.drag.active

    MouseArea { id: dragArea
        anchors.fill: parent
        drag.target: parent
    }

    X.Label { id: classLabel
        text: node.algorithmClass;

        color: "#333333";
        font.pixelSize: 12;

        anchors.fill: parent
        anchors.leftMargin: 10
        horizontalAlignment: Text.AlignLeft;
    }

    X.Label { id: pluginLabel
        text: node.algorithmPlugin;

        color: "#333333";
        font.pixelSize: 10;

        anchors.fill: parent
        anchors.topMargin: 16
        anchors.rightMargin: 10
        horizontalAlignment: Text.AlignRight;
    }
}