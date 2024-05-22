import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P

import gnomon.Utils as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G

import gnomon.Workspaces 1.0 as GW
import gnomon.Project 1.0 as GP

G.Workspace {

    id: _workspace;

    workspace_title: "CellImage Filter";
    
    fill: () => {
        if(world.currentRef < 0)
            return;

        _source_view.droppedFromManager(world.currentRef);
    }
    viewSelected: _source_view

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    G.SessionSettings {
        id: stt
        category: uuid
        property alias state: d.state
    }

    d: GW.WorkspaceCellImageFilter {
        id: d;

        onStarted: idleStart();
        onFinished: idleStop();

        onParametersChanged: {
            stt.sync()
            updateParametersModel();
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

        G.View {

            id: _source_view;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            viewLogic: d.source;

            onDroppedFromManager: (index) => {
                console.info('Retrieving from manager');
                window.currentView = _source_view
                d.source.drop(index);
            }
        }

        G.View {

            id: _target_view;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            viewLogic: d.target;
        }
    }

    Component.onCompleted: {
        G.Associator.associate(_source_view, d.source);
        G.Associator.associate(_target_view, d.target);

        d.onParametersChanged();
    }

}