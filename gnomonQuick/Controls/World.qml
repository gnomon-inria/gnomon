import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15

import Qt.labs.platform  1.0 as P

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

import gnomonQuick.Controls 1.0 as G
import gnomonQuick.Style    1.0 as G

import gnomon.Visualization 1.0 as GV

ListView {

    id: _control;

    clip: true;
    orientation: ListView.Horizontal;
    spacing: G.Style.smallPadding;

    snapMode: ListView.SnapToItem

    property alias count: _world_model.count;

    property int currentRef: -1;

    model: ListModel {
        id: _world_model;
    }

    delegate: G.WorldDelegate {
        flickable: _control;
    }

    ScrollIndicator.horizontal: ScrollIndicator {

        orientation: Qt.Horizontal;
        visible: _world.contentWidth > _world.width;
    }

    Connections {
        target: GV.World
        function onAdded(id) {
            _world_model.append({"form_id": id })
            _control.currentIndex = _world_model.count - 1;
            _control.currentRef = id;
        }
        function onAlreadyAdded() {
            _already_added_toast.open();
        }
    }

    G.Toast {
        id: _already_added_toast

        parent: Overlay.overlay
        header: "Already exported !"
        message: "The Form you are trying to export is already present in the Form Manager; it will not be added a second time.";

        type: G.Style.ButtonType.Warning
    }

    MouseArea {
        anchors.fill: parent
        z: -1
        hoverEnabled: true
        property bool _world_opened: false
        
        onEntered: {
            if(_world_model.count > 0 & header_state === "UNANCHORED") {
                header_state  = "ANCHORED"
                _world_opened = true
                if(_timer.running)
                    _timer.stop()
            }
        }

        onExited: {
            if(_world_opened) {
                _timer.start()
                _world_opened = false
            }
        }
    }

    Timer {
        id: _timer;
        interval: 1000;
        onTriggered: {
            header_state  = "UNANCHORED"
        }
    }
}
