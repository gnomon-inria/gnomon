import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.platform  1.0 as P

import gnomon.Utils
import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G

import gnomon.Project         1.0 as GP

import "../Controls/utils.js" as Utils

Item {

    id: _self;

    required property Item editor;
    property var d;
    
    property string _current_file: "";
    property bool _read_only : false

    property string mode : "Python" // "L-Py"


    Label {
        id: _label

        anchors.top: parent.top
        anchors.left: parent.left

        text: "Project Files"
        font: G.Style.fonts.header
        color: G.Style.colors.textColorBase
    }

    G.ProjectBrowser {
        id: _project_browser

        anchors.topMargin: G.Style.smallPadding
        anchors.bottomMargin: G.Style.smallPadding
        anchors.top: _label.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: _button_container.top;

        rootDir: GP.ProjectManager.project.currentDir
        dataPath: GP.ProjectManager.project.dataPath

        onFileDoubleClicked: (fileUrl) => {
            let relative_path = GP.ProjectManager.project.relativePath(fileUrl)
            let extension_ok = relative_path.endsWith(".py");
            if (_self.mode == "L-Py") {
                extension_ok = extension_ok | relative_path.endsWith(".lpy")
            }
            if (extension_ok) {
                _self._read_only = GP.ProjectManager.project.isReadOnly(relative_path)
                open_source_file(relative_path)
            } else {
                _wrong_extension_toast.open()
            }
        }

        Connections {
            target: GP.ProjectManager.project
            function onFileImported(file_path) {
                console.log(file_path)
                _project_browser.projectTree.requestFileSelection(file_path)
            }
        }
    }

    Item {
        id: _button_container

        anchors.right: parent.right;
        anchors.left: parent.left;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: G.Style.smallPadding
        height: G.Style.largeButtonHeight

        G.Button {
            anchors.right: _import_button.left;
            anchors.verticalCenter: _button_container.verticalCenter
            anchors.margins: G.Style.smallPadding;

            text: "Save";

            type: G.Style.ButtonType.Base
            flat: true

            onClicked: {
                let file_name = d.fileName;
                let default_file_name = _self.mode == "Python" ? "example.py" : "vonKoch.lpy";
                if(file_name == default_file_name) {
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
            anchors.margins: G.Style.smallPadding

            text: "Open";

            type: G.Style.ButtonType.Base

            onClicked: {
                _file_dialog.open();
            }
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
            _self._read_only = GP.ProjectManager.project.isReadOnly(_file_dialog.file)
            if(_self._read_only) {
                _read_only_toast.open()
            }
            open_source_file(_file_dialog.file)
        }
    }

    P.FileDialog {
        id: _file_dialog_save

        title: "Save source file"

        folder: "file://"+GP.ProjectManager.project.currentDir
        fileMode: P.FileDialog.SaveFile

        modality: Qt.WindowModal;
        nameFilters: _self.mode == "L-Py" ? ["L-Py source files (*.lpy *.py)"] : ["Python source files (*.py)"]

        onAccepted: {
            let file_path = decodeURIComponent(_file_dialog_save.file);
            let file_name = file_path.split('/').pop()

            let old_file_name = d.fileName;

            if ((old_file_name.split('.').length === 1) || (old_file_name.split('.').pop() === file_name.split('.').pop())) { //same extension
                d.save(file_path);
                if (_self.mode === "Python") {
                    editor.tabName = file_name;
                    d.fileName = file_name;
                } else {
                    editor.tabName = file_name
                }
                if (_self.mode === "L-Py") {
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

        width: G.Style.smallDialogWidth;
        height: G.Style.smallDialogHeight;

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
            alignment: Qt.AlignRight
            spacing: G.Style.smallPadding

            G.Button {
                text: "Cancel"
                type: G.Style.ButtonType.Neutral
                width: G.Style.buttonWidth
                flat: true;

                onClicked: {
                    _message_dialog.close();
                }
            }

            G.Button {
                text: "Replace"
                type: G.Style.ButtonType.Warning
                width: G.Style.buttonWidth
                flat: true;

                onClicked: {
                    d.save(_self._current_file);
                    _message_dialog.close();
                }
            }

            G.Button {
                text: "Save as"
                type: G.Style.ButtonType.Base
                width: G.Style.buttonWidth
                flat: false;

                onClicked: {
                    _file_dialog_save.open();
                }
            }
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

    G.Toast {
        id: _read_only_toast

        parent: Overlay.overlay
        header: "Read-only file"
        message: "The source file you opened does not belong to the project, therefore it is read-only in the code editor."

        type: G.Style.ButtonType.Warning
    }

    G.Toast {
        id: _wrong_extension_toast

        parent: Overlay.overlay
        header: "Not a valid source file"
        message: "The file you are trying to open is not a code source file and can not be loaded"

        type: G.Style.ButtonType.Warning
    }

    function open_source_file(path) {
        let file_path = decodeURIComponent(path);
        let file_name = file_path.split('/').pop()
        
        /* if (_self.mode == "Python") {
            d.code.fileName = file_name;
        } */
        
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

                            text: guessFile(modelData)

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
                            anchors.verticalCenter: parent.verticalCenter;
                            anchors.rightMargin: G.Style.smallPadding
                            visible : _texture_file_path.text.endsWith(".jpg") & GUtils.fileExists(_texture_file_path.text)
                            color : "green"
                            size: G.Style.iconSmall;
                            icon: "file-check"

                        }

                        G.IconButton {
                            id: _edit_texture_button

                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter;
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
                                _missing_textures_dialog.missingTextureFiles.push(file_path)
                            }
                        }
                    }
                }
            }
        }

        standardButtons:  Dialog.Ok | Dialog.Cancel

        onAccepted : {
            d.copyTextureFiles(_missing_textures_dialog.missingTextureFiles);
        }
    }

    function guessFile(filename) {
        let current_directory = Utils.urlToPath(_self._current_file).split("/").slice(0, -1).reduce((path, dir) => path + "/" + dir, "")
        if (current_directory.startsWith("//")) {
            current_directory = current_directory.substring(1)
        }
        let candidate_file = current_directory + "/" + filename
        return GUtils.fileExists(candidate_file)? candidate_file : ""
    }
}
