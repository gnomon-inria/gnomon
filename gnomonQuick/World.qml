import QtQuick            2.15
import QtQuick.Controls   2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts    1.15

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

import gnomon.Visualization 1.0 as GV

ListView {

    id: _control;

    clip: true;
    orientation: ListView.Horizontal;

    snapMode: ListView.SnapToItem

    property alias count: _world_model.count;

    model: ListModel {
        id: _world_model;

    }

    delegate: Item {

        id: _world_delegate;

        width: _world.height;
        height: _world.height;

        Rectangle {

            anchors.fill: parent;
            anchors.margins: 5;

            color: "#99D0A3BF"
            radius: 2;

            Drag.active: _dragger.drag.active
            Drag.dragType: Drag.Automatic
            Drag.hotSpot: Qt.point(_world.height/2, _world.height/2);
            Drag.mimeData: {"gnomon/item": model.index }
            property int ref: model.index

            MouseArea {
                id: _dragger;

                anchors.fill: parent

                hoverEnabled: true;

                drag.target: _world_delegate;
                drag.maximumX: 0;
                drag.maximumY: 0;

                onPressed: parent.grabToImage(function(result) {
                    parent.Drag.imageSource = result.url
                }, Qt.size(_world.height,_world.height));
            }
        }
    }

    ScrollIndicator.horizontal: ScrollIndicator {

        orientation: Qt.Horizontal;
        visible: _world.contentWidth > _world.width;
    }

    Connections {
        target: GV.World
        function onAdded(id) {
            console.log("form added", id);
            _world_model.append({"ref": _world_model.count })
        }
    }

}
