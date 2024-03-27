import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.platform  1.0 as P
import QtQml.Models

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G

import gnomon.MetaData        1.0 as GM
import gnomon.Project         1.0 as GP

Item {

    id: _self;

    property var parameters;
    property var d;
    property string _current_file: "";

    ListModel {
        id: _extensions_model;
        ListElement {
            text: "All"
            regexp: "*"
        }
    }

    P.FileDialog {
        id: _file_dialog;

        currentFile: _self._current_file;
        folder: d.defaultReadPath();

        modality: Qt.NonModal;
        fileMode: P.FileDialog.OpenFiles;

        onAccepted: {
            window.current_workspace().checkFileAccessibility(_file_dialog.files)
        }
    }

    G.Dialog {
        id: _load_url_dialog;

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.smallDialogWidth;

        parent: Overlay.overlay
        modal: true
        standardButtons:  Dialog.Ok | Dialog.Cancel
        G.TextField {
            id: _url_text_field
            Layout.fillWidth: true
            width: parent.width
            placeholderText: qsTr("Enter an url like: https://...")
        }
        header: ToolBar {
            height: 0
        }
        onAccepted: {
            let file_url = _url_text_field.text
            _url_text_field.text = ""
            d.readerPath = decodeURIComponent(file_url);
            d.requestReaders("");
        }
    }

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
                window.current_workspace().requestOpenFiles([relative_path])
            }

            onFileRightClicked: (fileUrl) => {
                let relative_path = GP.ProjectManager.project.relativePath(fileUrl)
                d.readerPath = decodeURIComponent(relative_path);
                d.requestReaders("");
            }
        }

        Item {
            id: _button_container

            height: G.Style.largeButtonHeight
            Layout.fillWidth: true;

            G.Button {
                id: _load_file_button
                anchors.top: _button_container.top
                anchors.left: _button_container.left;
                anchors.margins: G.Style.smallPadding;

                text: "File";
                type: G.Style.ButtonType.Base
                iconName: "folder-multiple-plus"
                empty: true

                onClicked: {
                    _file_dialog.open()
                }
            }

            G.Button {
                id: _load_url_button
                anchors.top: _button_container.top
                anchors.left: _load_file_button.right
                anchors.margins: G.Style.smallPadding;

                text: "URL";
                type: G.Style.ButtonType.Base
                iconName: "cloud-download"
                empty: true

                onClicked: {
                    _load_url_dialog.open()
                }
            }
        }
    }

    /*Component.onCompleted: {
        const extensions = d.extensions;

        const regexps = extensions.reduce((prev, curr) => {
            return prev.concat(["*." + curr])
        }, [])

        _extensions_model.append({
            "text": "Readable files (" + extensions.join() + ")",
            "regexp": regexps.join()
        })
    }*/
}
