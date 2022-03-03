import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G

Control {

    id: _self

    property var model;
    property var code;
    property string type: "Parameter";

    Layout.fillWidth: true;
    Layout.fillHeight: true;

    ColumnLayout
    {
        anchors.fill: parent;
        anchors.margins: 10;

        X.Label {
            text: _self.type + "s";
            Layout.fillWidth: true;
            height: 20

            color: X.Style.foregroundColor;
            horizontalAlignment: Text.AlignLeft;
        }

        ListView {

            id: _list_view

            spacing: 10;
            clip: true;

            model: _self.type == "Parameter" ?
                   list_model(d.code.parameters) :
                   _self.type == "Input Form"  ? list_model(d.code.inputForms)  :
                   _self.type == "Output Form" ? list_model(d.code.outputForms) : [] ;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            delegate: ItemDelegate {
                width: _list_view.width
                highlighted: ListView.isCurrentItem

                X.Label {
                    anchors.left: parent.left;
                    anchors.top: parent.top;
                    anchors.margins: 10;

                    text: _self.type == "Parameter" ?
                          modelData.name + " (" + modelData.type + ")" :
                          modelData.name + " (" + (modelData.type).replace("gnomon","") + ")";

                    color: X.Style.foregroundColor;
                    font.pixelSize: 12;
                }

                X.Label {
                    anchors.right: parent.right;
                    anchors.bottom: parent.bottom;
                    anchors.margins: 10;

                    text: _self.type == "Parameter" ?
                          modelData.doc :
                          modelData.data_plugin;

                    color: X.Style.foregroundColor;
                    font.pixelSize: 8;
                }

                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 50

                    radius: 4
                    color: X.Style.backgroundColor
                }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        X.ButtonRaw {
            text: "Add "+ _self.type + "...";

            Layout.fillWidth: true;

            onClicked: {
                _plugin_dialog.open();
            }
        }
    }

    background: Rectangle {
        color: Qt.darker(X.Style.backgroundColor);
        radius: 4;

        border.width: 1;
        border.color: X.Style.borderColor;
    }

    G.PythonPluginDialog {
        id: _plugin_dialog;

        type: _self.type
        code: _self.code
    }

    function list_model(json_object) {
        var l = [];
        for(var key in json_object) {
            var item = json_object[key];
            item["key"] = key;
            l.push(item)
        }
        return l;
    }
}
