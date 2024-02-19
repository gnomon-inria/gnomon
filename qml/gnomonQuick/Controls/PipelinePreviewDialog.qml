import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import gnomon.Utils

import Qt.labs.platform as P

import gnomonQuick.Controls   as G
import gnomonQuick.Style      as G
import gnomon.Pipeline   as GP
import gnomon.Project    as GP


G.Dialog {
    id: _self

    property string pipelineFile: ""
    property alias pipeline: _pipeline

    modal: true

    parent: Overlay.overlay
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: G.Style.largeDialogWidth
    height: G.Style.hugeDialogHeight

    title: "Run pipeline"

    GP.Pipeline {
        id: _pipeline
    }

    G.Pipeline {
        id: _pipeline_view

        pipeline: _self.pipeline
        edgeThumbnails: false

        anchors.left: parent.left
        anchors.right: parent.right
        height: G.Style.largeDelegateHeight
    }

    onPipelineFileChanged: {
        _self.pipeline.readFromJson(_self.pipelineFile)
    }

    onClosed: {
        _self.pipeline.clear()
    }

    standardButtons: Dialog.Ok | Dialog.Cancel
}