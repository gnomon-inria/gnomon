import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import crossQuick        1.0 as C
import crossParameters   1.0 as C

import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G

import gnomon.Workspaces 1.0 as GW

G.Workspace {

    id: _self;

    workspace_title: "Registration";

    property alias d: d;

    fill: () => {
        console.log('Automatic filling of input source');

        if(world.currentRef < 0)
            return;

        _source_view_ref.droppedFromManager(world.currentRef);
    }

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    /* G.WorkspaceProgressDialog { */
    /*     id: _progress; */
    /*     workspace_logic: d; */
    /* } */

    GW.WorkspaceRegistration {
        id: d;

        onParametersChanged: {
            _self.updateParametersModel();
        }

        onStackLevelChanged: {
            _stack.currentIndex = d.stackLevel+1
        }
    }

    GridLayout {

        anchors.fill: parent
        anchors.margins: 5;

        columns: 2;

        G.View {
            id: _source_view_other;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            onDroppedFromManager: {
                d.sources.views[1].drop(index);
            }

            X.Label {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 10

                height: 24
                text: "Floating Image"
                horizontalAlignment: Text.AlignRight
            }

            viewLogic: d.sources.views[1];

            Component.onCompleted: G.Associator.associate(_source_view_other, d.sources.views[1]);
        }

        Control {
            id: _stack_container;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            ColumnLayout {
                anchors.fill: parent

                X.Label {
                    Layout.fillWidth: true;

                    height: 24
                    text: "Stack level " + _stack.currentIndex
                    horizontalAlignment: Text.AlignRight
                }

                X.Label {
                    Layout.fillWidth: true;

                    height: 60
                    text: "Transformation matrix:\n" + d.transformStringAt(_stack.currentIndex-1)
                    font.pixelSize: 10;
                }

                G.Stack {
                    id: _stack;

                    count: d.stackSize
                    currentIndex: d.stackLevel+1

                    Layout.fillHeight: true;
                    Layout.maximumWidth: _stack_container.width;
                    Layout.minimumWidth: _stack_container.width;

                    onCurrentIndexChanged: {
                        d.stackLevel = currentIndex-1
                    }
                }

                X.ButtonRaw {
                    Layout.fillWidth: true;

                    text: "Iterate";

                    onClicked: {
                        d.iterate();
                    }
                }
            }
        }

        G.View {
            id: _source_view_ref;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            onDroppedFromManager: {
                d.sources.views[0].drop(index);
            }

            X.Label {
                anchors.bottom: parent.bottom
                anchors.right: parent.right

                height: 24
                text: "Reference Image"
                horizontalAlignment: Text.AlignRight
            }

            viewLogic: d.sources.views[0];

            Component.onCompleted: G.Associator.associate(_source_view_ref, d.sources.views[0]);
        }

        G.View {
            id: _target_view;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            X.Label {
                anchors.bottom: parent.bottom
                anchors.right: parent.right

                height: 24
                text: "Registered Image"
                horizontalAlignment: Text.AlignRight
            }

            viewLogic: d.target;

            Component.onCompleted: G.Associator.associate(_target_view, d.target);
        }
    }

    Component.onCompleted: {
        d.onParametersChanged();
    }
}
