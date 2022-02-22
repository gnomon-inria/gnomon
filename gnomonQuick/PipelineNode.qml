import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtQuick.Layouts  1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X
import xQuick.Style     1.0 as X
import xQuick.Fonts     1.0 as X

import gnomonQuick     1.0 as GX
import gnomon.Pipeline  1.0 as GP

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


    X.Dialog {
        id: _node_edit_dialog;

        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)

        width: Math.round(window.width / 3 * 2)

        parent: Overlay.overlay
        focus: true
        modal: true

        title: "Node info"
        GridLayout {

            columns: 2
            rows: 3

            width: parent.width

            X.Label {
                text: "Input";
                font {
                    pointSize: 14;
                    weight: Font.Bold
                }
            }

            X.TextField {
                id: _input_edit;

                placeholderText: qsTr("Enter input name")
                implicitWidth: Math.round(js_info_dialog.width/2)
                implicitHeight: Math.round(js_info_dialog.height/8)
                wrapMode: TextInput.WrapAnywhere
            }

            X.Label {
                text: "Output";
                font {
                    pointSize: 14;
                    weight: Font.Bold
                }
            }

            X.TextField {
                id: _output_edit;
                placeholderText: qsTr("Enter output name")
                implicitWidth: Math.round(js_info_dialog.width/2)
                implicitHeight: Math.round(js_info_dialog.height/8)
                wrapMode: TextInput.WrapAnywhere

            }

            X.Label {
                text: "Description";
                font {
                    pointSize: 14;
                    weight: Font.Bold
                }
            }

            X.TextField {
                id: _node_description_edit
                placeholderText: qsTr("Enter description")
                implicitWidth: Math.round(js_info_dialog.width/2)
                implicitHeight: Math.round(js_info_dialog.height/4)
                wrapMode: TextInput.WrapAnywhere
            }

        }

        footer: X.DialogButtonBox {
            visible: true

            X.ButtonRaw {
                text: "Cancel"
                foregroundColor: X.Style.accentColor
                onClicked: {
                    _node_edit_dialog.close();
                }
            }

            X.ButtonRaw {
                text: "Save"
                foregroundColor: X.Style.accentColor
                onClicked: {
                    GP.Pipeline.setPipeplineInfoForJsonExport(node.name, _input_edit.text, _output_edit.text, _node_description_edit.text);
                    _node_edit_dialog.close();
                }
            }
        }
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

        onClicked: _node_edit_dialog.open();
    }

    X.Label {
        id: _classLabel
        text: node.algorithmClass;

        color: "#333333"; //TODO: get value from theme
        font.pixelSize: 12; //TODO: get value from theme

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
            GX.PipelinePort {
                id: _port
                port: node.inputPort(modelData)
                Component.onCompleted: {
                    _self.inputPorts[port.label] = _input_ports.itemAt(index)
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
            GX.PipelinePort {
                id: _port
                port: node.outputPort(modelData)
                Component.onCompleted: {
                    _self.outputPorts[port.label] = _output_ports.itemAt(index)
                }
            }
        }
    }
}
