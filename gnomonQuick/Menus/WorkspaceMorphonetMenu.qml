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
                    _ds_comments.text = mn_ds_info.get(_datasets.currentIndex).comments;

                    // idle.start() ?? 
                    d.importDatasetPreview(d.currentId);
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

        X.Label { text: "Comments:";  font.pixelSize: 14;}
        X.Label {
            id: _ds_comments;
            Layout.fillWidth: true;

            text: "";
            font.pixelSize: 10;
            wrapMode: Label.WordWrap
        }
        
        RowLayout {
            Layout.fillWidth: true;
 
            X.Label { text: "Time start:"; color: X.Style.foregroundColor; font.pixelSize: 14; }
            X.Label { id: _ds_time_start; text: "";  font.pixelSize: 12;}
            X.Label { text: "Time  End:";  font.pixelSize: 14;}
            X.Label { id: _ds_time_end; text: "";  font.pixelSize: 12;}
        }

        X.ButtonRaw {
            text: "import";

            onClicked: {
                //_progress.open();
                //_progress.start();
                console.info('Importing selected dataset from morphonet!')
                //d.importDataset(time_start, time_end)
            }

          

            Layout.fillWidth: true;
        }
    }

    background: Rectangle { color: "#00000000"; }
}
