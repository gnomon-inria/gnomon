import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import Qt.labs.platform  1.0 as P
import Qt.labs.settings

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G

import gnomon.Workspaces      1.0 as GW

G.Workspace {

    id: _self;

    workspace_title: "Browsing";

    property string current_file: "";
    property alias d: d;

    Settings {
        id: _cache;
        category: "ReaderDialogCache";
    }

    fill: () => {}

    focus: true;

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    GW.WorkspaceBrowser {
        id: d;

        onAvailable: (readers) => {
            console.log(readers, Object.keys(readers))
            if(Object.keys(readers).length == 1) {
                d.readWith(Object.keys(readers)[0])
            } else {
                _reader_dialog.availableReaders.clear();
                for (var r in readers) {
                    _reader_dialog.availableReaders.append({
                              "title": r.toString(),
                        "description": readers[r].toString()
                    });
                }
                // if(d.displayReaderDialog)
                _reader_dialog.open();
            }
        }

        onFinished: idleStop();
    }

// /////////////////////////////////////////////////////////////////////////////

    G.View {
        id: _view;

        anchors.fill: parent;
        anchors.margins: 10;

        onDroppedFromFile: (path) => {
            let urls = path.split(',')
            let paths = [];
            let default_plugin = ""
            for(let i_n in urls) {
                let path = decodeURIComponent(urls[i_n])
                if (default_plugin == "") {
                    default_plugin = _cache.value(path, "")
                    console.log(path, " --> ", default_plugin)
                }
                paths.push(path)
            }
            d.readerPath = paths.join(",");
            d.requestReaders(default_plugin);
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
                console.log("Saving ", reader, " as default reader for ", paths[i])
                _cache.setValue(paths[i], reader)
            }
            d.readWith(reader);
        }
    }

    Component.onCompleted :  window.drawelr_closed = false;
}
