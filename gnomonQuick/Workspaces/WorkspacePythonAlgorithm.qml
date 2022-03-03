import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import xQuick            1.0 as X
import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X


import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G

import gnomon.Workspaces    1.0 as GW
import gnomon.Jupyter    1.0 as G


G.Workspace { id: _self;

    workspace_title: "Python Algorithm";

    property alias d: d;
    property alias code: d.code;

    fill: () => {}

    focus: true;

    GW.WorkspacePythonAlgorithm { id: d;
        onEditModeChanged: {
            d.code.text = _editor.actualContents
            if (!d.editMode) {
                d.loadAlgorithm()
                _source_view.droppedFromManager(world.currentRef)
            }
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
        anchors.margins: 10;

        // Rectangle {
        //     Layout.fillWidth: true;
        //     Layout.fillHeight: true;

            RowLayout {
                Layout.fillWidth: true;
                Layout.fillHeight: true;
                Layout.margins: 10;
                // anchors.top: parent.top;
                // anchors.bottom: parent.bottom;
                // anchors.left: parent.left;
                // anchors.right: parent.right;
                // anchors.margins: 10;

                // TODO: use editor from x-vsc when ready
                X.Editor { id: _editor;

                    Layout.fillWidth: true;
                    Layout.fillHeight: true;
                    visible: d.editMode;

                    actualContents: d.code.text;

                    X.SourceHighliter { Component.onCompleted: {
                            setup(_editor.document, X.Style.languages.python);
                        }
                    }

                    onActualContentsChanged: {
                        d.code.text = actualContents;
                    }
                }

                G.View { id: _source_view;
                    Layout.fillWidth: true;
                    Layout.fillHeight: true;
                    visible: !d.editMode;

                    onDroppedFromManager: {
                        console.info('Retrieving from manager');
                        d.source.drop(index);
                    }

                    viewLogic: d.source;
                }

                G.View { id: _target_view;
                    Layout.fillWidth: true;
                    Layout.fillHeight: true;
                    visible: !d.editMode;

                    viewLogic: d.target;
                }
            }

        // color: Qt.darker(X.Style.backgroundColor)
        //}

        // Rectangle {
        G.JupyterConsole {
            id: _console;
            Layout.fillWidth: true;
            Layout.preferredHeight:parent.height/3

            focus: true;

            Component.onCompleted: _console.set_style_sheet(X.Style.alternateBaseColor);
        }
    }

    Connections {
        target: X.Style

        function onVariantChanged() {
            console.log('Setting color for', X.Style.flavors, 'and', X.Style.variant);

            var color;

            if (X.Style.flavors == 'MACOS' && X.Style.variant == 'LIGHT')
                color = X.Style.flavor_macos.base07;
            if (X.Style.flavors == 'MACOS' && X.Style.variant == 'DARK')
                color = X.Style.flavor_macos.base00;
            if (X.Style.flavors == 'UBUNTU' && X.Style.variant == 'LIGHT')
                color = X.Style.flavor_ubuntu.base07;
            if (X.Style.flavors == 'UBUNTU' && X.Style.variant == 'DARK')
                color = X.Style.flavor_ubuntu.base00;
            if (X.Style.flavors == 'FEDORA' && X.Style.variant == 'LIGHT')
                color = X.Style.flavor_fedora.base07;
            if (X.Style.flavors == 'FEDORA' && X.Style.variant == 'DARK')
                color = X.Style.flavor_fedora.base00;

            _console.set_style_sheet(color);
            _console.update();
        }
    }

    Component.onCompleted: {
        G.Associator.associate(_source_view, d.source);
        G.Associator.associate(_target_view, d.target);
        drawel.close();
    }
}
