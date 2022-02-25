import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15

import Qt.labs.platform  1.0 as P

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

import gnomonQuick          1.0 as G
import gnomon.Visualization 1.0 as GV

ListView {

    id: _control;

    clip: true;
    orientation: ListView.Horizontal;

    snapMode: ListView.SnapToItem

    property alias count: _world_model.count;

    property int currentRef: -1;

    model: ListModel {
        id: _world_model;

    }

    delegate: G.WorldDelegate {

    }

    ScrollIndicator.horizontal: ScrollIndicator {

        orientation: Qt.Horizontal;
        visible: _world.contentWidth > _world.width;
    }

    Connections {
        target: GV.World
        function onAdded(id) {
            console.log("form added", id);
            _world_model.append({"form_id": id })
            _control.currentIndex = _world_model.count - 1;
            _control.currentRef = id;
        }
    }
}
