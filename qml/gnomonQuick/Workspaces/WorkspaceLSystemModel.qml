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

import "../Controls/utils.js" as Utils

G.Workspace {

    id: _self;

    workspace_title: "L-System Model";

    property string _current_file: "";
    property alias editor: _editor;
    property bool _read_only_lpy_file : false

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
                    let file_path = GP.ProjectManager.project.findFile(d.fileName)
                    _editor.readOnly = (file_path.length === 0) & (!d.fileName.includes("vonKoch.lpy"))

                }

                onIdeIsReady : () => {
                    //d.restore();
                    d.codeEditorReady()
                }

                onMakeFileEditable: () => {
                    import_lpy_file_to_project.open()
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
        d.reset();
        drawel.close();
    }

    G.Dialog {
        id: import_lpy_file_to_project

        simple_dialog : true

        parent: Overlay.overlay
        x: (parent.width - width) / 4
        y: (parent.height - height) / 4
        width: G.Style.smallDialogWidth
        height: G.Style.largeDelegateHeight
        header.height: 0

        Label {
            text: "Import this file to your project for editing. Otherwise, it remains read-only. \nProceed with importing?"
            font: G.Style.fonts.nodeHeaderSelected
        }

        standardButtons:  Dialog.Yes | Dialog.No

        onAccepted : {
            _editor.readOnly = false
            d.importFile(d.fileName)
        }
    }
}
