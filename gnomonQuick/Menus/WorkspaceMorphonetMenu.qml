import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G


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

        TabButton {
            text: "Download"
        }
        TabButton {
            text: "Upload"
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

                X.ComboBox {
                    id: _datasets
                    Layout.fillWidth: true;
                    Layout.preferredHeight: 28;
                    model: mn_ds_info;
                    displayText: "Choose a Dataset Here";

                    onCurrentIndexChanged: {
                        if (d) {
                            d.currentId = mn_ds_info.get(_datasets.currentIndex).morpho_id;
                            displayText = mn_ds_info.get(_datasets.currentIndex).name
                            _ds_id.text = mn_ds_info.get(_datasets.currentIndex).morpho_id;
                            _ds_date.text = mn_ds_info.get(_datasets.currentIndex).date;
                            _ds_owner.text = mn_ds_info.get(_datasets.currentIndex).owner;
                            _ds_comments.text = mn_ds_info.get(_datasets.currentIndex).comments;
                            _ds_delete.visible = mn_ds_info.get(_datasets.currentIndex).own;

                            // idle.start() ??
                            d.importDatasetPreview(d.currentId,
                                 Number(_voxelsize.text)
                            );
                            _ds_time_start.text = d.timeStart
                            _ds_time_end.text = d.timeEnd
                            //idle.end() ???
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true;

                    X.Label { text: "Id:"; color: X.Style.foregroundColor; font.pixelSize: 14; }
                    X.Label { id: _ds_id; text: "";  font.pixelSize: 12;}
                    X.Label { text: "Date:";  font.pixelSize: 14;}
                    X.Label { id: _ds_date; text: "";  font.pixelSize: 12;}
                }

                RowLayout {
                    Layout.fillWidth: true;

                    X.Label { text: "Owner:"; color: X.Style.foregroundColor; font.pixelSize: 14; }
                    X.Label { id: _ds_owner; text: "";  font.pixelSize: 12;}
                    X.ButtonRaw {
                        id: _ds_delete; 
                        text: "Delete";
                        onClicked: {
                           d.deleteDataset();
                           d.updateDatasetsInfo();
                        }
                    }
                }

                X.Label { text: "Comments:";  font.pixelSize: 14;}
                X.Label {
                    id: _ds_comments;
                    Layout.fillWidth: true;

                    text: "";
                    font.pixelSize: 10;
                    wrapMode: Label.WordWrap
                }
                /*

                RowLayout {
                    Layout.fillWidth: true;

                    X.Label {
                        text: "Image Dims:";
                        color: X.Style.foregroundColor;
                        font.pixelSize: 14;
                    }
                    X.TextField { id: _dim_x; text: "100"; errorText: "bad Value";
                        validator: IntValidator{bottom: 1; top: 10000;}
                    }
                    X.TextField {id: _dim_y; text: "100"; errorText: "bad Value";
                        validator: IntValidator{bottom: 1; top: 10000;}
                    }
                    X.TextField {id: _dim_z; text: "100"; errorText: "bad Value";
                        validator: IntValidator{bottom: 1; top: 10000;}
                    }

                    MouseArea{
                        id: _dims_ma
                        Layout.fillWidth: true;
                        Layout.fillHeight: true
                        hoverEnabled: true
                        propagateComposedEvents: true
                    }

                    X.ToolTip {
                        visible: _dims_ma.containsMouse
                        text: "image dimensions (x,y,z) in range [1, 10000]"
                    }

                }
                */

                RowLayout {
                    Layout.fillWidth: true;

                    X.Label {
                        text: "Voxel size:";
                        color: X.Style.foregroundColor;
                        font.pixelSize: 14;
                    }
                    X.TextField { id: _voxelsize; text: "0.20"; errorText: "bad Value";
                        //validator: DoubleValidator{bottom: 0.01; top: 10; locale: Qt.locale("en"); notation: DoubleValidator.StandardNotation}
                    }

                    MouseArea{
                        id: _voxsize_ma
                        Layout.fillWidth: true;
                        Layout.fillHeight: true
                        hoverEnabled: true
                        propagateComposedEvents: true
                    }

                    X.ToolTip {
                        visible: _voxsize_ma.containsMouse
                        text: "Voxel size (x,y,z) in range [0.01, 10]"
                    }

                }

                RowLayout {
                    Layout.fillWidth: true;

                    X.Label { text: "Time start:"; color: X.Style.foregroundColor; font.pixelSize: 14; }
                    X.Label { id: _ds_time_start; text: "";  font.pixelSize: 12;}
                    X.Label { text: "Time  End:";  font.pixelSize: 14;}
                    X.Label { id: _ds_time_end; text: "";  font.pixelSize: 12;}
                }

                X.ButtonRaw {
                    Layout.fillWidth: true;
                    text: "import";

                    onClicked: {
                        //_progress.open();
                        //_progress.start();
                        console.info('Importing selected dataset from morphonet!')
                        //d.importDataset(time_start, time_end)
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

                RowLayout {
                    Layout.fillWidth: true;
                    X.Label { text: "Name:"; color: X.Style.foregroundColor; font.pixelSize: 14; }
                    X.TextField { id: _up_name; helperText: "New Dataset Name"; }
                }

                RowLayout {
                    Layout.fillWidth: true;
                    X.Label { text: "NCBI:"; color: X.Style.foregroundColor; font.pixelSize: 14; }
                    X.TextField {
                        id: _up_ncbi;
                        placeholderText: qsTr("NCBI specie if available");
                        text: "0";
                        errorText: "bad Value";
                        validator: IntValidator{bottom: 0; top: 10000; }
                    }
                }


                RowLayout {
                    Layout.fillWidth: true;
                    X.Label { text: "type:"; color: X.Style.foregroundColor; font.pixelSize: 14; }
                    X.TextField {
                        id: _up_type;
                        placeholderText: qsTr("type : 0, 1, 2");
                        text: "0";
                        errorText: "bad Value";
                        validator: IntValidator{bottom: 0; top: 2; }
                    }
                }


                RowLayout {
                    Layout.fillWidth: true;
                    X.Label { text: "Description:"; color: X.Style.foregroundColor; font.pixelSize: 14; }
                    X.TextField {
                        id: _up_description;
                        placeholderText: qsTr("Description");
                        text: "";
                    }
                }

                X.ButtonRaw {
                    id: _dataset_created
                    Layout.fillWidth: true;

                    property var link : ""

                    text: "Dataset Uploaded with Id: XXXXX";
                    font.pixelSize: 16;
                    visible: false
                    enabled: false

                    onClicked: {
                        Qt.openUrlExternally(_dataset_created.link);
                    }
                }

                X.ButtonRaw {
                    id: _upload_button
                    Layout.fillWidth: true;
                    text: "Create dataset and Upload."

                    onClicked: {
                        //_progress.open();
                        //_progress.start();
                        console.info('Importing selected dataset from morphonet!')
                        var res = d.exportDataset(_up_name.text, Number(_up_ncbi.text), Number(_up_type.text), _up_description)
                        if( res != -1) {
                            let new_text = "Dataset Uploaded with Id: %1"
                            _dataset_created.text = new_text.arg(res)
                            _dataset_created.visible = true
                            _dataset_created.enabled = true
                            let new_link = "https://morphonet.org/morphoapi?id_dataset=%1"
                            _dataset_created.link = new_link.arg(res)
                            _dataset_created.contentItem.color = "green"
                            // _upload_button.enabled = false //TODO deactivate new upload if successfull
                        } else {
                            _dataset_created.text = "[Error] Dataset Not Uploaded"
                            _dataset_created.contentItem.color = "red"
                            _dataset_created.visible = true
                            _dataset_created.enabled =  false
                        }
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
