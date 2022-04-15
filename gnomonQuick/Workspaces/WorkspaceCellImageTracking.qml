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

import gnomon.Workspaces 1.0 as GW

G.Workspace {

    id: _self;

    workspace_title: "Tracking";

    property alias d: d;

    fill: () => {
        if(world.currentRef < 0)
            return;

        _source_view.droppedFromManager(world.currentRef);
    }

    GW.WorkspaceCellImageTracking {
        id: d;

        onParametersChanged: {
            _self.updateParametersModel();
        }
    }

    RowLayout {

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.margins: 10;

        Layout.fillWidth: true;
        Layout.fillHeight: true;

        G.View {

            id: _source_view;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            onDroppedFromManager: (index) => {
                console.info('Retrieving from manager');
                d.source.drop(index);
            }

            viewLogic: d.source;
            ts_slider.value: Math.max(d.target.currentTime-1, ts_slider.from)

            Component.onCompleted: G.Associator.associate(_source_view, d.source);
        }

        G.View {

            id: _target_view;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            viewLogic: d.target;

            ts_slider.to: d.source.timeMax
            ts_slider.value: Math.min(d.source.currentTime+1, ts_slider.to)

            ts_slider.enabled: true;
            ts_slider.visible: true;

            Component.onCompleted: G.Associator.associate(_target_view, d.target);
        }
    }

    Component.onCompleted: {
        d.onParametersChanged();
    }
}
