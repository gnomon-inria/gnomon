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
                if(world.currentRef >= 0)
                    _source_view.droppedFromManager(world.currentRef)
            }
            drawel.update_menu()
        }

        onParametersChanged: {
            _self.updateParametersModel();
        }
    }

    ColumnLayout {
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.margins: 0;

        RowLayout {
            Layout.fillWidth: true;
            Layout.fillHeight: true;
            Layout.margins: 0;

            G.Monaco {
                id: _editor
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: d.editMode

                theme: G.Style.mode == G.Style.Mode.Dark ? 'vs-dark' : 'vs-light';
                language: 'python';
                fileName: d.code.fileName

                onModified: (contents) => {
                    d.code.text = eval(contents);
                    d.code.parseCode()
                }
                onFileSwitched : (name) => {
                    name = eval(name)
                    // Don't emit fileNameChanged signal when Tab 0
                    if(!name.endsWith("0"))
                        d.code.fileName = name
                    let file_path = GP.ProjectManager.project.findFile(d.code.fileName)
                    _editor.readOnly = (file_path.length === 0) & (!d.code.fileName.includes("example.py"))

                }

                onIdeIsReady : () => {
                    //d.restore();
                    d.codeEditorReady()
                }
            }

            G.View {
                id: _source_view
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: !d.editMode

                onDroppedFromManager: (index) => {
                    console.info('Retrieving from manager');
                    window.currentView = _source_view
                    d.source.drop(index);
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

        G.JupyterConsole {
            id: _console;
            Layout.fillWidth: true;
            Layout.preferredHeight:parent.height/3

            focus: true;

            Component.onCompleted: _console.set_style_sheet(G.Style.colors.bgColor);
        }
    }

    Connections {
        target: d.code
        function onCodeUpdated() {
            _editor.tabName = d.code.fileName;
            _editor.contents = d.code.text; 
        }
    }

   // Connections {
   //     target: X.Style
   //
   //     function onVariantChanged() {
   //         console.log('Setting color for', X.Style.flavors, 'and', X.Style.variant);
   //
   //         var color;
   //
   //         if (X.Style.flavors == 'MACOS' && X.Style.variant == 'LIGHT')
   //             color = X.Style.flavor_macos.base07;
   //         if (X.Style.flavors == 'MACOS' && X.Style.variant == 'DARK')
   //             color = X.Style.flavor_macos.base00;
   //         if (X.Style.flavors == 'UBUNTU' && X.Style.variant == 'LIGHT')
   //             color = X.Style.flavor_ubuntu.base07;
   //         if (X.Style.flavors == 'UBUNTU' && X.Style.variant == 'DARK')
   //             color = X.Style.flavor_ubuntu.base00;
   //         if (X.Style.flavors == 'FEDORA' && X.Style.variant == 'LIGHT')
   //             color = X.Style.flavor_fedora.base07;
   //         if (X.Style.flavors == 'FEDORA' && X.Style.variant == 'DARK')
   //             color = X.Style.flavor_fedora.base00;
   //
   //         if(X.Style.variant == 'LIGHT')
   //             _editor.theme = "vs-light";
   //         else
   //             _editor.theme = "vs-dark";
   //
   //         _console.set_style_sheet(color);
   //         _console.update();
   //     }
   // }

    Component.onCompleted: {
        G.Associator.associate(_source_view, d.source);
        G.Associator.associate(_target_view, d.target);
        if(d.code.fileName)
            _editor.tabName = d.code.fileName;
        _editor.contents = d.code.text;
        drawel.close();
    }
}
