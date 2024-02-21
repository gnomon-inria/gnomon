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
    property string projectSource: ""
    property alias pipeline: _pipeline

    modal: true

    parent: Overlay.overlay
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: G.Style.largeDialogWidth
    height: G.Style.largeDialogHeight

    title: "Load session by running pipeline"

    GP.Pipeline {
        id: _pipeline
    }

    G.Pipeline {
        id: _pipeline_view

        pipeline: _self.pipeline
        readOnly: true
        edgeThumbnails: false

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: -2*G.Style.smallPadding
        height: G.Style.largeDelegateHeight + 2*G.Style.smallPadding
    }

    onPipelineFileChanged: {
        if (_self.pipelineFile != "") {
            _self.pipeline.readFromJson(_self.pipelineFile)
            if (_self.pipeline.name == "") {
                let filename = _self.pipelineFile.split('/').pop()
                _self.pipeline.name = filename.slice(0, -5)
            }
        }
    }

    Rectangle {
        id: _separator

        anchors.top: _pipeline_view.bottom
        anchors.bottom: parent.bottom
        anchors.horizontalCenter:  parent.horizontalCenter
        anchors.topMargin: G.Style.smallPadding
        anchors.bottomMargin: -2*G.Style.smallPadding
        width: 2

        color: G.Style.colors.embossColor
    }
    
    Label {
        id: _inputs_label

        text: "Pipeline Inputs"

        anchors.top: _pipeline_view.bottom
        anchors.left: parent.left
        anchors.right: _separator.left
        anchors.margins: G.Style.smallPadding
        anchors.bottomMargin: 0
        anchors.leftMargin: -2*G.Style.smallPadding

        color: G.Style.colors.textColorNeutral
        font: G.Style.fonts.formLabel
    }

    ListView {
        id: _input_list

        anchors.top: _inputs_label.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: _separator.left

        anchors.margins: G.Style.smallPadding
        anchors.leftMargin: -2*G.Style.smallPadding

        model: _pipeline.inputNodeNames

        delegate: ItemDelegate {

            width: _input_list.width;
            height: _input_name_label.height + G.Style.mediumLabelHeight + 2*G.Style.smallPadding

            Label {
                id: _input_name_label

                text: modelData

                anchors.left: parent.left
                anchors.margins: G.Style.smallPadding
                anchors.verticalCenter: parent.verticalCenter
                width: G.Style.largeDelegateHeight;

                color: G.Style.colors.textColorNeutral
                font: G.Style.fonts.subHeader
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: _input_name_label.bottom

                height: G.Style.mediumLabelHeight

                color: G.Style.colors.gutterColor;
                radius: G.Style.panelRadius

                G.TextField {
                    id: _input_file_path

                    property var isValid: checkFileExistence(_input_file_path.text)

                    anchors.right: _check_input_path_icon.left
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom;
                    anchors.top: parent.top
                    anchors.bottomMargin: G.Style.tinyPadding

                    backgroundHighlightColor: isValid? G.Style.colors.okColor : G.Style.colors.dangerColor

                    text: _pipeline.inputNodePath(modelData)
                    font: G.Style.fonts.value
                }

                G.Icon {
                    id: _check_input_path_icon

                    anchors.right: _edit_input_path_button.left
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.rightMargin: G.Style.smallPadding

                    color: _input_file_path.isValid? G.Style.colors.okColor : G.Style.colors.dangerColor
                    size: G.Style.iconSmall;
                    icon: _input_file_path.isValid? "file-check" : "file-alert"
                }

                G.IconButton {
                    id: _edit_input_path_button

                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.rightMargin: G.Style.smallPadding
                    size: G.Style.iconSmall;
                    iconName: "folder-open"

                    onClicked: {
                        let path = GUtils.findProjectFile(_input_file_path.text.split(',')[0], _self.projectSource)
                        console.log(path)
                        let parent_folder = path.split("/").slice(0, -1).join("/")
                        _file_dialog.folder = "file://"+parent_folder
                        _file_dialog.currentFile = "file://"+path
                        _file_dialog.open()
                    }
                }

                P.FileDialog {
                    id: _file_dialog;

                    folder: _self.projectSource
                    fileMode: P.FileDialog.OpenFiles;

                    modality: Qt.NonModal;

                    onAccepted: {
                        let urls = _file_dialog.files
                        let relative_paths = []
                        for(let i_n in urls) {
                            let path = decodeURIComponent(urls[i_n]).slice(7)
                            let relative_path = GUtils.projectRelativePath(path, _self.projectSource)
                            console.log(relative_path)
                            if (relative_path != "") {
                                relative_paths.push(relative_path)
                            }
                        }
                        _input_file_path.text = relative_paths.join(',');
                    }
                }
            }
        }
    }
    
    Label {
        id: _outputs_label

        text: "Pipeline Outputs"

        anchors.top: _pipeline_view.bottom
        anchors.left: _separator.right
        anchors.right: parent.right
        anchors.margins: G.Style.smallPadding
        anchors.bottomMargin: 0
        anchors.rightMargin: -G.Style.smallPadding

        color: G.Style.colors.textColorNeutral
        font: G.Style.fonts.formLabel
    }

    ListView {
        id: _output_list

        anchors.top: _outputs_label.bottom
        anchors.bottom: parent.bottom
        anchors.left: _separator.right
        anchors.right: parent.right

        anchors.margins: G.Style.smallPadding
        anchors.rightMargin: -2*G.Style.smallPadding

        model: _pipeline.outputNodeNames

        delegate: ItemDelegate {

            width: _output_list.width;
            height: _output_name_label.height + G.Style.mediumLabelHeight + 2*G.Style.smallPadding

            Label {
                id: _output_name_label

                text: modelData

                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: G.Style.smallPadding
                width: G.Style.largeDelegateHeight;

                color: G.Style.colors.textColorNeutral
                font: G.Style.fonts.subHeader
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: _output_name_label.bottom

                height: G.Style.mediumLabelHeight

                color: G.Style.colors.gutterColor;
                radius: G.Style.panelRadius

                G.TextField {
                    id: _output_file_path

                    property var isValid: checkParentFolderExistence(_output_file_path.text)
                    property var overwrites: checkFileExistence(_output_file_path.text)

                    anchors.right: _check_output_path_icon.left
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom;
                    anchors.top: parent.top
                    anchors.bottomMargin: G.Style.tinyPadding

                    backgroundHighlightColor: isValid? (overwrites? G.Style.colors.warningColor : G.Style.colors.okColor) : G.Style.colors.dangerColor

                    text: _pipeline.outputNodePath(modelData)
                    font: G.Style.fonts.value
                }

                G.Icon {
                    id: _check_output_path_icon

                    anchors.right: _edit_output_path_button.left
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.rightMargin: G.Style.smallPadding

                    color: _output_file_path.isValid? (_output_file_path.overwrites? G.Style.colors.warningColor : G.Style.colors.okColor) : G.Style.colors.dangerColor
                    size: G.Style.iconSmall;
                    icon: _output_file_path.isValid & !_output_file_path.overwrites? "file-check" : "file-alert"
                }

                G.IconButton {
                    id: _edit_output_path_button

                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.rightMargin: G.Style.smallPadding
                    size: G.Style.iconSmall;
                    iconName: "folder-open"

                    onClicked: {
                        let path = GUtils.findProjectFile(_output_file_path.text.split(',')[0], _self.projectSource)
                        let parent_folder = path.split("/").slice(0, -1).join("/")
                        _file_dialog_save.folder = "file://"+parent_folder
                        _file_dialog_save.currentFile = "file://"+path
                        _file_dialog_save.open()
                    }
                }

                P.FileDialog {
                    id: _file_dialog_save;

                    folder: _self.projectSource
                    fileMode: P.FileDialog.SaveFile;

                    modality: Qt.NonModal;

                    onAccepted: {
                        let urls = _file_dialog_save.files
                        let relative_paths = []
                        for(let i_n in urls) {
                            let path = decodeURIComponent(urls[i_n]).slice(7)
                            let relative_path = GUtils.projectRelativePath(path, _self.projectSource)
                            if (relative_path != "") {
                                relative_paths.push(relative_path)
                            }
                        }
                        _output_file_path.text = relative_paths.join(',');
                    }
                }
            }
        }
    }

    function checkFileExistence(path) {
        let paths = path.split(",")
        let exists = true;
        for (var i in paths) {
            exists = exists & GUtils.fileBelongsToProject(paths[i], _self.projectSource)
        }
        return exists
    }

    function checkParentFolderExistence(path) {
        let paths = path.split(",")
        let exists = true;
        for (var i in paths) {
            let parent_folder = paths[i].split("/").slice(0, -1).join("/")
            exists = exists & GUtils.fileBelongsToProject(parent_folder, _self.projectSource)
        }
        return exists
    }

    onClosed: {
        _self.pipeline.clear()
        _self.pipelineFile = ""
    }

    onOpened: {
        _pipeline_view.zoomLevel = -4;
    }

    footer: DialogButtonBox {
        alignment: Qt.AlignRight
        spacing: G.Style.smallPadding

        background: Rectangle {
            anchors.fill: parent
            color: G.Style.colors.gutterColor
        }

        G.Button {
            text: 'Cancel';
            flat: true
            type: G.Style.ButtonType.Neutral
            width: G.Style.buttonWidth
            onClicked: _self.reject();
        }

        G.Button {
            text: 'Run';
            iconName: "play"
            type: G.Style.ButtonType.OK
            width: G.Style.buttonWidth
            onClicked: _self.accept();
        }
    }
}