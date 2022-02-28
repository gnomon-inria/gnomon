import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtQuick.Layouts  1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X
import xQuick.Style     1.0 as X
import xQuick.Fonts     1.0 as X

import gnomonQuick.Controls     1.0 as G

Rectangle {

    id: _self;

    property int workspaceIndex;

    property var node;

    property var inputPorts: new Object();
    property var outputPorts: new Object();

    width: 200;
    height: 30 + 15*Math.max(node.inputPortsNames.length, node.outputPortsNames.length) - 5;
    radius: 8;

    border.color: X.Style.accentColor;
    border.width: _self.workspaceIndex === window.current_workspace_index() ? 2 : 0;

    opacity: _dragArea.containsMouse ? 0.8 : 1.0

    Drag.active: _dragArea.drag.active

    onXChanged: {
        if (_self.x != _canvas.width/2 + node.position.x) {
            node.position = Qt.point(_self.x - _canvas.width/2, (_self.y - _canvas.height/2)/0.33)
        }
    }

    onYChanged: {
        if (_self.y != _canvas.height/2 + 0.33*node.position.y) {
            node.position = Qt.point(_self.x - _canvas.width/2, (_self.y - _canvas.height/2)/0.33)
        }
    }

    X.Icon {
        id: _edit_button

        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 3;
        visible: _dragArea.containsMouse

        size: 15;
        color: "#333333"; //TODO: get value from theme

        icon: X.Icons.icons.edit

    }

    ToolTip {
         text: node.description
         visible: (node.description != "") && _dragArea.containsMouse
    }

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

    MouseArea {
        id: _save_area;

        anchors.fill: _edit_button;

        onClicked: {
            var node_dialog_component = Qt.createComponent("PipelineNodeDialog.qml");
            if (node_dialog_component.status == Component.Ready) {
                var dialog = node_dialog_component.createObject(_self, {
                    "node": _self.node,
                });
                dialog.open()
            }
        }
    }

    X.Label {
        id: _classLabel
        text: node.name;

        color: "#333333"; //TODO: get value from theme
        font.pixelSize: 12; //TODO: get value from theme
        font.bold: _self.workspaceIndex === window.current_workspace_index()

        anchors.fill: parent
        anchors.topMargin: 3 //TODO: get value from theme
        anchors.leftMargin: 6 //TODO: get value from theme
        horizontalAlignment: Text.AlignLeft;
    }

    X.Label {
        id: _pluginLabel
        text: node.algorithmPlugin;

        color: "#333333";
        font.pixelSize: 10;
        font.bold: _self.workspaceIndex === window.current_workspace_index()

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
            model: node.inputPortsNames;
            G.PipelinePort {
                id: _port
                port: node.inputPort(modelData)
                Component.onCompleted: {
                    _self.inputPorts[node.inputPort(modelData)] = _input_ports.itemAt(index)
                }
            }
        }
    }

    Column {
        spacing: 5;
        anchors.horizontalCenter: parent.right
        anchors.top: parent.top
        anchors.topMargin: 15

        Repeater {
            id: _output_ports
            model: node.outputPortsNames;
            G.PipelinePort {
                id: _port
                port: node.outputPort(modelData)
                Component.onCompleted: {
                    _self.outputPorts[node.outputPort(modelData)] = _output_ports.itemAt(index)
                }
            }
        }
    }
}
