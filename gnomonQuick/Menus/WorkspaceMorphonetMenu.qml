import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G

Control {

    id: _menu;

    property var d
    property var mn_ds_info

    TabBar {
        id: _bar;

        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;

        currentIndex: _container.currentIndex

        G.TabButton {
            text: "Download"
            //Component.onCompleted: { contentItem.color = X.Style.foregroundColor }
        }
        G.TabButton {
            text: "Upload"
            //Component.onCompleted: { contentItem.color = X.Style.foregroundColor }
        }
        G.TabButton {
            text: "Curate"
            //Component.onCompleted: { contentItem.color = X.Style.foregroundColor }
        }
    }

    StackLayout {
        id: _container;

        anchors.top: _bar.bottom;
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        anchors.left: parent.left;
        anchors.topMargin: 15;

        currentIndex: _bar.currentIndex;

        Control {
            //anchors.fill: parent
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                id: selected_ds_info

                anchors.fill: parent
                anchors.margins: 10

                Label {
                    Layout.fillWidth: true;
                    text: "Existing Dataset";
                    color: G.Style.colors.textColorBase;
                    font: G.Style.fonts.header
                }

                G.ComboBox {
                    id: _datasets
                    Layout.fillWidth: true;
                    Layout.preferredHeight: 28;
                    model: mn_ds_info;
                    displayText: "Choose a Dataset Here";
                    currentIndex: -1

                    onCurrentIndexChanged: {
                        if (d && _datasets.currentIndex != -1) {
                            d.currentId = mn_ds_info.get(_datasets.currentIndex).morpho_id;
                            displayText = mn_ds_info.get(_datasets.currentIndex).name
                            _ds_id.text = mn_ds_info.get(_datasets.currentIndex).morpho_id;
                            _ds_date.text = mn_ds_info.get(_datasets.currentIndex).date;
                            _ds_owner.text = mn_ds_info.get(_datasets.currentIndex).owner;
                            _ds_comments.text = mn_ds_info.get(_datasets.currentIndex).comments;
                            _ds_delete.visible = mn_ds_info.get(_datasets.currentIndex).own;

                            //idle.start()
                            d.importDataset(d.currentId,
                                 Number(_voxelsize.text)
                            );
                            _ds_time_start.text = d.timeStart
                            _ds_time_end.text = d.timeEnd

                            row_times_tooltip.text = "times in range [" + d.timeStart + ", " + d.timeEnd + "]"
                            //idle.end()
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true;

                    Label { text: "Id:"; color: G.Style.colors.textColorBase; font: G.Style.fonts.formLabel; }
                    Label { id: _ds_id; text: ""; color: G.Style.colors.textColorBase; font: G.Style.fonts.value;}
                    Label { text: "Date:"; color: G.Style.colors.textColorBase; font: G.Style.fonts.formLabel;}
                    Label { id: _ds_date; text: ""; color: G.Style.colors.textColorBase; font: G.Style.fonts.value;}
                }

                RowLayout {
                    Layout.fillWidth: true;

                    Label { text: "Owner:"; color: G.Style.colors.textColorBase; font: G.Style.fonts.formLabel; }
                    Label { id: _ds_owner; text: ""; color: G.Style.colors.textColorBase; font: G.Style.fonts.value;}
                    G.Button {
                        id: _ds_delete;
                        text: "Delete";
                        visible: false;
                        type: G.Style.ButtonType.Danger
                        flat: true
                        onClicked: {
                           d.deleteDataset();
                           d.setDatasetInfos();
                        }
                    }
                }

                Label { text: "Description:"; color: G.Style.colors.textColorBase; font: G.Style.fonts.formLabel;}
                Label {
                    id: _ds_comments;
                    Layout.fillWidth: true;
                    Layout.minimumHeight: selected_ds_info.height/4;

                    text: "";
                    color: G.Style.colors.textColorBase;
                    font: G.Style.fonts.label;
                    wrapMode: Label.WordWrap
                }


                RowLayout {
                    Layout.fillWidth: true;

                    Label {
                        text: "Voxel size:";
                        color: G.Style.colors.textColorBase;
                        font: G.Style.fonts.formLabel;
                    }
                    G.TextField {
                        id: _voxelsize;
                        text: "1.0";
                        color: G.Style.colors.textColorBase;
                        font: G.Style.fonts.value;
                        //errorText: "Incorrect value";
                        Layout.fillWidth: true;
                        //validator: DoubleValidator{bottom: 0.01; top: 10; locale: Qt.locale("en"); notation: DoubleValidator.StandardNotation}
                    }

                    G.ToolTip {
                        visible: _voxelsize.hovered
                        text: "Voxel size (x,y,z) in range [0.01, 10]"
                    }

                }

                RowLayout {
                    Layout.fillWidth: true;

                    Label { text: "Time start:"; color: G.Style.colors.textColorBase; font: G.Style.fonts.formLabel; }
                    G.TextField { id: _ds_time_start;
                        Layout.fillWidth: true;
                        text: "";
                        color: G.Style.colors.textColorBase;
                        font: G.Style.fonts.value;
                        //errorText: "Incorrect value";
                        validator: IntValidator{bottom: 0; top: 10000;}
                    }
                    Label { text: "Time  End:"; color: G.Style.colors.textColorBase; font: G.Style.fonts.formLabel;}
                    G.TextField { id: _ds_time_end; text: "";
                        color: G.Style.colors.textColorBase;
                        font: G.Style.fonts.value;
                        Layout.fillWidth: true;
                        //errorText: "Incorrect value";
                        validator: IntValidator{bottom: 0; top: 10000;}
                    }
                    G.ToolTip {
                        id: row_times_tooltip
                        visible: _ds_time_end.hovered || _ds_time_start.hovered
                        text: "times in range [0, 10000]"
                    }
                }

                Item {
                    Layout.fillHeight: true;
                }

                G.Button {
                    Layout.fillWidth: true;
                    text: "Download";
                    type: G.Style.ButtonType.Base
                    flat: false

                    onClicked: {
                        //_progress.open();
                        //_progress.start();
                        console.info('Downloading selected dataset from MorphoNet!')
                        d.importDataset(d.currentId, Number(_voxelsize.text), Number(_ds_time_start.text), Number(_ds_time_end.text))
                    }
                }
            }
        }

        Control {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                id: upload_ds_info

                anchors.fill: parent
                anchors.margins: 10

                Label {
                    Layout.fillWidth: true;
                    text: "New Dataset";
                    color: G.Style.colors.textColorBase;
                    font: G.Style.fonts.header
                }

                RowLayout {
                    Layout.fillWidth: true;
                    Label { text: "Name:"; color: G.Style.colors.textColorBase; font: G.Style.fonts.formLabel; }
                    G.TextField {
                        id: _up_name;
                        Layout.fillWidth: true;
                        helperText: "New Dataset Name";
                        color: G.Style.colors.textColorBase;
                        font: G.Style.fonts.value;
                    }
                }

                RowLayout {
                    Layout.fillWidth: true;
                    Label { text: "NCBI:"; color: G.Style.colors.textColorBase; font: G.Style.fonts.formLabel; }
                    G.TextField {
                        id: _up_ncbi;
                        Layout.fillWidth: true;
                        placeholderText: qsTr("NCBI specie if available");
                        text: "0";
                        color: G.Style.colors.textColorBase;
                        font: G.Style.fonts.value;
                        //errorText: "Incorrect value";
                        validator: IntValidator{bottom: 0; top: 10000; }
                    }
                    G.ToolTip {
                        visible: _up_ncbi.hovered
                        text: "NCBI id see: https://www.ncbi.nlm.nih.gov"
                    }

                }

                RowLayout {
                    Layout.fillWidth: true;
                    Label { text: "Type:"; color: G.Style.colors.textColorBase; font: G.Style.fonts.formLabel; }
                    G.ComboBox { id: _up_type
                        model: ["0: Observed", "1: Simulated", "2: Drawing"]
                        currentIndex: 0
                        Layout.fillWidth: true;
                    }
                    G.ToolTip {
                        visible: _up_type.hovered
                        text: " 0 for Observed Data, 1 for Simulated Data, 2 for Drawing Data"
                    }
                }

                RowLayout {
                    Layout.fillWidth: true;
                    Label { text: "Description:"; color: G.Style.colors.textColorBase; font: G.Style.fonts.formLabel; }
                    G.TextField {
                        id: _up_description;
                        Layout.fillWidth: true;
                        text: "(Uploaded from Gnomon)";
                        color: G.Style.colors.textColorBase;
                        font: G.Style.fonts.value;
                    }
                }

                RowLayout {
                    Layout.fillWidth: true;

                    Label {
                        text: "Voxel size:";
                        color: G.Style.colors.textColorBase;
                        font: G.Style.fonts.formLabel;
                    }
                    G.TextField {
                        id: _upload_voxelsize;
                        Layout.fillWidth: true;
                        text: "1.0";
                        color: G.Style.colors.textColorBase;
                        font: G.Style.fonts.value;
                        //errorText: "Incorrect value";
                        //validator: DoubleValidator{bottom: 0.01; top: 10; locale: Qt.locale("en"); notation: DoubleValidator.StandardNotation}
                    }

                    G.ToolTip {
                        visible: _upload_voxelsize.hovered
                        text: "Voxel size (x,y,z) for mesh generation"
                    }
                }

                Item {
                    Layout.fillHeight: true
                }

                G.Button {
                    id: _dataset_created
                    Layout.fillWidth: true;

                    type: G.Style.ButtonType.Base
                    flat: true

                    property var link : ""

                    text: "Dataset Uploaded with Id: XXXXX";
                    font.pixelSize: 16;
                    visible: false
                    enabled: false

                    onClicked: {
                        Qt.openUrlExternally(_dataset_created.link);
                    }
                }

                G.Button {
                    id: _sync_infos
                    Layout.fillWidth: true;

                    type: G.Style.ButtonType.Base
                    flat: true

                    text: "Sync cell infos";
                    visible: false

                    onClicked: {
                        console.info('Downloading Morphonet cell infos...')
                        var res = d.importDatasetInfos()
                        if( res != -1) {
                            console.log('Successfully upated cell infos!')
                        }
                    }

                    G.Icon {
                        icon: "sync";
                        color: G.Style.colors.textColorBase;

                        anchors.left: _sync_infos.left;
                        anchors.leftMargin: 10;
                        anchors.verticalCenter: _sync_infos.verticalCenter;
                    }
                }

                G.Button {
                    id: _upload_button
                    Layout.fillWidth: true;
                    type: G.Style.ButtonType.Base
                    flat: false
                    text: "Create Dataset and Upload"

                    onClicked: {
                        //_progress.open();
                        //_progress.start();
                        var res = d.exportDataset(_up_name.text, Number(_up_ncbi.text), _up_type.currentIndex, _up_description, Number(_upload_voxelsize.text))
                        if( res != -1) {
                            let new_text = "Dataset Uploaded with Id: %1"
                            _dataset_created.text = new_text.arg(res)
                            _dataset_created.visible = true
                            _dataset_created.enabled = true
                            let new_link = "https://morphonet.org/morphoapi?id_dataset=%1"
                            _dataset_created.link = new_link.arg(res)
                            _dataset_created.contentItem.color = "green"
                            // _upload_button.enabled = false //TODO deactivate new upload if successfull

                            _sync_infos.visible = true

                        } else {
                            _dataset_created.text = "[Error] Dataset Not Uploaded"
                            _dataset_created.contentItem.color = "red"
                            _dataset_created.visible = true
                            _dataset_created.enabled =  false

                            _sync_infos.visible = false
                        }
                    }
                }
            }
        }
        Control {
            //anchors.fill: parent
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent;
                anchors.margins: G.Style.smallPadding;

                spacing: G.Style.smallPadding


                RowLayout {
                    Layout.fillWidth: true;

                    Label {
                        text: "Voxel size:";
                        color: G.Style.colors.textColorBase;
                        font: G.Style.fonts.formLabel;
                    }
                    G.TextField {
                        id: _curation_voxelsize;
                        text: "0.5";
                        color: G.Style.colors.textColorBase;
                        font: G.Style.fonts.value;
                        //errorText: "Incorrect value";
                        Layout.fillWidth: true;
                        //validator: DoubleValidator{bottom: 0.01; top: 10; locale: Qt.locale("en"); notation: DoubleValidator.StandardNotation}
                    }

                    G.ToolTip {
                        visible: _curation_voxelsize.hovered
                        text: "Voxel size (x,y,z) for mesh generation"
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                G.Button {
                    id: _launch_morphoplot
                    Layout.fillWidth: true

                    type: G.Style.ButtonType.Base
                    flat: !enabled
                    //hoverEnabled: enabled
                    text: "Launch Curation";
                    onClicked: {
                        _launch_morphoplot.enabled = false;
                        let res = d.morphoPlot(Number(_curation_voxelsize.text));
                        if(res == 0) {
                            _launch_morphoplot.enabled = false;
                            _collect_morphoplot.enabled = true;
                        } else {
                            _launch_morphoplot.enabled = true;
                        }
                    }
                }

                G.Button {
                    id: _collect_morphoplot
                    Layout.fillWidth: true

                    type: G.Style.ButtonType.Base
                    flat: !enabled
                    hoverEnabled: false
                    enabled: false;
                    text: "Collect Results";
                    onClicked: {
                        d.morphoPlotCollect();
                        _launch_morphoplot.enabled = true;
                        _collect_morphoplot.enabled = false;
                    }
                }
            }
        }
    }

    Connections {
        target: d
        function onUploadModeChanged() {
            _bar.setCurrentIndex(d.uploadMode ? 1 : 0)
         }
    }

    background: Rectangle { color: "#00000000"; }
}
