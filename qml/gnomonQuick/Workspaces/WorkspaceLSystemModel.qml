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

    P.FileDialog {
        id: _file_dialog;

        currentFile: _self._current_file;
        folder: _self._path;
        fileMode: P.FileDialog.OpenFile;

        modality: Qt.NonModal;
        nameFilters: ["L-Py source files (*.lpy *.py)"]

        onAccepted: {
            copy_lpy_file_to_project.open()
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
                    let file_path = GP.ProjectManager.project.findFile(d.fileName)
                    _editor.readOnly = (file_path.length === 0) & (!d.fileName.includes("vonKoch.lpy"))

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

    G.Dialog {
        id: copy_lpy_file_to_project

        simple_dialog : true

        parent: Overlay.overlay
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.smallDialogWidth
        height: G.Style.largeDelegateHeight
        header.height: 0

        modal: true


        Label {
            text: "Copy this file to your project for editing. Otherwise, it remains read-only. \nProceed with copying?"
            font: G.Style.fonts.nodeHeaderSelected
        }

        standardButtons:  Dialog.Yes | Dialog.No

        onAccepted : {
            _self._read_only_lpy_file = false
            open_lpy_file(_file_dialog.file)
        }

        onRejected : {
            _self._read_only_lpy_file = true
            open_lpy_file(_file_dialog.file)
        }

    }

    G.Dialog {
        id : _missing_textures_dialog

        property var missingTextureFiles: []

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.mediumDialogWidth
        height: G.Style.mediumDialogHeight

        padding: 0;

        parent: Overlay.overlay

        focus: true
        modal: true
        title : "It seems you have some missing textures, please add them"
        Column {
            spacing: G.Style.smallPadding;
            anchors.fill: parent
            anchors.topMargin: G.Style.smallButtonHeight
            Repeater {
                model: d.missingTextures
                RowLayout {
                    Layout.fillWidth: true
                    height: G.Style.mediumLabelHeight
                    spacing: 2
                    Label {
                        id: _texture_label

                        Layout.preferredWidth: _texture_label.contentWidth;
                        Layout.rightMargin: G.Style.largePadding
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignTop

                        text: modelData
                        font: G.Style.fonts.formLabel
                        color: G.Style.colors.textColorBase
                    }

                    Rectangle {
                        height: G.Style.mediumLabelHeight
                        implicitWidth: Math.round(2/3 * parent.width)
                        color: G.Style.colors.gutterColor;
                        radius: G.Style.panelRadius

                        G.TextField {
                            id: _texture_file_path

                            anchors.right: _check_texture_icon.left
                            anchors.left: parent.left
                            anchors.bottom: parent.bottom;
                            anchors.top: parent.top
                            anchors.bottomMargin: G.Style.tinyPadding
                            placeholderText: qsTr("Enter texture file path like: /Users/...")
                        }

                        G.Icon {
                            id: _check_texture_icon

                            anchors.right: _edit_texture_button.left
                            anchors.bottom: parent.bottom;
                            anchors.rightMargin: G.Style.smallPadding
                            visible : false
                            color : "green"
                            size: G.Style.iconSmall;
                            icon: "file-check"

                        }

                        G.IconButton {
                            id: _edit_texture_button

                            anchors.right: parent.right
                            anchors.bottom: parent.bottom;
                            anchors.rightMargin: G.Style.smallPadding
                            size: G.Style.iconSmall;
                            iconName: "folder-open"

                            onClicked: {
                                _texture_dialog.open();
                            }
                        }

                    }

                    P.FileDialog {
                        id: _texture_dialog

                        nameFilters: [ "Image files (*.jpg)" ]
                        title: "Open texture file"
                        modality: Qt.WindowModal;
                        fileMode: P.FileDialog.OpenFile

                        onAccepted: {
                            let file_path = Utils.urlToPath(_texture_dialog.file.toString())
                            if(file_path.split("/").slice(-1)[0] === _texture_label.text) {
                                _texture_file_path.text = file_path
                                _check_texture_icon.visible = true
                                _missing_textures_dialog.missingTextureFiles.push(file_path)
                            }
                        }
                    }  
                }
            }
        }
        standardButtons:  Dialog.Ok | Dialog.Cancel

        onAccepted : {
            d.copyTexturesFiles(_missing_textures_dialog.missingTextureFiles);
        }

    }


    function open_lpy_file(path) {
        d.read(decodeURIComponent(path), _self._read_only_lpy_file);
        _editor.contents = d.text
        _self._current_file = decodeURIComponent(path);
        _editor.language = _self._current_file.endsWith(".lpy") ? "lpy" : "python"


        if (!_self._current_file.endsWith(".lpy")) {
            _non_lpy_toast.open()
        }
        if(d.missingTextures.length > 0)
            _missing_textures_dialog.open()
    }
}
