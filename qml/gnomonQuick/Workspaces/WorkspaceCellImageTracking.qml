import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P

import gnomon.Utils as G
import gnomonQuick.Workspaces as G
import gnomonQuick.Controls as G
import gnomonQuick.Style as G

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

    viewSelected: _source_view

    G.SessionSettings {
        category: uuid
        property alias state: d.state
        property alias lineage_values: _lineage_values.text
    }

    d: GW.WorkspaceCellImageTracking {
        id: d;

        onStarted: idleStart();
        onFinished: idleStop();

        onParametersChanged: {
            _self.updateParametersModel();
        }
    }

    G.Toast {
        id: _picked_cells_toast

        property string picked_cells_number: "";

        parent: Overlay.overlay
        header: "Need at least 4 cell pairings!"
        message: "Not enough cell pairings were provided, the transformation matrix will be estimated from scratch. You added " + picked_cells_number + " cell pairings, but at least 4 are required";

        type: G.Style.ButtonType.Warning
    }

    G.Toast {
        id: _init_trans_matrix

        parent: Overlay.overlay
        header: "Transformation matrix not used!"
        message: "You have provided manual cell pairings, hence the transformation matrix will not be used for cell tracking";

        type: G.Style.ButtonType.Base
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
            clip: true
            //interactive: false;

            Item  {
                id: transfo_view
                G.TextView {
                    id: _data_source_view;

                    anchors.fill: parent

                    onDroppedFromManager: (index) => {
                        d.sourceDict.drop(index);
                    }

                    Label {
                        anchors.top: parent.top
                        anchors.left: parent.left

                        text: "Transformation matrix"
                        color: G.Style.colors.fgColor
                    }

                    Label {
                        anchors.centerIn: parent

                        text: d.sourceDict.dataDict
                        horizontalAlignment: Text.AlignRight
                        color: G.Style.colors.fgColor
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
                        anchors.rightMargin: G.Style.iconLarge + 2*G.Style.smallPadding

                        size: G.Style.iconMedium;
                        color: G.Style.colors.textColorBase

                        iconName: "pencil";

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

                G.TextField {
                    id: _lineage_values

                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.left: _lineage_label.right
                    anchors.margins: G.Style.smallPadding

                    text: ""
                    color: G.Style.colors.hoveredBaseColor
                    font: G.Style.fonts.value

                    onTextEdited: {
                        updateCustomLineageInVisu()
                    }
                }

                G.Button {
                    id: _new_picking;

                    anchors.left: parent.left;
                    anchors.bottom: parent.bottom;
                    anchors.bottomMargin: G.Style.smallPadding;
                    anchors.leftMargin: G.Style.mediumPadding;

                    implicitWidth: G.Style.longButtonWidth

                    type: G.Style.ButtonType.Base
                    text: "Add Cells"
                    hoverEnabled: !is_picking

                    onClicked: {
                        is_picking = true;
                        _new_picking.text = "[ ? || ? ]"
                        d.source.startPicking()
                        d.target.startPicking()
                    }
                }

                G.Button {
                    id: _finish_picking;

                    anchors.right: parent.right;
                    anchors.top:  _new_picking.top;

                    implicitWidth: G.Style.longButtonWidth

                    type: G.Style.ButtonType.Neutral
                    text: "Finish"
                    visible: !is_picking;

                    onClicked: {
                        _new_picking.text = "Add Cells"
                        _swipe.currentIndex = 0;
                        d.setInputs()
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
                        _new_picking.text = "Add Cells"
                        d.source.stopPicking()
                        d.target.stopPicking()
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
                        if(_lineage_values.text) {
                            _lineage_values.text += ", "
                        }
                        _lineage_values.text +=  _new_picking.text.replace('||', ',')
                        _new_picking.text = "Add Cells"
                        updateCustomLineageInVisu()
                        d.source.stopPicking()
                        d.target.stopPicking()
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
        function onPickedCellsChanged() {
            if(d.source.pickedCells.length == 0) {
                _new_picking.text = "[ ? ||" + _new_picking.text.split('||')[1]
            } else if(d.source.pickedCells.length > 1) {
                _new_picking.text = "[ [" + d.source.pickedCells + "] ||" + _new_picking.text.split('||')[1]
            } else {
                _new_picking.text = "[" + d.source.pickedCells[0] + " ||" + _new_picking.text.split('||')[1]
            }
        }
    }

    Connections {
        target: d.target
        function onPickedCellsChanged() {
            _new_picking.text =  _new_picking.text.split('||')[0] + "||"
            if(d.target.pickedCells.length == 0) {
                _new_picking.text += " ? ]"
            } else if(d.target.pickedCells.length > 1) {
                _new_picking.text += " [" + d.target.pickedCells + "] ]"
            } else {
                _new_picking.text += " " + d.target.pickedCells[0] + "]"
            }
        }

        function onFormAdded(formType) {
            console.log(formType)
            if (formType == "gnomonCellImage") {
                updateCustomLineageInVisu()
            }
        }

        function onFormVisualizationChanged() {
            updateCustomLineageInVisu()
        }
    }

    Connections {
        target: d
        function onNotEnoughCells(picked_cells) {
            _picked_cells_toast.picked_cells_number = picked_cells
            _picked_cells_toast.open()
        }
        function onNotInitTrans() {
            _init_trans_matrix.open()
        }
    }

    Component.onCompleted: {
        d.onParametersChanged();
    }

    //"[ [4,5] , [11, [17, 18]] , [ [22], [23,24]]]"
    function updateCustomLineageInVisu()
    {
        let lineage_text = "[ " + _lineage_values.text.replace(/\?/g, '-1') + " ]"
        let lineage_array
        try {
            lineage_array = JSON.parse(lineage_text)
        } catch (e) {
            lineage_array = []
        }

        let source_lineage_idx = []
        let target_lineage_idx = []
        for(const lineage of lineage_array) {
            source_lineage_idx.push(lineage[0])
            target_lineage_idx.push(lineage[1])
        }
        d.source.setFormVisuParameter("gnomonCellImage", "manual_lineage", JSON.stringify(source_lineage_idx))
        console.log(JSON.stringify(target_lineage_idx))
        d.target.setFormVisuParameter("gnomonCellImage", "manual_lineage", JSON.stringify(target_lineage_idx))
    }

}
