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
            requestOpenFiles(urls)
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
