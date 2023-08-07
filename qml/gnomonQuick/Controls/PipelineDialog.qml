import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls      1.0 as G
import gnomonQuick.Style         1.0 as G
import gnomonQuick.Workspaces    1.0 as G
import "."                       1.0 as G

import gnomon.Pipeline  1.0 as GP

G.Dialog {

    id: _Self
    parent: Overlay.overlay

    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)
    width: G.Style.mediumDialogWidth
    height: G.Style.mediumDialogHeight

    focus: true
    modal: true

    standardButtons: Dialog.Ok | Dialog.Cancel
    title: "Pipeline Info"

    onRejected: {
        _name.text = GP.PipelineManager.pipeline.name
        _description.text = GP.PipelineManager.pipeline.description
    }

    onAccepted: {
        GP.PipelineManager.pipeline.name = _name.text
        GP.PipelineManager.pipeline.description = _description.text
    }

    GridLayout {

        columns: 2
        anchors.fill: parent

        Label {
            Layout.maximumWidth: Math.round(_Self.width/4)
            text: "Pipeline Name";
            font: G.Style.fonts.formLabel
            verticalAlignment: Text.AlignVCenter
        }

        G.TextField {
            id: _name

            Layout.fillWidth: true
            Layout.maximumHeight: implicitHeight
            placeholderText: qsTr("Enter Pipeline Name here")

            text: GP.PipelineManager.pipeline.name
            color: G.Style.colors.textColorBase
            font: G.Style.fonts.cardLabel
            wrapMode: TextInput.WrapAnywhere

            rightPadding: G.Style.smallPadding
            leftPadding: G.Style.smallPadding

            background: Rectangle {
                anchors.fill: parent
                color: G.Style.colors.gutterColor
                radius: G.Style.panelRadius
            }
        }

        Label {
            //Layout.fillWidth: true
            Layout.fillHeight: true
            text: "Description";
            font: G.Style.fonts.formLabel
            verticalAlignment: Text.AlignVCenter
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            G.TextArea {
                id: _description
                placeholderText: qsTr("Enter description here")
                text: GP.PipelineManager.pipeline.description
                wrapMode: TextInput.WrapAnywhere
            }
        }


    }

}
