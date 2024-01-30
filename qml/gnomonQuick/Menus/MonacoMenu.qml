import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.platform  1.0 as P

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G

import gnomon.Project         1.0 as GP

Item {

    id: _self;

    required property Item editor;
    property var d;
    
    property string _current_file: "";
    property bool _read_only : false

    property string mode : "Python" // "L-Py"


    ColumnLayout {
        anchors.fill: parent;
        anchors.margins: G.Style.smallPadding;

        G.ProjectBrowser {
            id: _project_browser

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            rootDir: GP.ProjectManager.project.currentDir
            dataPath: GP.ProjectManager.project.dataPath

            onFileDoubleClicked: (fileUrl) => {
                let relative_path = GP.ProjectManager.project.relativePath(fileUrl)
                _self._read_only = false
                open_source_file(relative_path)
            }
        }
        
        Item {
            id: _button_container

            height: G.Style.largeButtonHeight
            Layout.fillWidth: true

            G.Button {
                anchors.right: _import_button.left;
                anchors.verticalCenter: _button_container.verticalCenter
                anchors.margins: G.Style.smallPadding;

                text: "Save";

                type: G.Style.ButtonType.Base
                iconName: "content-save"
                empty: true

                onClicked: {
                    if(_self._current_file == "") {
                        _file_dialog_save.open()

                    } else {
                        _message_dialog.open();
                    }
                }
            }

            G.Button {
                id: _import_button

                anchors.right: _button_container.right;
                anchors.verticalCenter: _button_container.verticalCenter
                anchors.margins: G.Style.smallPadding;

                text: "Import";

                type: G.Style.ButtonType.Base
                iconName: "folder-open"
                empty: true

                onClicked: {
                    _file_dialog.open();
                }
            }
        }
    }

    Connections {
        target: d
        function onRequestOpenFile(path) {
            _self.open_source_file(path)
        }
    }

    P.FileDialog {
        id: _file_dialog;

        currentFile: _self._current_file;
        folder: _self._current_file;
        fileMode: P.FileDialog.OpenFile;

        modality: Qt.NonModal;
        nameFilters: _self.mode == "L-Py" ? ["L-Py source files (*.lpy *.py)"] : ["Python source files (*.py)"]

        onAccepted: {
            if (_self.mode == "Python") {
                copy_py_file_to_project.open()
            } else {
                _self._read_only = true
                open_source_file(_file_dialog.file)
            }
        }
    }

    P.FileDialog {
        id: _file_dialog_save

        title: "Save source file"

        folder: _self._current_file;
        fileMode: P.FileDialog.SaveFile

        modality: Qt.WindowModal;
        nameFilters: _self.mode == "L-Py" ? ["L-Py source files (*.lpy *.py)"] : ["Python source files (*.py)"]

        onAccepted: {
            let file_path = decodeURIComponent(_file_dialog_save.file);
            let file_name = file_path.split('/').pop()

            let old_file_name = _self.mode == "Python" ? d.code.fileName : d.fileName;

            if ((old_file_name.split('.').length == 1) || (old_file_name.split('.').pop() == file_name.split('.').pop())) { //same extension
                if (_self.mode == "Python") {
                    d.code.fileName = file_name;
                } else {
                    _editor.tabName = file_name
                }
                d.save(file_path);
                if (_self.mode == "L-Py") {
                    d.fileName = file_name
                }
                _self._current_file = _file_dialog_save.file;
            } else {
                _extension_change_toast.open()
            }

            if (_message_dialog.visible) {
                _message_dialog.close()
            }
        }
    }

    G.Dialog {
        id: _message_dialog;

        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)

        width: Math.round(window.width / 3 * 2)
        height: 200

        parent: Overlay.overlay
        focus: true
        modal: true

        title: "Existing file"

        Label {
            anchors.fill: parent;

            text: "The file already exists, do you want to replace it or save as new ?";
            font: G.Style.fonts.value;
        }

        footer: DialogButtonBox {
            visible: true

            G.Button {
                text: "Cancel"
                type: G.Style.ButtonType.Neutral
                flat: true;

                onClicked: {
                    _message_dialog.close();
                }
            }

            G.Button {
                text: "Replace"
                type: G.Style.ButtonType.Warning
                flat: true;

                onClicked: {
                    d.save(_file_dialog_save.file);
                    _message_dialog.close();
                }
            }

            G.Button {
                text: "Save as"
                type: G.Style.ButtonType.Base
                flat: false;

                onClicked: {
                    _self._current_file = "";
                    _file_dialog_save.open();
                }
            }
        }
    }

    G.Dialog {
        id: copy_py_file_to_project

        simple_dialog : true

        parent: Overlay.overlay
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.smallDialogWidth
        height: G.Style.largeDelegateHeight
        header.height: 0

        modal: true


        Label {
            text: "Copy this file to your project for editing. Otherwise, it remains read-only. \nProceed with copying?"
            font: G.Style.fonts.nodeHeaderSelected
        }

        standardButtons:  Dialog.Yes | Dialog.No

        onAccepted : {
            _self._read_only = false
            open_source_file(_file_dialog.file)
        }

        onRejected : {
            _self._read_only = true
            open_source_file(_file_dialog.file)
        }

    }

    G.Toast {
        id: _extension_change_toast

        parent: Overlay.overlay
        header: "Impossible to change extension"
        message: "You can not save this file using a different extension, please save it as a ." + d.fileName.split('.').pop() + " file."

        type: G.Style.ButtonType.Danger
    }

    G.Toast {
        id: _non_lpy_toast

        parent: Overlay.overlay
        header: "Not a .lpy file"
        message: "The file you opened is not a .lpy file, and can therefore not be run as a LSystem model."

        type: G.Style.ButtonType.Warning
    }

    function open_source_file(path) {
        let file_path = decodeURIComponent(path);
        let file_name = file_path.split('/').pop()
        
        if (_self.mode == "Python") {
            d.code.fileName = file_name;
        }
        
        d.read(file_path, _self._read_only);
        
        if (_self.mode == "L-Py") {
            _self.editor.contents = d.text
        }

        _self._current_file = path;
        _self.editor.language = _self._current_file.endsWith(".lpy") ? "lpy" : "python"

        if (_self.mode == "L-Py") {
            if (!_self._current_file.endsWith(".lpy")) {
                _non_lpy_toast.open()
            }
            if (d.missingTextures.length > 0) {
                _missing_textures_dialog.open()
            }
        }
    }

    G.Dialog {
        id : _missing_textures_dialog

        property var missingTextureFiles: []

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.mediumDialogWidth
        height: G.Style.mediumDialogHeight

        padding: 0;

        parent: Overlay.overlay

        focus: true
        modal: true
        title : "It seems you have some missing textures, please add them"
        Column {
            spacing: G.Style.smallPadding;
            anchors.fill: parent
            anchors.topMargin: G.Style.smallButtonHeight
            Repeater {
                model: d.missingTextures
                RowLayout {
                    Layout.fillWidth: true
                    height: G.Style.mediumLabelHeight
                    spacing: 2
                    Label {
                        id: _texture_label

                        Layout.preferredWidth: G.Style.largeDelegateHeight;
                        Layout.rightMargin: G.Style.largePadding
                        Layout.leftMargin: G.Style.smallPadding
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignTop

                        text: modelData
                        font: G.Style.fonts.formLabel
                        color: G.Style.colors.textColorBase
                    }

                    Rectangle {
                        Layout.alignment: Qt.AlignRight
                        Layout.leftMargin: G.Style.largePadding
                        height: G.Style.mediumLabelHeight
                        implicitWidth: Math.round(2/3 * parent.width)
                        color: G.Style.colors.gutterColor;
                        radius: G.Style.panelRadius

                        G.TextField {
                            id: _texture_file_path

                            anchors.right: _check_texture_icon.left
                            anchors.left: parent.left
                            anchors.bottom: parent.bottom;
                            anchors.top: parent.top
                            anchors.bottomMargin: G.Style.tinyPadding
                            placeholderText: qsTr("Enter texture file path like: /Users/...")
                        }

                        G.Icon {
                            id: _check_texture_icon

                            anchors.right: _edit_texture_button.left
                            anchors.bottom: parent.bottom;
                            anchors.rightMargin: G.Style.smallPadding
                            visible : false
                            color : "green"
                            size: G.Style.iconSmall;
                            icon: "file-check"

                        }

                        G.IconButton {
                            id: _edit_texture_button

                            anchors.right: parent.right
                            anchors.bottom: parent.bottom;
                            anchors.rightMargin: G.Style.smallPadding
                            size: G.Style.iconSmall;
                            iconName: "folder-open"

                            onClicked: {
                                _texture_dialog.open();
                            }
                        }
                    }

                    P.FileDialog {
                        id: _texture_dialog

                        nameFilters: [ "Image files (*.jpg)" ]
                        title: "Open texture file"
                        modality: Qt.WindowModal;
                        fileMode: P.FileDialog.OpenFile

                        onAccepted: {
                            let file_path = Utils.urlToPath(_texture_dialog.file.toString())
                            if(file_path.split("/").slice(-1)[0] === _texture_label.text) {
                                _texture_file_path.text = file_path
                                _check_texture_icon.visible = true
                                _missing_textures_dialog.missingTextureFiles.push(file_path)
                            }
                        }
                    }
                }
            }
        }
        standardButtons:  Dialog.Ok | Dialog.Cancel

        onAccepted : {
            d.copyTexturesFiles(_missing_textures_dialog.missingTextureFiles);
        }
    }
}
