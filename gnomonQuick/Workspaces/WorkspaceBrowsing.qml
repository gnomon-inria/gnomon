import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import Qt.labs.platform  1.0 as P

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

    fill: () => {}

    focus: true;

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    GW.WorkspaceBrowser {
        id: d;

        onAvailable: {
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

        onFinished: idleStop();
    }

// /////////////////////////////////////////////////////////////////////////////

    G.View {
        id: _view;

        anchors.fill: parent;
        anchors.margins: 10;

        onDroppedFromFile: {
            d.readerPath = decodeURIComponent(path);
            d.requestReaders();
        }
        viewLogic: d.view;

        Component.onCompleted: G.Associator.associate(_view, d.view);
    }

    G.ReaderDialog {
        id: _reader_dialog;

        onReaderSelected: {
            console.log(reader);
            idleStart();
            d.readWith(reader);
        }
    }

    Component.onCompleted :  window.drawelr_closed = false;
}
