import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15

import Qt.labs.platform  1.0 as P

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

import gnomon.Visualization 1.0 as GV

import gnomonQuick.Style     1.0 as G
import gnomonQuick.Controls  1.0 as G
import gnomonQuick.Icons     1.0 as G

Item {

    id: _world_delegate;

    width: _world.height-6;
    height: _world.height-6;

    property var flickable: null;
    property alias ref: _thumbnail.ref;

    Repeater {
        id: _frame
        model: [2, 1, 0]

        Rectangle {
            id: _indicator;

             width: _world_delegate.width
            height: _world_delegate.height
            radius: 4;

            anchors.left: _world_delegate.left
            anchors.top: _world_delegate.top
            anchors.margins: 3*modelData

            color: G.Style.colors.gutterColor

            border.width: G.Style.borderWidth;
            border.color: _world.currentIndex == _world_delegate.ref ? G.Style.colors.highlightColor : G.Style.colors.gutterColor;

            visible: modelData == 0? true : GV.World.timeKeys(form_id).length > 1;
        }
    }

    Rectangle {
        id: _thumbnail

        anchors.fill: parent;
        anchors.margins: G.Style.borderWidth + 1;

        color: G.Style.colors.lightBlue
        radius: G.Style.borderWidth;

        Drag.active: _dragger.drag.active
        Drag.dragType: Drag.Automatic
        Drag.hotSpot: Qt.point(_world.height/2, _world.height/2);
        property int ref: form_id

        signal droppedFromManager(int index)

        P.FileDialog {
            id: _file_dialog

            nameFilters: [ GV.World.formWriterNameFilter(form_id), "All files (*)" ]
            title: "save Gnomon Form"
            folder: shortcuts ? shortcuts.home : ""
            modality: Qt.WindowModal;
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

            onContainsMouseChanged: {
                flickable.interactive = !containsMouse;
            }
        }

        DropArea {
            id: _drop;

            anchors.fill: parent;

            G.Icon {
                icon: G.Icons.icons["arrow-down-circle"];
                size: G.Style.smallDelegateHeight;
                color: G.Style.colors.lightBlue;
                visible: _drop.containsDrag;
                anchors.centerIn: parent;
            }

            onDropped: {
                // console.log("Dropping item on ", form_id, drop)
                if (!drop.hasUrls) {
                    GV.World.compose(form_id, drag.source.ref)
                }
                drop.accept();
            }
        }

        ToolTip {
            visible: _dragger.containsMouse
            text: GV.World.getDynamicFormMetadata(form_id).data["name"]
            delay: 500;
        }

        G.Icon {
            id: _edit_icon;
            icon: G.Icons.icons["pencil"];
            size: G.Style.iconMedium;
            color: G.Style.colors.textColorNeutral;
            clickable: true;
            tooltip: "Edit form metadata";

            anchors.bottom: _thumbnail.bottom
            anchors.bottomMargin: G.Style.smallPadding
            anchors.right: _thumbnail.right
            anchors.rightMargin: G.Style.smallPadding

            visible: (_dragger.containsMouse || _save_icon.mouse_area.containsMouse || _delete_icon.mouse_area.containsMouse || _edit_icon.mouse_area.containsMouse) && parent.parent.height > 42

            onClicked: {
                console.log("Form Id: ", form_id, metadata_edit.formId)
                metadata_edit.open()
            }
        }

        G.Icon {
            id: _delete_icon;
            icon: G.Icons.icons["delete"];
            size: G.Style.iconMedium;
            color: G.Style.colors.textColorNeutral;
            clickable: true;
            tooltip: "Delete form";

            anchors.top: _thumbnail.top
            anchors.topMargin: G.Style.smallPadding
            anchors.left: _thumbnail.left
            anchors.rightMargin: G.Style.smallPadding
            visible: (_dragger.containsMouse || _save_icon.mouse_area.containsMouse || _delete_icon.mouse_area.containsMouse || _edit_icon.mouse_area.containsMouse) && parent.parent.height > 42

            onClicked: {
                GV.World.deleteForm(form_id)
                console.log("delete ...", model.index, form_id)
                _world_model.remove(model.index)
            }
        }

        G.Icon {
            id: _save_icon;
            icon: G.Icons.icons["content-save"];
            size: G.Style.iconMedium;
            color: G.Style.colors.textColorNeutral;
            clickable: true;
            tooltip: "Save form";

            anchors.top: _thumbnail.top
            anchors.topMargin: G.Style.smallPadding
            anchors.right: _thumbnail.right
            anchors.rightMargin: G.Style.smallPadding
            visible: (_dragger.containsMouse || _save_icon.mouse_area.containsMouse || _delete_icon.mouse_area.containsMouse || _edit_icon.mouse_area.containsMouse) && parent.parent.height > 42

            onClicked: _file_dialog.open()
        }
    }

    G.MetadataDialog {
        id: metadata_edit

        formId: form_id;
    }
}
