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

import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G

import gnomon.Workspaces 1.0 as GW

G.Workspace {

    id: _workspace;

    workspace_title: "L-System Model";

    property string current_file: "";

    fill: () => {
        if(world.currentRef < 0)
            return;

        //_source_view.droppedFromManager(world.currentRef);
    }

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    d: GW.WorkspaceLSystemModel {
        id: d;

        onParametersChanged: {
            updateParametersModel(); //_workspace.updateParametersModel();
        }

        Component.onCompleted: {
            console.log(d.text)
        }
    }

    RowLayout {

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.margins: G.Style.smallPadding;

        Layout.fillWidth: true;
        Layout.fillHeight: true;

        G.Monaco {
            id: _editor

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            theme: X.Style.variant == 'LIGHT' ? 'vs-light' : 'vs-dark';
            language: 'python';
        }

        G.View {
            id: _view;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            viewLogic: d.view;
        }
    }

    Component.onCompleted: {
        G.Associator.associate(_view, d.view);

        console.log(d.text)
        _editor.contents = d.text;
        d.onParametersChanged();
    }
}
