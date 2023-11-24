import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.platform as P
import QtCore

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

import gnomon.Pipeline  as GP
import gnomon.Project   as  GP

G.ToolBar {

    id: _control;

    property int    baseSize:  G.Style.smallDelegateHeight;
    //property int minimumSize:  G.Style.smallDelegateHeight;
    property int maximumSize: G.Style.smallPanelHeight;

    property alias world: _world;

    location: G.Style.ToolBarLocation.Top
    state: 'UNANCHORED';

    RowLayout {

        id: _anchor;

        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.leftMargin: G.Style.smallPadding;
        anchors.right: parent.right;
        anchors.rightMargin: G.Style.smallPadding;

        height: G.Style.controlHeight;

        spacing: G.Style.mediumPadding;

        G.IconButton {
            iconName: "menu";
            size: G.Style.iconLarge;
            color: G.Style.colors.textColorFaded;
            visible: Qt.platform.os != "osx"
            onClicked: _fileMenu.opened ? _fileMenu.close() : _fileMenu.open()

            G.Menu {
                id: _fileMenu

                y: parent.height + G.Style.smallPadding

                Action {
                    text: qsTr("Open")
                    shortcut: StandardKey.Open
                    onTriggered: {
                        loadFileDialog.open();
                    }
                }

                Action {
                    text: qsTr("Save Pipeline")
                    shortcut: StandardKey.Save
                    onTriggered: {
                        saveFileDialog.open()
                    }
                }

                Action {
                    text: qsTr("New Project")
                    shortcut: StandardKey.New
                    onTriggered: {
                        //TODO: come back on the behavior of new project from anywhere (should it close everything?)
                        //TODO: should use the new project dialog as well
                        folderDialog.open();
                    }
                }
                Action {
                    text: qsTr("Save Project")
                    shortcut: StandardKey.Save
                    onTriggered: {
                        GP.ProjectManager.project.save()
                    }
                }

                Action {
                    text: qsTr("Reset")
                    onTriggered: {
                        reset()
                    }
                }

                Action {
                    text: qsTr("Quit")
                    shortcut: StandardKey.Quit
                    onTriggered: {
                        Qt.quit();
                    }
                }
            }
        }

        Item { id: _placeholder_unanchored; Layout.fillWidth: true; Layout.fillHeight: true; }

        G.IconButton {
            iconName: "close-box-multiple";
            size: G.Style.iconMedium;
            color: G.Style.colors.textColorFaded;
            hoverColor: G.Style.colors.hoveredDangerColor;
            tooltip: "Reset: clear all the forms and workspaces"
            visible: stack_launcher.currentIndex != 0

            onClicked: {
                _reset_dialog.open();
            }
        }

        G.Dialog {
            id: _reset_dialog

            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: G.Style.mediumDialogWidth;
            height: G.Style.smallDialogHeight;

            padding: G.Style.smallPadding;

            parent: Overlay.overlay
            modal: true
            title: "Exit project"
            standardButtons: Dialog.Ok | Dialog.Cancel

            Label {
                anchors.fill: parent
                anchors.margins: G.Style.smallPadding

                text: "Do you really want to close the project and clear all the forms and workspaces?"
                wrapMode: Text.WordWrap

                font: G.Style.fonts.cardLabel
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }

            onAccepted: {
                console.log("reset!")
                window.reset();
            }
        }


        G.IconButton {
            iconName: "help-circle";
            size: G.Style.iconMedium;
            color: G.Style.colors.textColorFaded;
            tooltip: "Help"

            onClicked: _help_menu.opened ? _help_menu.close() : _help_menu.open();

            G.Menu {
                id: _help_menu

                x: parent.x + parent.width - _help_menu.width
                y: parent.height + G.Style.smallPadding

                Action {
                    text: qsTr("Gnomon Help Pages")
                    onTriggered: {
                        Qt.openUrlExternally("https://gnomon.gitlabpages.inria.fr/gnomon/");
                    }
                }
                Action {
                    function format(w_name) {
                        let words = w_name.split(' ');
                        let decap_words = []
                        for (let i in words) {
                            let w = words[i]
                            decap_words.push(w[0].toLowerCase() + w.substring(1))
                        }
                        return decap_words.join('_')
                    }

                    enabled : window.current_workspace() != undefined;
                    text: qsTr("Workspace Documentation")
                    onTriggered: {
                        let w_name = window.current_workspace().workspace_title
                        Qt.openUrlExternally("https://gnomon.gitlabpages.inria.fr/gnomon/workspace/" + format(w_name));
                    }
                }
                Action {
                    enabled : window.currentView;
                    text: qsTr("Viewer Shortcuts")
                    onTriggered: {
                        _shortcuts_dialog.open()
                    }
                }
            }
        }

        G.IconButton {
            iconName: "book";
            size: G.Style.iconMedium;
            color: G.Style.colors.textColorFaded;
            tooltip: "Journal"

            onClicked: {
                journl.open();
            }
        }

        G.IconButton {
            iconName: "cog";
            size: G.Style.iconMedium;
            color: G.Style.colors.textColorFaded;
            tooltip: "Settings"

            onClicked: {
                settgs.open();
            }
        }

        G.Dialog {
            id: _shortcuts_dialog

            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: G.Style.mediumDialogWidth;
            height: G.Style.mediumDialogHeight;

            padding: G.Style.smallPadding;

            parent: Overlay.overlay
            modal: true
            title: "Viewer Shortcuts"

            ListView {
                id: _shortcut_list_view
                anchors.fill: parent

                model: _shortcuts_list

                delegate: G.ListItemDelegate {
                    width: _shortcut_list_view.width

                    Label {
                        id: _shortcut_label
                        anchors.left: parent.left;
                        anchors.verticalCenter: parent.verticalCenter;
                        anchors.margins: G.Style.smallPadding;

                        width: parent.width/3

                        font: G.Style.fonts.formLabel;
                        verticalAlignment: Text.AlignVCenter
                        text: _shortcut;
                    }

                    Label {
                        id: _description_label
                        anchors.left: _shortcut_label.right;
                        anchors.right: parent.right;
                        anchors.verticalCenter: parent.verticalCenter;
                        anchors.margins: G.Style.smallPadding;

                        font: G.Style.fonts.formLabel;
                        verticalAlignment: Text.AlignVCenter
                        text: _description;
                    }
                }
            }

            footer: DialogButtonBox
            {
                visible: true

                G.Button {
                    id: _ok_button
                    anchors.right: parent.right;
                    anchors.margins: G.Style.smallPadding;

                    text: 'OK';
                    type: G.Style.ButtonType.Base

                    onClicked: {
                        _shortcuts_dialog.accept();
                    }
                }

                G.Button {
                    anchors.right: _ok_button.left;
                    anchors.margins: G.Style.smallPadding;

                    text: 'Other Shortcuts...';
                    flat: true
                    type: G.Style.ButtonType.Neutral
                    width: G.Style.longButtonWidth

                    onClicked: {
                        Qt.openUrlExternally("https://gnomon.gitlabpages.inria.fr/gnomon/shortcuts.html");
                        _shortcuts_dialog.reject();
                    }
                }
            }
        }
    }

    Item {
        id: _placeholder_anchored;

        anchors.top: _anchor.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
    }

    G.World {
        id: _world

        anchors.fill: parent

        property bool empty: count==0;

        onEmptyChanged: {
        }

        parent: _placeholder_unanchored;
    }

    G.Pipeline {

        id: _pipeline;

        anchors.fill: parent;
        windowHeight: _control.maximumSize - _control.baseSize
        windowWidth: _control.width

        parent: _placeholder_anchored;

    }

    ListModel {
        id: _shortcuts_list
        ListElement {
            _shortcut: "⇧ Shift + drag"
            _description: "Translate parallel to camera"
        }
        ListElement {
            _shortcut: "Ctrl + drag"
            _description: "Rotate around camera axis"
        }
        ListElement {
            _shortcut: "Scroll forward / backward"
            _description: "Zoom in / out"
        }
    }

    // G.MetadataExplorer {
    //     id: metadata_explorer
    //     enabled: _control.state == 'ANCHORED';
    //     visible: enabled;
    //
    //     anchors.fill: parent;
    //     anchors.topMargin: 142;
    //     //height: parent.maximumSize - _world.height;
    //
    //
    //     color: X.Style.backgroundColor;
    //     border.color: X.Style.accentColor;
    //     border.width: 1;
    //
    //     formId: _world.currentIndex;
    //
    //     //z: parent.z+1;
    // }

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    MouseArea {
        anchors.fill: parent
        z: -1
        hoverEnabled: true
        property bool _world_opened: false

        onEntered: {
            if(_world.count > 0 & _control.state === "UNANCHORED") {
                // if(_out_timer.running)
                //     _out_timer.stop()
                // _in_timer.start()
                // _world_opened = true
            } else {
                _world_opened = false
            }
        }

        onExited: {
            if(_world_opened) {
                if (_world.containsMouse) {
                    _out_timer.stop()
                    _world_opened = true
                } else {
                    if(_in_timer.running)
                        _in_timer.stop()
                    _out_timer.start()
                    _world_opened = false
                }
            }
        }
    }

    Timer {
        id: _out_timer;
        interval: 500;
        onTriggered: {
            _control.state  = "UNANCHORED"
        }
    }

    Timer {
        id: _in_timer;
        interval: 500;
        onTriggered: {
            _control.state  = "ANCHORED"
        }
    }

    states: [
        State {
            name: 'UNANCHORED'
            // PropertyChanges {
            //     target: _world;
            //     parent: _placeholder_unanchored;
            //     anchors.leftMargin: Qt.platform.os == "osx" ? 72 : G.Style.smallPadding;
            // }
            PropertyChanges {
                target: _control;
                height: _control.baseSize;
            }
        },
        State {
            name: 'ANCHORED'
            // PropertyChanges {
            //    target: _world;
            //    parent: _placeholder_anchored;
            //    anchors.leftMargin:  G.Style.smallPadding;
            // }
            PropertyChanges {
                target: _control;
                height: _control.maximumSize;
            }
        }
    ]

    transitions: [
        Transition {
            to: "UNANCHORED"
            NumberAnimation{
                duration: 200
                properties: "height"
                easing.type: Easing.InCubic
            }
        },
        Transition {
            to: "ANCHORED"
            NumberAnimation{
                duration: 200
                properties: "height"
                easing.type: Easing.OutCubic
            }
        }
    ]

    function getAndRemoveWorldId(i) {
        return _world.getIdAndRemove(i)
    }
}
