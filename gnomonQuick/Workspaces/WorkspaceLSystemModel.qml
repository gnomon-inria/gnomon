import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P
import Qt.labs.settings

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import crossQuick        1.0 as C
import crossParameters   1.0 as C

import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G
import gnomonQuick.Icons      1.0 as G

import gnomon.Workspaces 1.0 as GW

G.Workspace {

    id: _self;

    workspace_title: "L-System Model";

    property string _current_file: "";
    property alias editor: _editor;

    fill: () => {
        if(world.currentRef < 0)
            return;

        //_source_view.droppedFromManager(world.currentRef);
    }

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    d: GW.WorkspaceLSystemModel {
        id: d;
        property bool running: false

        onStarted: {
            d.running = true;
            idleStart();
        }
        onFinished: {
            d.running = false;
            idleStop();
        }

        onParametersChanged: {
            updateParametersModel(); //_self.updateParametersModel();
        }
    }

    property string _path: d.defaultReadPath()

    Settings {
        category: "lpy"
        property alias path: _self._path
    }

    P.FileDialog {
        id: _file_dialog;

        currentFile: _self._current_file;
        folder: _self._path;
        fileMode: P.FileDialog.OpenFile;

        modality: Qt.NonModal;
        nameFilters: ["L-Py source files (*.lpy)"]

        onAccepted: {
            d.read(decodeURIComponent(_file_dialog.file));
            _editor.contents = d.text
            _self._current_file = _file_dialog.file;
            _self._path = folder;
        }
    }

    P.FileDialog {
        id: _file_dialog_save

        title: "Save L-System model"

        folder: _self._path;
        fileMode: P.FileDialog.SaveFile

        modality: Qt.WindowModal;
        nameFilters: ["L-Py source files (*.lpy)"]

        onAccepted: {
            d.save(decodeURIComponent(_file_dialog_save.file));
            _self._current_file = _file_dialog_save.file;
            _self._path = folder;
        }
    }

    RowLayout {

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.margins: G.Style.smallPadding;

        Layout.fillWidth: true;
        Layout.fillHeight: true;

        Control {
            id: _editor_pane

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            Item {
                id: _button_container

                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: G.Style.largeButtonHeight

                G.Button {
                    anchors.right: _load_button.left;
                    anchors.verticalCenter: _button_container.verticalCenter
                    anchors.margins: G.Style.smallPadding;

                    text: "Save";

                    type: G.Style.ButtonType.Base
                    iconName: G.Icons.icons["content-save"]
                    empty: true

                    onClicked: {
                        if(_self._current_file == "") {
                            _file_dialog_save.open()

                        } else {
                            _message_dialog.open();
                        }
                    }
                }

                G.Button {
                    id: _load_button

                    anchors.right: _button_container.right;
                    anchors.verticalCenter: _button_container.verticalCenter
                    anchors.margins: G.Style.smallPadding;

                    text: "Load";

                    type: G.Style.ButtonType.Base
                    iconName: G.Icons.icons["folder-open"]
                    empty: true

                    onClicked: {
                        _file_dialog.open();
                    }
                }
            }

            G.Monaco {
                id: _editor

                anchors.top: parent.top
                anchors.bottom: _button_container.top
                anchors.left: parent.left
                anchors.right: parent.right

                theme: X.Style.variant == 'LIGHT' ? 'vs-light' : 'vs-dark';
                language: 'lpy';

                onModified: (contents) => {
                    d.text = eval(contents);
                }
            }
        }

        G.View {
            id: _view;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            viewLogic: d.view;
        }
    }

    Component.onCompleted: {
        G.Associator.associate(_view, d.view);

        _editor.contents = d.text;
        d.onParametersChanged();
        drawel.close();
    }
}
