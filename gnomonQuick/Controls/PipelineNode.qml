import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtQuick.Layouts  1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X
import xQuick.Style     1.0 as X
import xQuick.Fonts     1.0 as X

import gnomonQuick.Controls  1.0 as G
import gnomonQuick.Style     1.0 as G
import gnomonQuick.Icons     1.0 as G

import gnomon.Visualization     1.0 as GV

Rectangle {

    id: _self;

    property int workspaceIndex;

    property var node;

    property var inputPorts: new Object();
    property var outputPorts: new Object();

    width: G.Style.smallPanelWidth;
    height: 2*G.Style.smallButtonHeight + G.Style.smallPadding*(2*Math.max(node.inputPortsNames.length, node.outputPortsNames.length) - 1)
    radius: G.Style.panelRadius;

    border.color: G.Style.colors.baseColor;
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

    G.Icon {
        id: _edit_button

        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: G.Style.borderWidth;
        visible: _dragArea.containsMouse

        size: G.Style.iconSmall;
        color: G.Style.colors.textColorDarkNeutral

        icon: G.Icons.icons["pencil"]

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

    Label {
        id: _classLabel
        text: node.name;

        color: G.Style.colors.textColorDarkNeutral
        font: _self.workspaceIndex === window.current_workspace_index() ? G.Style.fonts.nodeHeaderSelected : G.Style.fonts.nodeHeader

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: G.Style.smallPadding
        horizontalAlignment: Text.AlignLeft;
    }

    Label {
        id: _pluginLabel
        text: node.algorithmPlugin;

        color: G.Style.colors.textColorDarkNeutral
        font: _self.workspaceIndex === window.current_workspace_index() ? G.Style.fonts.nodeBodySelected : G.Style.fonts.nodeBody

        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: G.Style.smallPadding
        horizontalAlignment: Text.AlignRight;
    }

    Column {
        spacing: G.Style.smallPadding;
        anchors.horizontalCenter: parent.left
        anchors.top: parent.top
        anchors.topMargin: G.Style.smallButtonHeight;

        Repeater {
            id: _input_ports
            model: node.inputPortsNames;
            G.PipelinePort {
                id: _port
                port: node.inputPort(modelData)
                Component.onCompleted: {
                    let port = node.inputPort(modelData);
                    _self.inputPorts[port] = _input_ports.itemAt(index)
                    let form_id = port.formIndex;
                    if (form_id > -1) {
                        port.label = Qt.binding(function() {
                            return GV.World.getDynamicFormMetadata(form_id).data["name"]
                        })
                    }
                }
            }
        }
    }

    Column {
        spacing: G.Style.smallPadding;
        anchors.horizontalCenter: parent.right
        anchors.top: parent.top
        anchors.topMargin: G.Style.smallButtonHeight;

        Repeater {
            id: _output_ports
            model: node.outputPortsNames;
            G.PipelinePort {
                id: _port
                port: node.outputPort(modelData)
                Component.onCompleted: {
                    let port = node.outputPort(modelData);
                    _self.outputPorts[port] = _output_ports.itemAt(index)
                    let form_id = port.formIndex;
                    if (form_id > -1) {
                        port.label = Qt.binding(function() {
                            return GV.World.getDynamicFormMetadata(form_id).data["name"]
                        })
                    }
                }
            }
        }
    }
}
