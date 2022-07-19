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

    property bool containsMouse : (_dragger.containsMouse || _save_icon.containsMouse || _delete_icon.containsMouse || _edit_icon.containsMouse)

    Repeater {
        id: _frame
        model: [2, 1, 0]

        Rectangle {
            id: _indicator;

             width: _world_delegate.width
            height: _world_delegate.height
            radius: G.Style.panelRadius;

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

            G.IconButton {
                iconName: G.Icons.icons["arrow-down-drop-circle"];
                size: G.Style.smallDelegateHeight;
                color: G.Style.colors.lightBlue;
                visible: _drop.containsDrag;
                anchors.centerIn: parent;
            }

            onDropped: {
                if (!drop.hasUrls) {
                    GV.World.compose(form_id, drag.source.ref)
                }
                drop.accept();
            }
        }

        G.ToolTip {
            visible: _dragger.containsMouse
            text: GV.World.getDynamicFormMetadata(form_id).data["name"]
        }

        G.IconButton {
            id: _edit_icon;
            iconName: G.Icons.icons["text-box-edit"];
            size: G.Style.iconMedium;
            color: G.Style.colors.textColorFaded;
            tooltip: "Edit form metadata";

            anchors.bottom: _thumbnail.bottom
            anchors.bottomMargin: G.Style.borderWidth
            anchors.right: _thumbnail.right
            anchors.rightMargin: G.Style.borderWidth

            visible: _world_delegate.containsMouse && _world.height > G.Style.smallDelegateHeight

            onClicked: {
                metadata_edit.open()
            }
        }

        G.IconButton {
            id: _delete_icon;
            iconName: G.Icons.icons["delete"];
            size: G.Style.iconMedium;
            color: G.Style.colors.textColorFaded;
            tooltip: "Delete form";

            anchors.top: _thumbnail.top
            anchors.topMargin: G.Style.borderWidth
            anchors.left: _thumbnail.left
            anchors.leftMargin: G.Style.borderWidth
            visible: _world_delegate.containsMouse && _world.height > G.Style.smallDelegateHeight

            onClicked: {
                if(GV.World.deleteForm(form_id)) {
                    _world_model.remove(model.index)
                } else {
                    _delete_form_toast.open()
                }
            }
        }

        G.IconButton {
            id: _save_icon;
            iconName: G.Icons.icons["content-save"];
            size: G.Style.iconMedium;
            color: G.Style.colors.textColorFaded;
            tooltip: "Save form";

            anchors.top: _thumbnail.top
            anchors.topMargin: G.Style.borderWidth
            anchors.right: _thumbnail.right
            anchors.rightMargin: G.Style.borderWidth
            visible: _world_delegate.containsMouse && _world.height > G.Style.smallDelegateHeight

            onClicked: _file_dialog.open()
        }
    }

    G.MetadataDialog {
        id: metadata_edit

        formId: form_id;
    }

    G.Toast {
        id: _delete_form_toast
        parent: Overlay.overlay
        header: "Can't Delete This Form"
        message: "This form has an output edge or already dropped in an other workspace"

        type: G.Style.ButtonType.Warning
    }
}
