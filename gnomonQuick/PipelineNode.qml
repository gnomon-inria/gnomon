import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X

import gnomonQuick     1.0 as GX

Rectangle { id: self;

    property string algorithmClass: "";
    property string algorithmPlugin: "";

    property var inputPortsNames: [];
    property var outputPortsNames: [];

    width: 200;
    height: 30 + 15*Math.max(self.inputPortsNames.length, self.outputPortsNames.length) - 5;
    radius: 8;

    Drag.active: _dragArea.drag.active

    MouseArea { id: _dragArea
        anchors.fill: parent
        drag.target: parent
    }

    X.Label { id: _classLabel
        text: self.algorithmClass;

        color: "#333333"; //TODO: get value from theme
        font.pixelSize: 12; //TODO: get value from theme

        anchors.fill: parent
        anchors.topMargin: 3 //TODO: get value from theme
        anchors.leftMargin: 6 //TODO: get value from theme
        horizontalAlignment: Text.AlignLeft;
    }

    X.Label { id: _pluginLabel
        text: self.algorithmPlugin;

        color: "#333333";
        font.pixelSize: 10;

        anchors.fill: parent
        anchors.topMargin: 20
        anchors.rightMargin: 5
        horizontalAlignment: Text.AlignRight;
    }

    property var inputPorts: new Object();

    Column {
        spacing: 5;
        anchors.horizontalCenter: parent.left
        anchors.top: parent.top
        anchors.topMargin: 15

        Repeater { id: _input_ports
            model: self.inputPortsNames;
            GX.PipelinePort { id: _port
                name: modelData
                Component.onCompleted: {
                    self.inputPorts[_port.name] = _input_ports.itemAt(index)
                }
            }
        }
    }

    property var outputPorts: new Object();

    Column {
        spacing: 5;
        anchors.horizontalCenter: parent.right
        anchors.top: parent.top
        anchors.topMargin: 15

        Repeater { id: _output_ports
            model: self.outputPortsNames;
            GX.PipelinePort { id: _port
                name: modelData
                Component.onCompleted: {
                    self.outputPorts[_port.name] = _output_ports.itemAt(index)
                }
            }
        }
    }
}