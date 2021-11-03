import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X

import gnomonQuick     1.0 as GX

Rectangle { id: node;

    property string algorithmClass: "";
    property string algorithmPlugin: "";

    property var inputPorts: [];
    property var outputPorts: [];

    width: 200;
    height: 30 + 15*Math.max(node.inputPorts.length, node.outputPorts.length) - 5;
    radius: 8;

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
        anchors.topMargin: 3
        anchors.leftMargin: 6
        horizontalAlignment: Text.AlignLeft;
    }

    X.Label { id: pluginLabel
        text: node.algorithmPlugin;

        color: "#333333";
        font.pixelSize: 10;

        anchors.fill: parent
        anchors.topMargin: 20
        anchors.rightMargin: 5
        horizontalAlignment: Text.AlignRight;
    }

    Column { id: _input_ports
        spacing: 5;
        anchors.horizontalCenter: parent.left
        anchors.top: parent.top
        anchors.topMargin: 15

        Repeater {
            model: node.inputPorts;
            GX.PipelinePort {
                name: modelData
            }
        }
    }

    Column { id: _output_ports
        spacing: 5;
        anchors.horizontalCenter: parent.right
        anchors.top: parent.top
        anchors.topMargin: 15

        Repeater {
            model: node.outputPorts;
            GX.PipelinePort {
                name: modelData
            }
        }
    }
}