import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15

import Qt.labs.platform  1.0 as P

import gnomon.Visualization 1.0 as GV

import gnomonQuick.Style     1.0 as G
import gnomonQuick.Controls  1.0 as G
import gnomonQuick.Icons     1.0 as G

Item {

    id: _self;

    //a reference to the parent container, the world
    property var world: null;
    property alias ref: _thumbnail.ref;
    property bool containsMouse : (_dragger.containsMouse || _save_icon.containsMouse || _delete_icon.containsMouse || _edit_icon.containsMouse)

    width: _self.world.height - 6;
    height: _self.world.height - 6;

    Repeater {
        id: _frame
        model: [2, 1, 0]

        Rectangle {
            id: _indicator;

            width: _self.width
            height: _self.height
            radius: G.Style.panelRadius;

            anchors.left: _self.left
            anchors.top: _self.top
            anchors.margins: 3*modelData

            color: G.Style.colors.gutterColor

            border.width: G.Style.borderWidth;
            border.color: world.currentIndex == _self.ref ? G.Style.colors.highlightColor : G.Style.colors.gutterColor;

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
        Drag.hotSpot: Qt.point(world.height/2, world.height/2);
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
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            hoverEnabled: true;

            drag.target: parent;
            drag.maximumX: 0;
            drag.maximumY: 0;

            onPressed: parent.grabToImage(function(result) {
                parent.Drag.imageSource = result.url
            }, Qt.size(world.height, world.height));

            onClicked: {

                if (mouse.button === Qt.LeftButton) {
                    world.currentIndex = model.index;
                    world.currentRef = _self.ref;
                }

                if (mouse.button === Qt.RightButton) {
                    if(_formMenu.opened)
                        _formMenu.close()
                    else
                        _formMenu.open()
                }

            }

            onContainsMouseChanged: {
                _self.world.interactive = !containsMouse;
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
            visible: _dragger.containsMouse && !_formMenu.opened
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

            visible: _self.containsMouse && world.height > G.Style.smallDelegateHeight

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
            visible: _self.containsMouse && world.height > G.Style.smallDelegateHeight

            onClicked: {
                if(GV.World.deleteForm(form_id)) {
                    world.getIdAndRemove(model.index)
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
            visible: _self.containsMouse && world.height > G.Style.smallDelegateHeight

            onClicked: _file_dialog.open()
        }
    }

    G.Menu {
        id: _formMenu

        y: _self.height + G.Style.smallPadding
        width: _self.width;

        title: GV.World.getDynamicFormMetadata(form_id).data["name"]

        Action {
            text: qsTr("Save")
            shortcut: StandardKey.Save
            onTriggered: {
                _file_dialog.open()
            }
        }
        Action {
            text: qsTr("Edit")
            shortcut: StandardKey.Open
            onTriggered: {
                metadata_edit.open()
            }
        }
        Action {
            text: qsTr("Delete")
            shortcut: StandardKey.Open
            onTriggered: {
                if(GV.World.deleteForm(form_id)) {
                    world.getIdAndRemove(model.index)
                } else {
                    _delete_form_toast.open()
                }
            }
        }
        Action {
            text: qsTr("Cache")
            shortcut: StandardKey.Open
            onTriggered: {
                //GV.World.formWriterNameFilter(form_id)
                GV.World.addToCache(form_id)
            }
        }
        Action {
            text: qsTr("Load Cache")
            shortcut: StandardKey.Open
            onTriggered: {
                GV.World.loadFromCache(form_id)
            }
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
