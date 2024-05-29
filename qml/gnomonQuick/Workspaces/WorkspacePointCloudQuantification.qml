import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P

import gnomon.Utils as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls  1.0 as G
import gnomonQuick.Style      1.0 as G

import gnomon.Visualization 1.0 as GV
import gnomon.Workspaces 1.0 as GW

G.Workspace {

    id: _self;

    workspace_title: "Point Cloud Quantification";

    fill: () => {
        if(world.currentRef < 0)
            return;

        _source_view.droppedFromManager(world.currentRef);
    }

    viewSelected: _source_view

    G.SessionSettings {
        category: uuid
        property alias state: d.state
    }

    d: GW.WorkspacePointCloudQuantification {
        id: d;

        onStarted: idleStart();
        onFinished: idleStop();

        onParametersChanged: {
            _self.updateParametersModel();
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


            onDroppedFromManager: (index) => {
                if (!GV.World.formLoaded(index)) {
                    _hibernating_toast.index = index
                    _hibernating_toast.open()
                } else {
                    d.source.drop(index);
                }
                console.info('Retrieving from manager');
                window.currentView = _source_view
            }

            viewLogic: d.source;

            Component.onCompleted: G.Associator.associate(_source_view, d.source);
        }

        G.Figure {

            id: _target_figure;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            viewLogic: d.figure;
        }
    }

    G.Toast {
        id: _hibernating_toast

        property int index;

        parent: Overlay.overlay
        header: "Dropping Hibernating Form"
        message: "The form was hibernating, please wait while it is reloaded. This may take a few seconds."

        type: G.Style.ButtonType.Base

        onOpened: {
            d.source.drop(_hibernating_toast.index);
            _hibernating_toast.close()
        }
    }

    Component.onCompleted: {
        d.onParametersChanged();
        console.log("Setting figure number",_target_figure.number)
        d.figure.figureNumber = _target_figure.number
        d.figure.figureCanvasReady();
    }
}
