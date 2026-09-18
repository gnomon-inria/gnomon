import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls  1.0 as G
import gnomonQuick.Style     1.0 as G

import gnomon.Visualization     1.0 as GV

Rectangle {

    id: _self;

    property int workspaceIndex;

    property var node;
    property bool interactive: true

    property var inputPorts: new Object();
    property var outputPorts: new Object();

    width: G.Style.smallPanelWidth + _self.height;
    height: 3*G.Style.smallButtonHeight + G.Style.smallPadding*(2*Math.max(node.data.inputPortsNames.length, node.data.outputPortsNames.length) - 1)
    radius: G.Style.panelRadius;

    border.color: G.Style.colors.baseColor;
    border.width: _self.workspaceIndex === window.current_workspace_index() ? 2 : 0;

    opacity: _self.interactive & _dragArea.containsMouse ? 0.8 : 1.0

    Drag.active: _dragArea.drag.active

    Behavior on x {
        enabled: !_dragArea.drag.active
        NumberAnimation {
            duration: 500
        }
    }

    Behavior on y {
        enabled: !_dragArea.drag.active
        NumberAnimation {
            duration: 500
        }
    }

    G.SessionSettings {
        category: "pipeline_nodes__"+node.data.name
        property alias x: _self.x
        property alias y: _self.y
    }

    // onXChanged: {
    //     if (_self.x != _canvas.width/2 + node.data.position.x) {
    //         node.data.position = Qt.point(_self.x - _canvas.width/2, (_self.y - _canvas.height/2)/0.33)
    //     }
    // }

    // onYChanged: {
    //     if (_self.y != _canvas.height/2 + 0.33*node.data.position.y) {
    //         node.data.position = Qt.point(_self.x - _canvas.width/2, (_self.y - _canvas.height/2)/0.33)
    //     }
    // }

    G.Icon {
        id: _edit_button

        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: G.Style.borderWidth;
        visible: _self.interactive & _dragArea.containsMouse

        size: G.Style.iconSmall;
        color: G.Style.colors.textColorDarkNeutral

        icon: "pencil"

    }

    G.ToolTip {
         text: node.data.description
         visible: (node.data.description != "") && _dragArea.containsMouse
    }

    MouseArea {
        id: _dragArea
        anchors.fill: parent
        drag.target: parent
        hoverEnabled: true

        onDoubleClicked: {
            if (_self.interactive) {
                console.log("Switching to workspace ", _self.workspaceIndex)
                window.switch_workspace(_self.workspaceIndex)
            }
        }
    }

    MouseArea {
        id: _save_area;

        anchors.fill: _edit_button;

        G.PipelineNodeDialog {
            id: _test_dummy
            node: _self.node.data
        }

        onClicked: {
            if (_self.interactive) {
                var node_dialog_component = Qt.createComponent("PipelineNodeDialog.qml");
                if (node_dialog_component.status == Component.Ready || false) {
                    var dialog = node_dialog_component.createObject(_self, {
                        "node": _self.node.data,
                    });
                    _test_dummy.open()
                }
            }
        }
    }

    Label {
        id: _classLabel
        text: node.data.name;

        color: G.Style.colors.textColorDarkNeutral
        font: _self.workspaceIndex === window.current_workspace_index() ? G.Style.fonts.nodeHeaderSelected : G.Style.fonts.nodeHeader

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: G.Style.smallPadding
        horizontalAlignment: Text.AlignLeft;
    }

    Label {
        id: _pluginLabel
        text: node.data.algorithmPlugin;

        color: G.Style.colors.textColorDarkNeutral
        font: _self.workspaceIndex === window.current_workspace_index() ? G.Style.fonts.nodeBodySelected : G.Style.fonts.nodeBody

        anchors.bottom: parent.bottom
        anchors.left: parent.left
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
            model: node.data.inputPortsNames;
            G.PipelinePort {
                id: _port
                port: node.data.inputPort(modelData)
                Component.onCompleted: {
                    let port = node.data.inputPort(modelData);
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
            model: node.data.outputPortsNames;
            G.PipelinePort {
                id: _port
                port: node.data.outputPort(modelData)
                Component.onCompleted: {
                    let port = node.data.outputPort(modelData);
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
