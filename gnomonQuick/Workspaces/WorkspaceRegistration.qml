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
import gnomon.Visualization 1.0 as GV

G.Workspace {

    id: _self;

    workspace_title: "Registration";

    property alias d: d;

    fill: () => {
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

        onStarted: idleStart();
        onFinished: idleStop();

        onParametersChanged: {
            _self.updateParametersModel();
        }

        onStackLevelChanged: {
            _stack.currentIndex = d.stackLevel+1
        }
    }

    GridLayout {
        id: _grid_block

        anchors.fill: parent
        anchors.margins: 5;

        columns: 2;

        G.View {
            id: _source_view_other;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            onDroppedFromManager: (index) => {
                d.sources.views[1].drop(index);
                if(GV.World.timeKeys(index).length > 1) {
                    d.sources.views[0].drop(index);
                    ts_slider.value = Math.max(_source_view_ref.ts_slider.value - 1, ts_slider.from)
                }
            }

            ts_slider.value: Math.max(d.sources.views[0].currentTime-1, ts_slider.from)
            ts_slider.onValueChanged: {
                let new_val = Math.min(ts_slider.value+1, ts_slider.to)
                if(new_val != _source_view_ref.ts_slider.value) {
                    _source_view_ref.ts_slider.value = new_val
                }
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

                G.DataDict {
                    id: _data_target_view;

                    Layout.fillWidth: true;
                    height: window.height/8;

                    X.Label {
                        anchors.top: parent.top
                        anchors.left: parent.left

                        text: "Transformation matrix"
                        color: X.Style.foregroundColor
                    }

                    X.Label {
                        anchors.centerIn: parent

                        text: d.targetDict.dataDict
                        horizontalAlignment: Text.AlignRight
                        color: X.Style.foregroundColor
                        font {
                            pointSize: 14
                            bold: true
                        }
                    }

                    viewLogic: d.targetDict;
                }

                X.Label {
                    Layout.fillWidth: true;

                    height: 24
                    text: "Stack level " + _stack.currentIndex
                    horizontalAlignment: Text.AlignRight
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

            onDroppedFromManager: (index) => {
                d.sources.views[0].drop(index);
                if(GV.World.timeKeys(index).length > 1) {
                    d.sources.views[1].drop(index);
                    ts_slider.value =  Math.min(_source_view_other.ts_slider.value + 1, ts_slider.to)
                }
            }

            ts_slider.onValueChanged: {
                let new_val = Math.max(ts_slider.value-1, ts_slider.from)
                if(new_val != _source_view_other.ts_slider.value) {
                    _source_view_other.ts_slider.value = new_val
                }
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
            property bool _fullscreen: true

            X.Icon {
                icon: _target_view._fullscreen ? X.Icons.icons.fullscreen : X.Icons.icons.close_fullscreen
                size: 32;
                color: X.Style.foregroundColor
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.bottomMargin: 30

                MouseArea {
                    id: _expand_view_area;

                    anchors.fill: parent;

                    hoverEnabled: true;

                    onClicked: {
                        var HEIGHT
                        var WIDTH
                        if(_target_view._fullscreen) {
                            _target_view._fullscreen = false
                            HEIGHT = _stack_container.height 
                            WIDTH = _stack_container.width
                            _target_view.Layout.preferredHeight = _grid_block.height
                            _target_view.Layout.preferredWidth = _grid_block.width
                            parent.ToolTip.text = "Reduce View"
                        } else {
                            _target_view._fullscreen = true
                            _target_view.Layout.preferredHeight = 0
                            _target_view.Layout.preferredWidth = 0
                            _target_view.Layout.height = HEIGHT
                            _target_view.Layout.width = WIDTH
                            parent.ToolTip.text = "Expand View"
                        }

                    }
                }

                ToolTip.visible: _expand_view_area.containsMouse;
                ToolTip.text: "Expand View";
            }

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
