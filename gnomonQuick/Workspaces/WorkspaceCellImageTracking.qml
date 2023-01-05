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

import gnomonQuick.Workspaces as G
import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G

import gnomon.Workspaces 1.0 as GW

G.Workspace {

    id: _self;

    workspace_title: "Tracking";
    property bool is_picking: false;

    fill: () => {
        if(world.currentRef < 0)
            return;

        _source_view.droppedFromManager(world.currentRef);
    }

    d: GW.WorkspaceCellImageTracking {
        id: d;

        onStarted: idleStart();
        onFinished: idleStop();

        onParametersChanged: {
            _self.updateParametersModel();
        }
    }

    ColumnLayout {

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.margins: G.Style.smallPadding;

        Layout.fillWidth: true;
        Layout.fillHeight: true;

        SwipeView {
            id: _swipe

            Layout.fillWidth: true;
            height: window.height/8;
            currentIndex: 0;
            //interactive: false;

            Item  {
                id: transfo_view
                G.DataDict {
                    id: _data_source_view;

                    anchors.fill: parent

                    onDroppedFromManager: (index) => {
                        console.info('Retrieving from manager');
                        d.sourceDict.drop(index);
                    }

                    X.Label {
                        anchors.top: parent.top
                        anchors.left: parent.left

                        text: "Transformation matrix"
                        color: X.Style.foregroundColor
                    }

                    X.Label {
                        anchors.centerIn: parent

                        text: d.sourceDict.dataDict
                        horizontalAlignment: Text.AlignRight
                        color: X.Style.foregroundColor
                        font {
                            pointSize: 14
                            bold: true
                        }
                    }

                    G.IconButton {
                        id: _enable_picking;

                        anchors.top: parent.top
                        anchors.topMargin: G.Style.smallPadding
                        anchors.right: parent.right
                        anchors.rightMargin: G.Style.largePadding

                        size: G.Style.iconMedium;
                        color: G.Style.colors.textColorBase

                        iconName: G.Icons.icons["pencil"];

                        onClicked: {
                            _swipe.currentIndex = 1;
                        }

                    }

                    viewLogic: d.sourceDict;
                }
            }
            Item {
                id: picker_view

                Label {
                    id: _lineage_label
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.leftMargin: G.Style.smallPadding

                    text: "Manual lineage"
                    font: G.Style.fonts.value
                    color: G.Style.colors.textColorBase
                }

                TextField {
                    id: _lineage_values

                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.left: _lineage_label.right
                    anchors.margins: G.Style.smallPadding

                    text: ""
                    color: G.Style.colors.hoveredBaseColor
                    font: G.Style.fonts.value
                }

                G.Button {
                    id: _new_picking;

                    anchors.left: parent.left;
                    anchors.bottom: parent.bottom;
                    anchors.bottomMargin: G.Style.smallPadding;
                    anchors.leftMargin: G.Style.mediumPadding;

                    implicitWidth: G.Style.longButtonWidth

                    type: G.Style.ButtonType.Base
                    text: "New Picking"
                    hoverEnabled: !is_picking

                    onClicked: {
                        console.log("picking enabled !");
                        is_picking = true;
                        _new_picking.text = "( ? , ? )"
                        d.source.startPicking()
                    }
                }

                G.Button {
                    id: _finish_picking;

                    anchors.right: parent.right;
                    anchors.top:  _new_picking.top;

                    implicitWidth: G.Style.longButtonWidth

                    type: G.Style.ButtonType.Neutral
                    text: "Finish Picking"
                    visible: !is_picking;

                    onClicked: {
                        console.log("picking finished !");
                        _new_picking.text = "New Picking"
                        _swipe.currentIndex = 0;
                    }
                }

                G.Button {
                    id: _picking_cancel;

                    anchors.right: parent.right;
                    anchors.top:  _new_picking.top;

                    implicitWidth: G.Style.buttonWidth

                    type: G.Style.ButtonType.Warning
                    text: "Cancel"

                    visible: is_picking;

                    onClicked: {
                        console.log("picking cancelled");
                        _new_picking.text = "New Picking"
                        d.source.stopPicking()
                        is_picking = false;
                    }
                }

                G.Button {
                    id: _picking_ok;

                    anchors.right: _picking_cancel.left;
                    anchors.top: _new_picking.top;
                    anchors.rightMargin: G.Style.smallPadding

                    implicitWidth: G.Style.shortButtonWidth

                    type: G.Style.ButtonType.OK
                    text: "OK"

                    visible: is_picking;

                    onClicked: {
                        console.log("picking OK!");
                        _lineage_values.text += _new_picking.text + " , "
                        _new_picking.text = "New Picking"
                        d.source.stopPicking()
                        is_picking = false;
                    }
                }
            }
        }

        Control {

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            RowLayout {
                anchors.fill: parent


                G.View {

                    id: _source_view;

                    Layout.fillWidth: true;
                    Layout.fillHeight: true;

                    onDroppedFromManager: (index) => {
                        console.info('Retrieving from manager');
                        window.currentView = _source_view
                        d.source.drop(index);
                    }

                    viewLogic: d.source;
                    ts_slider.value: Math.max(d.target.currentTime-1, ts_slider.from)

                    Component.onCompleted: G.Associator.associate(_source_view, d.source);
                }

                G.View {

                    id: _target_view;

                    Layout.fillWidth: true;
                    Layout.fillHeight: true;

                    viewLogic: d.target;

                    ts_slider.to: d.source.timeMax
                    ts_slider.value: Math.min(d.source.currentTime+1, ts_slider.to)

                    ts_slider.enabled: true;
                    ts_slider.visible: true;

                    Component.onCompleted: G.Associator.associate(_target_view, d.target);
                }
            }
        }
    }

    Connections {
        target: d.source
        function onPickedCell(cell_id) {
            console.log("cell picked from source " , cell_id);
            _new_picking.text = "(" + cell_id + ", " + _new_picking.text.split(',')[1]
        }

        //TODO target

    }

    Component.onCompleted: {
        d.onParametersChanged();
    }
}
