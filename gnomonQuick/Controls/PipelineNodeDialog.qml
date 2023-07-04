import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls 1.0 as G
import gnomonQuick.Style    1.0 as G
import gnomon.Visualization 1.0 as GV


G.Dialog {
    id: _self;

    property var node;

    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)

    width: G.Style.largeDialogWidth
    height: G.Style.largeDialogHeight

    parent: Overlay.overlay
    focus: true
    modal: true
    //clip: true

    title: "Node info"

    standardButtons: Dialog.Ok | Dialog.Cancel


    Control {
        id: _left_panel
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: _separator.left
        anchors.rightMargin: G.Style.mediumPadding

        ColumnLayout {
            anchors.fill: parent

            Label {
                Layout.fillWidth: true
                text: "Name";
                font: G.Style.fonts.formLabel
                verticalAlignment: Text.AlignBottom
                horizontalAlignment: Text.AlignLeft
            }

            TextField { id: _name_edit;
                Layout.fillWidth: true
                text: node.name
                wrapMode: TextInput.WrapAnywhere
                font: G.Style.fonts.value
            }

            Label {
                Layout.fillWidth: true
                text: "Description";
                font: G.Style.fonts.formLabel
                verticalAlignment: Text.AlignBottom
                horizontalAlignment: Text.AlignLeft
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                G.TextArea {
                    id: _node_description_edit
                    text: node.description
                    placeholderText: qsTr("Enter node description")
                    wrapMode: TextInput.WrapAnywhere
                }
            }
        }
    }

    Rectangle {
        id: _separator
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        //anchors.margins: G.Style.smallPadding
        x: parent.x + Math.round(parent.width/2)
        width: 2
        color: G.Style.colors.embossColor
    }

    Control {
        id: _rigth_panel
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: _separator.right
        anchors.right: parent.right
        anchors.leftMargin: G.Style.mediumPadding

        ColumnLayout {
            anchors.fill: parent

            Label {
                //visible: _self.node.inputPortsNames.length > 0

                text: "Inputs";
                font: G.Style.fonts.formLabel
                Layout.fillWidth: true
                verticalAlignment: Text.AlignBottom
                horizontalAlignment: Text.AlignLeft
            }

            G.Gutter {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ListView {
                    id: input_list
                    anchors.fill: parent
                    //visible: node.inputPortsNames.length > 0

                    //spacing: 6

                    model: node.inputPortsNames;

                    delegate: G.PipelinePortEdit {
                        name: modelData
                        label: node.inputPort(modelData).label

                        //height: 36
                        width: output_list.width
                    }
                }
            }


            Label {
                text: "Outputs";
                //visible: _self.node.outputPortsNames.length > 0

                font: G.Style.fonts.formLabel
                Layout.fillWidth: true
                verticalAlignment: Text.AlignBottom
                horizontalAlignment: Text.AlignLeft
            }

            G.Gutter {
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: true
                ListView {
                    id: output_list
                    anchors.fill: parent
                    //visible: node.outputPortsNames.length > 0

                    //spacing: 6

                    model: node.outputPortsNames;

                    delegate: G.PipelinePortEdit {
                        name: modelData
                        label: node.outputPort(modelData).label

                        //height: 36
                        width: output_list.width
                    }
                }
            }
        }
    }


    onAccepted: {
        node.name = _name_edit.text

        for (var input in node.inputPortsNames) {
            var input_name = node.inputPortsNames[input]
            let form_id = node.inputPort(input_name).formIndex
            if (form_id != -1) {
                GV.World.getDynamicFormMetadata(form_id).set("name", input_list.itemAtIndex(input).form_name)
            } else {
                node.inputPort(input_name).label = input_list.itemAtIndex(input).form_name
            }
        }
        for (var output in node.outputPortsNames) {
            var output_name = node.outputPortsNames[output]
            let form_id = node.outputPort(output_name).formIndex
            if (form_id != -1) {
                GV.World.getDynamicFormMetadata(form_id).set("name", output_list.itemAtIndex(output).form_name)
            } else {
                node.outputPort(output_name).label = output_list.itemAtIndex(output).form_name
            }
        }

        node.description = _node_description_edit.text

        _self.close();
    }
}
