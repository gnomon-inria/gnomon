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

    workspace_title: "L-System Model";

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
                    import_lpy_file_to_project.importPath = GP.ProjectManager.project.currentDir;
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
        drawel.close();
    }

    G.Dialog {
        id: import_lpy_file_to_project

        modal: true

        property alias importPath : _copy_path.text

        parent: Overlay.overlay
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.smallDialogWidth
        height: G.Style.smallDialogHeight

        title: "Import file to the project?"

        Text {
            id: _long_message
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: G.Style.smallPadding
            anchors.bottomMargin: G.Style.smallPadding

            text: "You have to import this file to your project to edit it, otherwise, it will remain read-only. Please select where you want to copy it."
            font: G.Style.fonts.value
            color: G.Style.colors.textColorBase
            wrapMode: Text.Wrap
        }

        G.TextField {
            id: _copy_path

            property var isValid: _copy_path.text.includes(GP.ProjectManager.project.currentDir)

            backgroundColor: G.Style.colors.gutterColor
            backgroundHighlightColor: isValid? G.Style.colors.okColor : G.Style.colors.warningColor

            anchors.top: _long_message.bottom
            anchors.right: _check_path_icon.left
            anchors.left: parent.left
            anchors.margins: G.Style.smallPadding
        }

        G.Icon {
            id: _check_path_icon

            anchors.right: _edit_path_button.left
            anchors.verticalCenter: _copy_path.verticalCenter;
            anchors.rightMargin: G.Style.smallPadding
            color: _copy_path.isValid? G.Style.colors.okColor : G.Style.colors.warningColor
            size: G.Style.iconSmall;
            icon: _copy_path.isValid? "folder-check" : "folder-alert"
        }

        G.IconButton {
            id: _edit_path_button

            anchors.right: parent.right
            anchors.verticalCenter: _copy_path.verticalCenter;
            anchors.rightMargin: G.Style.smallPadding
            size: G.Style.iconSmall;
            iconName: "folder-open"

            onClicked: {
                _copy_path_dialog.currentFolder = "file://" + GP.ProjectManager.project.currentDir
                _copy_path_dialog.open();
            }
        }

        P.FolderDialog {
            id: _copy_path_dialog

            title: "Select folder"
            modality: Qt.WindowModal;

            onAccepted: {
                import_lpy_file_to_project.importPath = decodeURIComponent(_copy_path_dialog.folder).slice(7)
            }
        }

        footer: DialogButtonBox
        {
            visible: true

            alignment: Qt.AlignRight
            background: Rectangle {
                anchors.fill: parent
                color: G.Style.colors.gutterColor
            }

            G.Button {
                text: 'Cancel';
                flat: true
                type: G.Style.ButtonType.Neutral
                width: G.Style.buttonWidth
                onClicked: import_lpy_file_to_project.reject();
            }

            G.Button {
                text: 'OK';
                flat: true
                enabled: _copy_path.isValid
                type: _copy_path.isValid ? G.Style.ButtonType.OK : G.Style.ButtonType.Warning
                width: G.Style.buttonWidth
                onClicked: import_lpy_file_to_project.accept();
            }
        }

        onAccepted : {
            _editor.readOnly = false
            d.importFile(d.fileName, import_lpy_file_to_project.importPath)
        }
    }
}
