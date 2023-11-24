import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.platform as P
import QtCore

import gnomon.Utils as G
import gnomonQuick.Workspaces as G
import gnomonQuick.Controls   as G
import gnomonQuick.Style      as G

import gnomon.Workspaces as GW
import gnomon.Project    as GP

G.Workspace {
    id: _self;

    workspace_title: "Data Browsing";

    property string current_file: "";

    Settings {
        id: _cache;
        category: "ReaderDialogCache";
    }

    G.SessionSettings {
        category: uuid
        property alias state: d.state
    }

    function requestOpenFiles(urls) {
        let paths = [];
        let default_plugin = ""
        for(let i_n in urls) {
            let path = decodeURIComponent(urls[i_n])
            if (default_plugin == "") {
                default_plugin = _cache.value(path, "")
            }
            console.log(GP.ProjectManager.project.findFile(path))
            paths.push(path)
        }
        d.readerPath = paths.join(",");
        d.requestReaders(default_plugin);
    }

    fill: () => {}

    focus: true;

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    d: GW.WorkspaceBrowser {
        id: d;

        onStarted: idleStart();
        onFinished: idleStop();

        onAvailable: (readers) => {
            if(Object.keys(readers).length == 1) {
                d.readWith(Object.keys(readers)[0])
            } else {
                _reader_dialog.availableReaders.clear();
                for (var r in readers) {
                    _reader_dialog.availableReaders.append({
                              "title": readers[r].metadata.name.toString(),
                              "key": r.toString(),
                            "description": readers[r].description.toString(),
                            "preview": readers[r].preview.toString()
                    });
                }
                _reader_dialog.open();
            }
        }

        onNoReaderAvailable: (ext) => {
            console.log("No reader available: extension not recognized.")
            extension_error.message = "No reader available: extension not recognized."
            extension_error.open()
        }
    }

    G.Toast {
        id: extension_error
        header: "Reader error"
        message: ""
        type: G.Style.ButtonType.Warning
    }

// /////////////////////////////////////////////////////////////////////////////

    G.View {
        id: _view;

        anchors.fill: parent;
        anchors.margins: G.Style.smallPadding;

        onDroppedFromFile: (path) => {
            let urls = path.split(',')
            let external_paths = []
            let relative_paths = []
            for(let i_n in urls) {
                let path = decodeURIComponent(urls[i_n])
                if (!GP.ProjectManager.project.isAccessible(path)) {
                    external_paths.push(path)
                } else {
                    relative_paths.push(GP.ProjectManager.project.relativePath(path));
                }
            }
            if (external_paths.length > 0) {
                _external_data_dialog.urls = urls
                _external_data_dialog.paths = external_paths
                _external_data_dialog.open()
            } else {
                requestOpenFiles(relative_paths)
            }
        }
        viewLogic: d.view;

        Component.onCompleted: G.Associator.associate(_view, d.view);
    }

    G.ReaderDialog {
        id: _reader_dialog;

        onReaderSelected: (reader) => {
            idleStart();
            let paths = d.readerPath.split(",")
            for(let i in paths) {
                console.debug("Saving", reader, "as default reader for", paths[i])
                _cache.setValue(paths[i], reader)
            }
            _reader_toast.reader_name = reader;
            _reader_toast.open();
            d.readWith(reader);
        }
    }

     G.Dialog {
        id: _external_data_dialog;

        property var urls: []
        property var paths: []

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.mediumDialogWidth;
        height: G.Style.mediumDialogHeight;

        padding: G.Style.smallPadding;

        parent: Overlay.overlay
        modal: true
        title: "External data"
        standardButtons:  Dialog.Cancel | Dialog.Ok

        Label {
            id: _external_label
            anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.right: parent.right;
            text: "Some of the data files you would like to open are not part of the project directory, or of one of its data directories.\nPlease specify which directories to add to the project's data path to be able to load them."
            wrapMode: Text.WordWrap

            font: G.Style.fonts.cardLabel
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            topPadding: G.Style.smallPadding;
            bottomPadding: G.Style.smallPadding;
            leftPadding: G.Style.mediumPadding
            rightPadding: G.Style.mediumPadding
        }

        ListView {
            id: _external_list_view

            anchors.top: _external_label.bottom;
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;
            anchors.left: parent.left;
            anchors.margins: G.Style.smallPadding;

            clip: true;
            spacing: G.Style.smallPadding;

            model: _external_data_dialog.paths

            delegate: TextArea {
                width: _external_list_view.width

                text: modelData.startsWith("file://") ? modelData.slice(7) : modelData
                font: G.Style.fonts.label

                background: Rectangle {
                    color: G.Style.colors.gutterColor
                }
            }
        }

        onAccepted: {
            for (let i_f in _external_data_dialog.paths) {
                let data_path = _external_list_view.itemAtIndex(i_f).text
                GP.ProjectManager.project.addDirectoryToDataPath(data_path)
            }
            let relative_paths = [];
            for (let i_f in _external_data_dialog.urls) {
                let url = _external_data_dialog.urls[i_f];
                relative_paths.push(GP.ProjectManager.project.relativePath(url));
            }
            requestOpenFiles(relative_paths)
        }

        onRejected: {
            _external_data_dialog.close();
        }
    }

    G.Toast {
        id: _reader_toast

        property string reader_name: "";

        parent: Overlay.overlay
        header: "Reader updated"
        message: "Your choice has been saved! Next time you will open this file, it will be loaded using " + _reader_toast.reader_name + ". You can use \"LOAD AS\" to choose a different Reader.";

        type: G.Style.ButtonType.Base
    }

    Component.onCompleted :  window.drawelr_closed = false;
}
