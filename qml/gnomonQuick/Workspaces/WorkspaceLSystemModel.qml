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
import gnomon.Project    as GP

import gnomon.Workspaces 1.0 as GW


G.Workspace {

    id: _self;

    workspace_title: "L-System Model (Beta)";

    property string _current_file: "";
    property alias editor: _editor;

    fill: () => {
        if(world.currentRef < 0)
            return;

        //_source_view.droppedFromManager(world.currentRef);
    }

    viewSelected: _editor

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

        // Called only whe restoring workspace
        onRequestOpenFile: (path) => {
            let file_path = decodeURIComponent(path);
            let file_name = file_path.split('/').pop()
            d.read(file_path, false, true);
            _self.editor.contents = d.text
            _self.editor.language = _self._current_file.endsWith(".lpy") ? "lpy" : "python"
        }
    }

    property string _path: d.defaultReadPath()

    Settings {
        category: "lpy"
        property alias path: _self._path
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

            G.Monaco {
                id: _editor

                anchors.fill: parent

                theme: G.Style.mode == G.Style.Mode.Dark ? 'vs-dark' : 'vs-light';
                language: "lpy";
                fileName: d.fileName
                readOnly: d.readOnly

                onModified: (contents) => {
                    if(eval(contents)) {
                        d.text = eval(contents);
                    }
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
                    let file_path = GP.ProjectManager.project.findFile(d.fileName)
                    //_editor.readOnly = GP.ProjectManager.project.isReadOnly(file_path) && (!d.fileName.includes("vonKoch.lpy"))

                }

                onIdeIsReady : () => {
                    //d.restore();
                    d.codeEditorReady()
                }

                onMakeFileEditable: () => {
                    import_lpy_file_to_project.importPath = GP.ProjectManager.project.currentDir;
                    import_lpy_file_to_project.open()
                }
                Connections {
                    target: d
                    function onTextChanged() {
                        if(_editor.contents !== d.text) {
                            _editor.contents = d.text;
                        }
                    }
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

    Component.onCompleted: {
        G.Associator.associate(_view, d.view);

        if (d.fileName)
            _editor.tabName = d.fileName;
        _editor.contents = d.text;
        d.onParametersChanged();
        drawel.close();
    }

    G.ProjectImportDialog {
        id: import_lpy_file_to_project

        onAccepted : {
            //_editor.readOnly = false
            d.importFile(d.fileName, import_lpy_file_to_project.importPath)
        }
    }
}
