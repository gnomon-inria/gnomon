import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P

import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G
import gnomonQuick.Icons      1.0 as G

import gnomon.Workspaces 1.0 as GW
import gnomon.Visualization 1.0 as GV

G.Workspace {

    id: _self;

    workspace_title: "Registration"

    fill: () => {
        if(world.currentRef < 0)
            return;

        _source_view_ref.droppedFromManager(world.currentRef);
    }

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    d: GW.WorkspaceRegistration {
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
                window.currentView = _source_view_other
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

            Label {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: G.Style.mediumPadding
                anchors.right: parent.right
                anchors.rightMargin: G.Style.smallPadding

                height: G.Style.smallLabelHeight
                text: "Floating Image"
                font: G.Style.fonts.value
                color: G.Style.colors.textColorNeutral
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

                    viewLogic: d.targetDict;

                    onDroppedFromManager: (index) => {
                        d.targetDict.drop(index);
                    }
                }

                Label {
                    Layout.fillWidth: true;

                    height: G.Style.smallLabelHeight
                    text: "Stack level " + _stack.currentIndex
                    horizontalAlignment: Text.AlignRight
                    font: G.Style.fonts.value
                    color: G.Style.colors.textColorNeutral
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
            }
        }

        G.View {
            id: _source_view_ref;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            onDroppedFromManager: (index) => {
                window.currentView = _source_view_ref
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

            Label {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: G.Style.mediumPadding
                anchors.right: parent.right
                anchors.rightMargin: G.Style.smallPadding

                height: G.Style.smallLabelHeight
                text: "Reference Image"
                horizontalAlignment: Text.AlignRight
                font: G.Style.fonts.value
                color: G.Style.colors.textColorNeutral
            }

            viewLogic: d.sources.views[0];

            Component.onCompleted: G.Associator.associate(_source_view_ref, d.sources.views[0]);
        }


        G.View {
            id: _target_view;

            Layout.fillWidth: true;
            Layout.fillHeight: true;
            property bool _fullscreen: false

            Behavior on Layout.preferredHeight {
                NumberAnimation {
                    duration: 500
                    easing {
                        type: _target_view._fullscreen? Easing.OutQuad : Easing.InQuad
                    }
                }
            }
            Behavior on Layout.preferredWidth {
                NumberAnimation {
                    duration: 500
                    easing {
                        type: _target_view._fullscreen? Easing.OutQuad : Easing.InQuad
                    }
                }
            }

            G.IconButton {
                iconName: _target_view._fullscreen ? G.Icons.icons["fullscreen-exit"] : G.Icons.icons["fullscreen"]
                size: G.Style.iconLarge;
                color: G.Style.colors.textColorNeutral
                tooltip: _target_view._fullscreen ? "Reduce View" : "Expand View"

                anchors.bottom: _registered_label.top
                anchors.right: parent.right
                anchors.margins: G.Style.smallPadding

                onClicked: {
                    if(!_target_view._fullscreen) {
                        _target_view._fullscreen = true
                        _target_view.Layout.preferredHeight = _grid_block.height
                        _target_view.Layout.preferredWidth = _grid_block.width
                    } else {
                        _target_view._fullscreen = false
                        _target_view.Layout.preferredHeight = 0
                        _target_view.Layout.preferredWidth = 0
                    }
                }
            }

            Label {
                id: _registered_label

                anchors.bottom: parent.bottom
                anchors.bottomMargin: G.Style.mediumPadding
                anchors.right: parent.right
                anchors.rightMargin: G.Style.smallPadding

                height: G.Style.smallLabelHeight
                text: "Registered Image"
                horizontalAlignment: Text.AlignRight
                font: G.Style.fonts.value
                color: G.Style.colors.textColorNeutral
            }

            viewLogic: d.target;

            Component.onCompleted: G.Associator.associate(_target_view, d.target);
        }
    }

    Component.onCompleted: {
        d.onParametersChanged();
    }
}
