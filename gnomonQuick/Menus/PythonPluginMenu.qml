import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G
import gnomonQuick.Icons      1.0 as G

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
        anchors.margins: G.Style.smallPadding;

        Control {
            Layout.fillWidth: true;
            height: G.Style.iconSmall

            Label {
                text: _self.type + "s";

                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter

                font: G.Style.fonts.subHeader;
                color: G.Style.colors.textColorBase;
                horizontalAlignment: Text.AlignLeft;
                verticalAlignment: Text.AlignVCenter;
            }

            G.IconButton {
                iconName: G.Icons.icons["plus"]
                tooltip: "Add " + _self.type
                hoverColor: G.Style.colors.hoveredBaseColor

                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                onClicked: {
                    _plugin_dialog.clear();
                    _plugin_dialog.open();
                }
            }
        }

        G.Gutter {
            Layout.fillWidth: true;
            Layout.fillHeight: true;

            ListView {

                id: _list_view

                anchors.fill: parent
                clip: true;

                model: _self.type == "Parameter" ?
                       list_model(d.code.parameters) :
                       _self.type == "Input Form"  ? list_model(d.code.inputForms)  :
                       _self.type == "Output Form" ? list_model(d.code.outputForms) : [] ;

                delegate: G.ListItemDelegate {
                    width: _list_view.width
                    highlighted: false

                    Label {
                        anchors.left: parent.left;
                        anchors.top: parent.top;
                        anchors.margins: 10;

                        text: _self.type == "Parameter" ?
                              modelData.name + " (" + modelData.type + ") = " + modelData.value:
                              modelData.name + " (" + (modelData.type).replace("gnomon","") + ")";

                        color: G.Style.colors.textColorBase;
                        font: G.Style.fonts.formLabel;
                    }

                    Label {
                        anchors.right: parent.right;
                        anchors.bottom: parent.bottom;
                        anchors.margins: 10;

                        text: _self.type == "Parameter" ?
                              modelData.doc :
                              modelData.data_plugin;

                        color: G.Style.colors.textColorBase;
                        font: G.Style.fonts.value;
                    }
                }

                ScrollIndicator.vertical: ScrollIndicator { }
            }
        }
    }

    background: Rectangle {
        anchors.fill: parent
        color: G.Style.colors.fgColor;
        radius: G.Style.panelRadius;

        //border.color: G.Style.colors.embossColor
        //border.width: 1
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
