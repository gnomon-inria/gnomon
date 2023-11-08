import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import QtQml.Models

import Qt.labs.platform as P
import QtCore

import gnomon.Utils as G
import gnomonQuick.Workspaces as G
import gnomonQuick.Controls   as G
import gnomonQuick.Style      as G
import gnomonQuick.Monaco     as G

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

    G.SessionSettings {
        category: uuid
        property alias state: d.state
    }

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

        onMessageChanged: {
            let msg_json = JSON.parse(d.message);
            if(msg_json.type === "parsing") {
                _editor.markers = msg_json
            }
        }

        onFileChanged: {
            _editor.fileName = d.fileName
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
        nameFilters: ["L-Py source files (*.lpy *.py)"]

        onAccepted: {
            _self.open_lpy_file(_file_dialog.file)
        }
    }

    G.Toast {
        id: _non_lpy_toast

        parent: Overlay.overlay
        header: "Not a .lpy file"
        message: "The file you opened is not a .lpy file, and can therefore not be run as a LSystem model."

        type: G.Style.ButtonType.Warning
    }

    P.FileDialog {
        id: _file_dialog_save

        title: "Save L-System model"

        currentFile: _self._path;
        fileMode: FileDialog.SaveFile

        modality: Qt.WindowModal;
        nameFilters: ["L-Py source files (*.lpy *.py)"]

        onAccepted: {
            let save_path = decodeURIComponent(_file_dialog_save.file)
            let save_filename = save_path.split('/').pop()
            if ((d.fileName.split('.').length == 1) || (d.fileName.split('.').pop() == save_filename.split('.').pop())) { //same extension
                _editor.tabName = save_filename
                d.save(save_path);
                d.fileName = save_filename
                _self._current_file = save_path;
                _self._path = folder;
            } else {
                _extension_change_toast.open()
            }
        }
    }

    G.Toast {
        id: _extension_change_toast

        parent: Overlay.overlay
        header: "Impossible to change extension"
        message: "You can not save this file using a different extension, please save it as a ." + d.fileName.split('.').pop() + " file."

        type: G.Style.ButtonType.Danger
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
                    iconName: "content-save"
                    empty: true

                    onClicked: {
                        _file_dialog_save.open()
                    }
                }

                G.Button {
                    id: _load_button

                    anchors.right: _button_container.right;
                    anchors.verticalCenter: _button_container.verticalCenter
                    anchors.margins: G.Style.smallPadding;

                    text: "Load";

                    type: G.Style.ButtonType.Base
                    iconName: "folder-open"
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

                theme: G.Style.mode == G.Style.Mode.Dark ? 'vs-dark' : 'vs-light';
                language: "lpy";
                fileName: d.fileName

                onModified: (contents) => {
                    d.text = eval(contents);
                }

                onFileSwitched: (name) => {
                    // Only takes into account .py and .lpy files
                    // python files needs to be taken into account because for now
                    // we are writing a copy of these files into a temporary dir.
                    // if the python files are not written into this temporary dir, local import of
                    // these files won't work.
                    // a fix would be to add the original directory into python syspath
                    name = eval(name)
                    if(name.endsWith("py"))
                        d.fileName = name
                }

                onIdeIsReady : () => {
                    d.restore();
                }
            }
        }

        Control {
            Layout.fillWidth: true;
            Layout.fillHeight: true;

            TabBar {
                id: _bar
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right

                currentIndex: _views.currentIndex

                G.TabButton {
                    text: "3D View"
                }
                G.TabButton {
                    text: "Text View"
                }
            }

            StackLayout {
                id: _views
                anchors.bottom: _bar.top
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                currentIndex: _bar.currentIndex;

                G.View {
                    id: _view;
                    Layout.fillWidth: true;
                    Layout.fillHeight: true;
                    viewLogic: d.view;
                    export_enabled: true;
                }

                G.TextView {
                    id: _lstring_view;
                    Layout.fillWidth: true;
                    Layout.fillHeight: true;
                    viewLogic: d.textView;
                    export_enabled: true;
                }

                onCurrentIndexChanged: {
                    window.currentView = currentIndex == 1 ? _lstring_view : _view
                }
            }
        }
    }

    Connections {
        target: d
        function onRequestOpenFile(path) {
            _self.open_lpy_file(path);
        }
    }

    Component.onCompleted: {
        G.Associator.associate(_view, d.view);

        if (d.fileName)
            _editor.tabName = d.fileName;
        _editor.contents = d.text;
        d.onParametersChanged();
        d.reset();
        drawel.close();
    }

    function open_lpy_file(path) {
        d.read(decodeURIComponent(path));
        _editor.contents = d.text
        _self._current_file = decodeURIComponent(path);
        _editor.language = _self._current_file.endsWith(".lpy") ? "lpy" : "python"
        _self._path = folder;

        if (!_self._current_file.endsWith(".lpy")) {
            _non_lpy_toast.open()
        }
    }
}
