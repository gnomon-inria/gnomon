import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X
import xQuick.Fonts     1.0 as X
import xQuick.Style     1.0 as X

import gnomonQuick.Controls      1.0 as G

Dialog { id: _self;

    property string type: "Parameter";
    property var code;

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: Math.min(window.width, window.height) / 3 * 2
    height: window.height / 3 * 2

    leftPadding: 0;
    rightPadding: 0;

    parent: Overlay.overlay

    modal: true
    title: "Add "+_self.type

    standardButtons: Dialog.Ok | Dialog.Cancel

    GridLayout { id: _form_layout
        columns: 2

        anchors.fill: parent
        anchors.margins: 10

        rowSpacing: 10
        columnSpacing: 10

        X.Label{
            text: _self.type == "Parameter" ? "Parameter type" : "Form type"
        }

        X.ComboBox { id: _type_combobox
            Layout.fillWidth: true
            Layout.preferredHeight: 28;

            model: _self.type == "Parameter" ? code.parameterTypes : code.formTypes
        }

        X.Label{
            text: "Form plugin"
            visible: _self.type != "Parameter"
        }

        X.ComboBox { id: _data_plugin_combobox
            Layout.fillWidth: true
            Layout.preferredHeight: 28;

            visible: _self.type != "Parameter"

            model: _self.type == "Parameter" ? [] : code.formDataPlugins(_type_combobox.currentValue);
        }

        X.Label{
            text: _self.type == "Parameter" ? "Parameter name" : "Variable name"
        }

        X.TextField { id: _name_field;
            Layout.fillWidth: true;
            Layout.preferredHeight: 28;
        }

        X.Label{
            text: "Parameter documentation"
            visible: _self.type == "Parameter"
        }

        X.TextField { id: _doc_field;
            Layout.fillWidth: true
            Layout.preferredHeight: 28;

            visible: _self.type == "Parameter"
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    onAccepted: {
        var name = _name_field.text
        var type = _type_combobox.currentValue

        if (_self.type == "Parameter") {
            var doc = _doc_field.text
            var param_desc = { "name": name, "type": type, "doc": doc }
            console.log("Add",_self.type,":",param_desc)
            code.addParameter(param_desc)
        } else {
            var data_plugin = _data_plugin_combobox.currentValue
            var form_desc = { "name": name, "type": type, "data_plugin": data_plugin }
            console.log("Add",_self.type,":",form_desc)
            if (_self.type == "Input Form") {
                code.addInputForm(form_desc)
            } else if (_self.type == "Output Form") {
                code.addOutputForm(form_desc)
            }
        }
    }
}
