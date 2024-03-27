import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15

import Qt.labs.platform  1.0 as P

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
    property bool containsMouse;
    property int currentRef: -1;

    model: ListModel {
        id: _world_model;
    }

    delegate: G.WorldDelegate {
        required property string form_name
        required property int form_id
        required property int index
        world: _control;
    }

    ScrollIndicator.horizontal: ScrollIndicator {
        orientation: Qt.Horizontal;
        visible: _world.contentWidth > _world.width;
    }

    Connections {
        target: GV.World
        function onAdded(id, form_name) {
            _world_model.append({"form_id": id , "form_name": form_name})
            _control.currentIndex = _world_model.count - 1;
            _control.currentRef = id;
            updateContainsMouse();
        }
        function onRemoved(id) {
            _control.currentIndex = -1;
            _control.currentRef = -1;
            updateContainsMouse();
        }
        function onAlreadyAdded() {
            _already_added_toast.open();
        }
    }


    function updateContainsMouse() {
        _control.containsMouse = Qt.binding(function() {
            let in_delegate = false;
            for(let index=0; index<_world_model.count; index++) {
                if (_control.itemAtIndex(index)) {
                    in_delegate = in_delegate || _control.itemAtIndex(index).containsMouse;
                }
            }
            return in_delegate;
        });
    }

    G.Toast {
        id: _already_added_toast

        parent: Overlay.overlay
        header: "Already exported !"
        message: "The Form you are trying to export is already present in the Form Manager; it will not be added a second time.";

        type: G.Style.ButtonType.Warning
    }

    function getIdAndRemove(i) {
        let res = _world_model.get(i).form_id
        _world_model.remove(i)
        return res
    }

    function getFormName(i) {
        let name = "None"
        if(_world_model.get(i))
            name = _world_model.get(i).form_name
        return name
    }
}
