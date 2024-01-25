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

    property var d;
    property string _current_file: "";
    property bool _read_only_py_file : false

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
                let file_name = relative_path.split('/').pop()

                d.code.fileName = file_name;
                d.read(relative_path, false);
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
            _self.open_py_file(path)
        }
    }

    P.FileDialog {
        id: _file_dialog;

        currentFile: _self._current_file;
        folder: d.defaultReadPath();
        fileMode: P.FileDialog.OpenFile;

        modality: Qt.NonModal;
        nameFilters: ["Python source files (*.py)"]

        onAccepted: {
            copy_py_file_to_project.open()
        }
    }

    P.FileDialog {
        id: _file_dialog_save

        title: "Save Python algorithm"

        folder: d.defaultReadPath();
        fileMode: P.FileDialog.SaveFile

        modality: Qt.WindowModal;
        nameFilters: ["Python source files (*.py)"]

        onAccepted: {
            let file_path = decodeURIComponent(_file_dialog_save.file);
            let file_name = file_path.split('/').pop()
            d.code.fileName = file_name;
            d.save(file_path);
            _self._current_file = _file_dialog_save.file;
            if (_message_dialog.visible) {
                _message_dialog.close()
            }
        }
    }

    G.Toast {
        id: _non_py_toast

        parent: Overlay.overlay
        header: "Not a saved file"
        message: "The file you opened is not saved, \nplease save it as python file before running."

        type: G.Style.ButtonType.Warning
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
            _self._read_only_py_file = false
            open_py_file(_file_dialog.file)
        }

        onRejected : {
            _self._read_only_py_file = true
            open_py_file(_file_dialog.file)
        }

    }

    function open_py_file(path) {
        let file_path = decodeURIComponent(path);
        let file_name = file_path.split('/').pop()
        d.code.fileName = file_name;
        d.read(file_path, _self._read_only_py_file);
        _self._current_file = path;
    }
}
