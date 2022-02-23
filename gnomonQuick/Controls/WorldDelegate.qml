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

Item {

    id: _world_delegate;

    width: _world.height;
    height: _world.height;

    property alias ref: _thumbnail.ref;

    Rectangle {
        id: _thumbnail

        anchors.fill: parent;
        anchors.margins: 5;

        color: "#99D0A3BF"
        radius: 2;

        Drag.active: _dragger.drag.active
        Drag.dragType: Drag.Automatic
        Drag.hotSpot: Qt.point(_world.height/2, _world.height/2);
        property int ref: form_id

        P.FileDialog {
            id: _file_dialog

            nameFilters: [ "Image files (*.png *.tif *.inr *.gz *.ply)", "All files (*)" ]
            title: "save Gnomon Form"
            folder: shortcuts.home
            modality: Qt.Modal;
            fileMode: P.FileDialog.SaveFile

            onAccepted: {
                console.log("save", form_id)
                GV.World.saveAs(form_id, _file_dialog.file);
            }
        }

        Image {
            id: _img
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: "image://thumbnails/" + form_id
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

            onClicked: {
                _world.currentIndex = model.index;
                _world.currentRef = _world_delegate.ref;
            }
        }

        X.Icon {
            id: _delete_icon;
            icon: X.Icons.icons.delete;
            size: 24;
            color: X.Style.foregroundColor;

            anchors.top: _thumbnail.top
            anchors.topMargin: 5
            anchors.left: _thumbnail.left
            anchors.rightMargin: 5
            visible: parent.parent.height > 42

            MouseArea { id: _delete_mouse_area;
                        anchors.fill: parent;
                        hoverEnabled: true;

                        onClicked: {
                            GV.World.deleteForm(form_id)
                            console.log("delete ...", model.index, form_id)
                            _world_model.remove(model.index)
                        }
                      }

            ToolTip.visible: _delete_mouse_area.containsMouse;
            ToolTip.text: "Delete form";
        }

        X.Icon {
            id: _save_icon;
            icon: X.Icons.icons.save;
            size: 24;
            color: X.Style.foregroundColor;

            anchors.top: _thumbnail.top
            anchors.topMargin: 5
            anchors.right: _thumbnail.right
            anchors.rightMargin: 5
            visible: parent.parent.height > 42

            MouseArea {
                id: _save_mouse_area;
                anchors.fill: parent;
                hoverEnabled: true;

                onClicked: _file_dialog.open()
            }

            ToolTip.visible: _save_mouse_area.containsMouse;
            ToolTip.text: "Save form";
        }
    }

    Rectangle {

        id: _higlight_indicator;

         width: _world_delegate.width - 1
        height: _world_delegate.height - 1
        radius: 4;

        color: "#00000000";

        border.width: 2;
        border.color: X.Style.accentColor;

        visible: _world.currentIndex == model.index;
    }
}
