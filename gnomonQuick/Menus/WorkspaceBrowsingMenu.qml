import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import crossQuick        1.0 as C
import crossParameters   1.0 as C

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G
import gnomonQuick.Icons      1.0 as G

import gnomon.MetaData        1.0 as GM

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
            window.current_workspace().requestOpenFiles(_file_dialog.files)
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
        TextField {
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

        X.Finder {
            id: _finder;
            Layout.fillWidth: true;
            Layout.fillHeight: true;

            extensionFilters: _extensions_model;

            onFileDoubleClicked: (fileUrl) => {
                window.current_workspace().requestOpenFiles([fileUrl])
            }
        }

        Item {
            id: _button_container

            height: G.Style.largeButtonHeight * 2
            Layout.fillWidth: true;

            G.Button {
                id: _load_button
                anchors.top: _button_container.top
                anchors.right: _button_container.right;
                anchors.margins: G.Style.smallPadding;
                size: G.Style.ButtonSize.Large
                implicitWidth: _load_file_button.implicitWidth + _load_url_button.implicitWidth + G.Style.buttonPadding

                text: _finder.selectedFolder ? "Open" : "Load as";
                enabled: _finder.selectedFolder || _finder.selectedFile
                type: G.Style.ButtonType.Base
                iconName: _finder.selectedFolder ? G.Icons.icons["folder-open"] : G.Icons.icons["folder-download"]

                onClicked: {
                    if(_finder.selectedFolder) _finder.openFolder(_finder.selectedFolder)
                    if(_finder.selectedFile) {
                        d.readerPath = decodeURIComponent(_finder.selectedFile);
                        d.requestReaders("");
                    }
                }
            }

            G.Button {
                id: _load_file_button
                anchors.top: _load_button.bottom
                anchors.right: _button_container.right;
                anchors.margins: G.Style.smallPadding;

                text: "Load...";
                type: G.Style.ButtonType.Base
                iconName: G.Icons.icons["folder-multiple-plus"]
                empty: true

                onClicked: {
                    _file_dialog.open()
                }
            }

            G.Button {
                id: _load_url_button
                anchors.top: _load_button.bottom
                anchors.right: _load_file_button.left
                anchors.margins: G.Style.smallPadding;

                text: "Load URL";
                type: G.Style.ButtonType.Base
                iconName: G.Icons.icons["file-upload"]
                empty: true

                onClicked: {
                    _load_url_dialog.open()
                }
            }
        }
    }

    Component.onCompleted: {
        const extensions = d.extensions;

        const regexps = extensions.reduce((prev, curr) => {
            return prev.concat(["*." + curr])
        }, [])

        _extensions_model.append({
            "text": "Readable files (" + extensions.join() + ")",
            "regexp": regexps.join()
        })
    }
}
