import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtQuick.Layouts  1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X
import xQuick.Style     1.0 as X
import xQuick.Fonts     1.0 as X

import gnomonQuick.Controls     1.0 as G
import gnomon.Visualization     1.0 as GV


X.Dialog {
    id: _self;

    property var node;

    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)

    width: Math.round(window.width / 3 * 2)
    height: Math.round(window.height / 3 * 2)

    parent: Overlay.overlay
    focus: true
    modal: true

    title: "Node info"

    standardButtons: Dialog.Ok | Dialog.Cancel

    GridLayout {
        anchors.fill: parent
        columns: 2

        X.Label {
            text: "Name";
            font {
                pointSize: 14;
                weight: Font.Bold
            }
        }

        X.TextField { id: _name_edit;
            Layout.fillWidth: true
            text: node.name
            wrapMode: TextInput.WrapAnywhere

        }

        X.Label {
            visible: _self.node.inputPortsNames.length > 0

            text: "Inputs";
            font {
                pointSize: 14;
                weight: Font.Bold
            }
        }

        ListView {
            id: input_list

            Layout.fillWidth: true
            Layout.preferredHeight: 42*node.inputPortsNames.length
            visible: node.inputPortsNames.length > 0

            spacing: 6

            model: node.inputPortsNames;

            delegate: G.PipelinePortEdit {
                name: modelData
                label: node.inputPort(modelData).label

                height: 36
                width: output_list.width
            }
        }

        X.Label {
            text: "Outputs";
            visible: _self.node.outputPortsNames.length > 0

            font {
                pointSize: 14;
                weight: Font.Bold
            }
        }
        
        ListView {
            id: output_list

            Layout.fillWidth: true
            Layout.preferredHeight: 42*node.outputPortsNames.length
            visible: node.outputPortsNames.length > 0

            spacing: 6

            model: node.outputPortsNames;

            delegate: G.PipelinePortEdit {
                name: modelData
                label: node.outputPort(modelData).label

                height: 36
                width: output_list.width
            }
        }

        X.Label {
            text: "Description";
            font {
                pointSize: 14;
                weight: Font.Bold
            }
        }

        X.TextField { id: _node_description_edit
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: node.description
            placeholderText: qsTr("Enter node description")
            wrapMode: TextInput.WrapAnywhere
        }

    }

    onAccepted: {
        node.name = _name_edit.text

        for (var input in node.inputPortsNames) {
            var input_name = node.inputPortsNames[input]
            node.inputPort(input_name).label = input_list.itemAtIndex(input).text
            console.log(input_name, ":", form_id)
            if (form_id != -1) {
                GV.World.getDynamicFormMetadata(form_id).set("name", node.inputPort(input_name).label)
            }
        }
        for (var output in node.outputPortsNames) {
            var output_name = node.outputPortsNames[output]
            node.outputPort(output_name).label = output_list.itemAtIndex(output).text
            let form_id = node.outputPort(output_name).formIndex
            if (form_id != -1) {
                GV.World.getDynamicFormMetadata(form_id).set("name", node.outputPort(output_name).label)
            }
        }

        node.description = _node_description_edit.text

        _self.close();
    }
}
