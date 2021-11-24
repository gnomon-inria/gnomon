import QtQuick            2.15
import QtQuick.Controls   2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts    1.15

import Qt.labs.platform  1.0 as P

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

    P.FileDialog {
        id: _file_dialog

        nameFilters: [ "Image files (*.png *.tif *.inr *.gz *.ply)", "All files (*)" ]
        title: "save Gnomon Form"
        modality: Qt.NonModal;
        fileMode: P.FileDialog.SaveFile

        onAccepted: {
            console.log(model.index,_file_dialog.file)
            GV.World.saveAs(model.index, _file_dialog.file);
        }
    }

    delegate: Item {

        id: _world_delegate;

        width: _world.height;
        height: _world.height;

        Rectangle {
            id: _thumbnail

            anchors.fill: parent;
            anchors.margins: 5;

            color: "#99D0A3BF"
            radius: 2;

            Drag.active: _dragger.drag.active
            Drag.dragType: Drag.Automatic
            Drag.hotSpot: Qt.point(_world.height/2, _world.height/2);
            Drag.mimeData: {"gnomon/item": model.index }
            property int ref: model.index

            Image {
                id: _img
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "image://thumbnails/" + model.index
            }

            MouseArea {
                id: _dragger;

                anchors.fill: parent

                hoverEnabled: true;

                drag.target: parent;
                drag.maximumX: 0;
                drag.maximumY: 0;

                onPressed: parent.grabToImage(function(result) {
                    parent.Drag.imageSource = result.url
                }, Qt.size(_world.height,_world.height));
            }

            X.Icon { id: _save_icon;
                     icon: X.Icons.icons.save;
                     size: 24;
                     color: X.Style.foregroundColor;

                     anchors.top: _thumbnail.top
                     anchors.topMargin: 5
                     anchors.right: _thumbnail.right
                     anchors.rightMargin: 5
                     visible: parent.parent.height > 42

                     MouseArea { id: _save_mouse_area;
                                 anchors.fill: parent;
                                 hoverEnabled: true;

                                 onClicked: {
                                     _file_dialog.open()
                                     console.log("saving ...")
                                 }
                               }

                     ToolTip.visible: _save_mouse_area.containsMouse;
                     ToolTip.text: "Save form";
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
