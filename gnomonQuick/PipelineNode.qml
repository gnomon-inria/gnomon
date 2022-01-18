import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X
import xQuick.Style     1.0 as X

import gnomonQuick     1.0 as GX

Rectangle {

    id: _self;

    property int workspaceIndex;

    property string algorithmClass: "";
    property string algorithmPlugin: "";

    property var inputPortsNames: [];
    property var outputPortsNames: [];
    property var inputPorts: new Object();

    width: 200;
    height: 30 + 15*Math.max(_self.inputPortsNames.length, _self.outputPortsNames.length) - 5;
    radius: 8;

    border.color: X.Style.accentColor;
    border.width: _self.workspaceIndex === window.current_workspace_index() ? 2 : 0;

    opacity: _dragArea.containsMouse ? 0.8 : 1.0

    Drag.active: _dragArea.drag.active


    MouseArea {
        id: _dragArea
        anchors.fill: parent
        drag.target: parent
        hoverEnabled: true

        onDoubleClicked: {
            console.log("Switching to workspace ", _self.workspaceIndex)
            window.switch_workspace(_self.workspaceIndex)
        }

    }

    X.Label {
        id: _classLabel
        text: _self.algorithmClass;

        color: "#333333"; //TODO: get value from theme
        font.pixelSize: 12; //TODO: get value from theme

        anchors.fill: parent
        anchors.topMargin: 3 //TODO: get value from theme
        anchors.leftMargin: 6 //TODO: get value from theme
        horizontalAlignment: Text.AlignLeft;
    }

    X.Label {
        id: _pluginLabel
        text: _self.algorithmPlugin;

        color: "#333333";
        font.pixelSize: 10;

        anchors.fill: parent
        anchors.topMargin: 20
        anchors.rightMargin: 5
        horizontalAlignment: Text.AlignRight;
    }

    Column {
        spacing: 5;
        anchors.horizontalCenter: parent.left
        anchors.top: parent.top
        anchors.topMargin: 15

        Repeater {
            id: _input_ports
            model: _self.inputPortsNames;
            GX.PipelinePort {
                id: _port
                name: modelData
                highlighted: _self.workspaceIndex === window.current_workspace_index()
                Component.onCompleted: {
                    _self.inputPorts[_port.name] = _input_ports.itemAt(index)
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

        Repeater {
            id: _output_ports
            model: _self.outputPortsNames;
            GX.PipelinePort {
                id: _port
                name: modelData
                highlighted: _self.workspaceIndex === window.current_workspace_index()
                Component.onCompleted: {
                    _self.outputPorts[_port.name] = _output_ports.itemAt(index)
                    console.log(_self.workspaceIndex, window.current_workspace_index())
                }
            }
        }

    }
}
