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
            /*
            let paths = [];
            for(let i_n in _file_dialog.files){
                paths.push(decodeURIComponent(_file_dialog.files[i_n]))
            }
            d.readerPath = paths.join(",")
            d.requestReaders();
            */
            window.current_workspace().requestOpenFiles(_file_dialog.files)
        }
    }

    ColumnLayout {
        anchors.fill: parent;
        anchors.margins: 12;

        X.Finder {
            id: _finder;
            Layout.fillWidth: true;
            Layout.fillHeight: true;

            extensionFilters: _extensions_model;

            onFileDoubleClicked: {
                //d.readerPath = decodeURIComponent(fileUrl);
                //d.requestReaders();
                window.current_workspace().requestOpenFiles([fileUrl])
            }
        }

        RowLayout {

            spacing: 10

            X.ButtonRaw {
                text: _finder.selectedFolder ? "Open" : "Load as";
                enabled: _finder.selectedFolder || _finder.selectedFile
                Layout.fillWidth: true;

                onClicked: {
                    if(_finder.selectedFolder) _finder.openFolder(_finder.selectedFolder)
                    if(_finder.selectedFile) {
                        d.readerPath = decodeURIComponent(_finder.selectedFile);
                        d.requestReaders("");
                        //window.current_workspace().requestOpenFiles([_finder.selectedFile])
                    }
                }
            }

            X.ButtonRaw {
                text: "Load Multiple Files";
                Layout.fillWidth: true;

                onClicked: {
                    _file_dialog.open()
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
