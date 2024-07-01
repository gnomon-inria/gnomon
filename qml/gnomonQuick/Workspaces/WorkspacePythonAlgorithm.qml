import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import gnomon.Utils as G
import gnomonQuick.Workspaces as G
import gnomonQuick.Controls   as G
import gnomonQuick.Style      as G
import gnomonQuick.Monaco     as G

import gnomon.Workspaces as GW
import gnomon.Jupyter    as G
import gnomon.Project    as GP


G.Workspace {

    id: _self;

    workspace_title: "Python Algorithm";

    property alias code: d.code;
    property alias editor: _editor;

    fill: () => {}

    viewSelected: _editor

    focus: true;

    G.SessionSettings {
        category: uuid
        property alias state: d.state
    }

    d: GW.WorkspacePythonAlgorithm {
        id: d;

        onStarted: idleStart();
        onFinished: idleStop();

        onEditModeChanged: {
            d.code.text = _editor.contents
            if(!d.editMode) {
                _self.viewSelected = _source_view
                _source_view.forceFocus()
                if(world.currentRef >= 0)
                    _source_view.droppedFromManager(world.currentRef)
            } else {
                _self.viewSelected = _editor
                _editor.forceFocus()
            }
            drawel.update_menu()
        }

        onParametersChanged: {
            _self.updateParametersModel();
        }

        onRequestOpenFile: (path) => {
            let file_path = decodeURIComponent(path);
            // let file_name = file_path.split('/').pop()
            // d.fileName = file_name;
            let read_only = !GP.ProjectManager.project.isAccessible(file_path)
            d.read(file_path, read_only);
        }
    }

    G.Monaco {
        id: _editor
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: parent.top;
        anchors.bottom: _console_panel.top;

        visible: d.editMode

        theme: G.Style.mode == G.Style.Mode.Dark ? 'vs-dark' : 'vs-light';
        language: 'python';
        fileName: d.fileName

        onModified: (contents) => {
            d.code.text = eval(contents);
            d.code.parseCode()
        }

        onFileSwitched : (name) => {
            name = eval(name)
            // Don't emit fileNameChanged signal when Tab 0
            if(!name.endsWith("0"))
                d.fileName = name
            console.log(d.fileName)
            let file_path = GP.ProjectManager.project.findFile(d.fileName)
            console.log(file_path)
            _editor.readOnly = (file_path.length === 0) && (!d.fileName.includes("example.py"))
            console.log(_editor.readOnly)
        }

        onFileClosed : (name) => {
            d.close(name)
        }

        onIdeIsReady : () => {
            //d.restore();
            _self.viewSelected = _editor
            d.codeEditorReady()
        }

        onMakeFileEditable: () => {
            import_file_to_project.importPath = GP.ProjectManager.project.currentDir;
            import_file_to_project.open()
        }

        onReadOnlyChanged: () => {
            d.readOnly = _editor.readOnly
        }
    }

    RowLayout {
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: parent.top;
        anchors.bottom: _console_panel.top;

        visible: !d.editMode

        G.View {
            id: _source_view
            Layout.fillWidth: true
            Layout.fillHeight: true

            onDroppedFromManager: (index) => {
                console.info('Retrieving from manager');
                window.currentView = _source_view
                _self.dropForm(d.source, index)
             }

            viewLogic: d.source
        }

        G.View {
            id: _target_view
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: !d.editMode

            viewLogic: d.target
        }
    }

    Rectangle {
        id: _console_panel;

        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        anchors.margins: G.Style.mediumPadding;

        radius: G.Style.panelRadius;
        color: G.Style.colors.gutterColor;
        border.color: G.Style.colors.fgColor;
        border.width: 1;

        width: G.Style.mediumPanelWidth;
        height: _console.height

        G.Dragger {
            id: _console_dragger;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.verticalCenter: parent.top;
            orientation: Qt.Horizontal;

            z: _self.parent? _self.parent.z + 1 : 0;

            onClicked: {
                if (_console.visible) {
                    _console.height = 0;
                    _console.visible = false;
                } else {
                    _console.height = G.Style.smallPanelHeight;
                    _console.visible = true;
                }
            }
        }

        G.JupyterConsole {
            id: _console;

            anchors.left: parent.left;
            anchors.right: parent.right;
            anchors.bottom: parent.bottom;
            anchors.margins: 0;

            // focus: true;
            visible: false
            height: 0

            Component.onCompleted: _console.set_style_sheet(G.Style.colors.bgColor);
        }
    }

    Connections {
        target: d.code
        function onCodeUpdated() {
            _editor.contents = d.code.text; 
        }
    }

    G.ProjectImportDialog {
        id: import_file_to_project

        onAccepted : {
            _editor.readOnly = false
            d.importFile(d.fileName, import_file_to_project.importPath)
        }
    }

    Component.onCompleted: {
        G.Associator.associate(_source_view, d.source);
        G.Associator.associate(_target_view, d.target);
        if(d.fileName)
            _editor.tabName = d.fileName;
        _editor.contents = d.code.text;
    }
}
