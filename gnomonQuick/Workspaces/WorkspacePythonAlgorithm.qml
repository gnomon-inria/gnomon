import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import xQuick           1.0 as X
import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G

import gnomon.Workspaces    1.0 as GW

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

    RowLayout {
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.margins: 10;

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

    Component.onCompleted: {
        G.Associator.associate(_source_view, d.source);
        G.Associator.associate(_target_view, d.target);
        drawel.close();
    }
}
