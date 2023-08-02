import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls 1.0 as G
import gnomonQuick.Style    1.0 as G

G.Dialog { id: _self;

    property string type: "Parameter";
    property var code;

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: G.Style.mediumDialogWidth
    height: G.Style.mediumDialogHeight

    leftPadding: 0;
    rightPadding: 0;

    parent: Overlay.overlay

    modal: true
    title: "Add "+_self.type

    standardButtons: Dialog.Ok | Dialog.Cancel

    GridLayout { id: _form_layout
        columns: 2
        clip: true

        anchors.fill: parent
        anchors.margins: G.Style.smallPadding

        rowSpacing: G.Style.smallPadding
        columnSpacing: G.Style.smallPadding

        Label{
            text: _self.type == "Parameter" ? "Parameter type" : "Form type"
        }

        G.ComboBox { id: _type_combobox
            Layout.fillWidth: true
            Layout.preferredHeight: 28;

            model: _self.type == "Parameter" ? code.parameterTypes : code.formTypes
        }

        Label{
            text: "Form plugin"
            visible: _self.type != "Parameter"
        }

        G.ComboBox { id: _data_plugin_combobox
            Layout.fillWidth: true
            Layout.preferredHeight: 28;

            visible: _self.type != "Parameter"

            model: _self.type == "Parameter" ? [] : code.formDataPlugins(_type_combobox.currentValue);
        }

        Label{
            text: _self.type == "Parameter" ? "Parameter name" : "Variable name"
        }

        G.TextField { id: _name_field;
            property int counter: 0;
            Layout.fillWidth: true;
            Layout.preferredHeight: 28;
            //text: "property".concat("_", counter.toString());
            text: "property";

        }

        Label{
            text: "Parameter documentation"
            visible: _self.type == "Parameter"
        }

        G.TextField { id: _doc_field;
            Layout.fillWidth: true
            Layout.preferredHeight: 28;

            visible: _self.type == "Parameter"
        }


        Label{
            text: "Parameter value"
            visible: _self.type == "Parameter"
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 28;

            Label{
                text: "Value"
            }

            G.Switch {
                id: _bool_value
                visible: _type_combobox.currentValue == "Bool"
            }

            G.TextField  {
                id: _num_value_input;

                Layout.preferredWidth: G.Style.sizes.s7
                placeholderText: qsTr("1")
                visible: _type_combobox.currentValue == "Double" || _type_combobox.currentValue == "Int"

                validator: DoubleValidator{
                    notation: DoubleValidator.StandardNotation
                }
                //errorText: "Enter a number";
            }


            G.TextField  {
                id: _str_value_input;

                Layout.preferredWidth: G.Style.sizes.s10
                placeholderText: _type_combobox.currentValue == "String" ? qsTr("\"\"") : qsTr("[\"\"]")
                visible: _type_combobox.currentValue == "String" || _type_combobox.currentValue == "StringList"

                // validator: RegExpValidator{
                //     regExp: _type_combobox.currentValue == "String" ? "[\'\"].*[\'\"]" : "\[[\'\"].*[\'\"]+\]";
                // }
            }
        }

        Label{
            text: "Parameter arguments"
            visible: _self.type == "Parameter" && _type_combobox.currentValue != "Bool"
        }

        RowLayout { id: _args_field;
            Layout.fillWidth: true
            Layout.preferredHeight: 28;

            Label{
                text: "Min"
                Layout.leftMargin: G.Style.sizes.s4
                visible: _type_combobox.currentValue == "Double" || _type_combobox.currentValue == "Int"
            }

            G.TextField  {
                id: _min_input;

                Layout.preferredWidth: G.Style.sizes.s7
                placeholderText: qsTr("0")
                visible: _type_combobox.currentValue == "Double" || _type_combobox.currentValue == "Int"

                validator: DoubleValidator{
                    notation: DoubleValidator.StandardNotation
                }
                //errorText: "Enter a double";
            }

            Label{
                text: "Max"
                Layout.leftMargin: G.Style.sizes.s4
                visible: _type_combobox.currentValue == "Double" || _type_combobox.currentValue == "Int"
            }

            G.TextField  {
                id: _max_input;

                Layout.preferredWidth: G.Style.sizes.s7
                placeholderText: qsTr("10")
                visible: _type_combobox.currentValue == "Double" || _type_combobox.currentValue == "Int"

                validator: DoubleValidator{
                    notation: DoubleValidator.StandardNotation
                }
                //errorText: "Enter a number";
            }

            Label{
                text: "Decimals"
                Layout.leftMargin: G.Style.sizes.s4
                visible: _type_combobox.currentValue == "Double"
            }

           G.TextField  {
                id: _decimals_input;

                Layout.preferredWidth:  G.Style.sizes.s7
                visible: _type_combobox.currentValue == "Double"
                placeholderText: qsTr("1")

                validator: DoubleValidator{
                    bottom: 0
                    top: 1000
                    decimals: 4
                    notation: DoubleValidator.StandardNotation
                }
                //errorText: "Enter a number";
            }


           Label{
                text: "List"
                Layout.leftMargin: G.Style.sizes.s4
                visible: _type_combobox.currentValue == "String" || _type_combobox.currentValue == "StringList"
            }

           G.TextField  {
                id: _string_list_input;

                Layout.preferredWidth:  G.Style.sizes.s10
                visible: _type_combobox.currentValue == "String" || _type_combobox.currentValue == "StringList"
                placeholderText: qsTr("[\"\"]")

                // validator: RegExpValidator{
                //     regExp: "\[[\'\"].*[\'\"]+\]";
                // }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    function clear() {
        if(_self.type == "Parameter") {
            _name_field.text = "property".concat("_", _name_field.counter.toString())
        } else if(_self.type == "Input Form") {
            _name_field.text = "input".concat("_", _name_field.counter.toString())
        } else if(_self.type == "Output Form") {
            _name_field.text = "output".concat("_", _name_field.counter.toString())
        }
        _type_combobox.currentIndex = 0
        if (_self.type == "Parameter") {
            _doc_field.text = ""
            _num_value_input.text = ""
            _str_value_input.text = ""
            _min_input.text = ""
            _max_input.text = ""
            _decimals_input.text = ""
            _bool_value.checked = false
        } else {
            _data_plugin_combobox.currentIndex = 0
        }
    }

    onAccepted: {
        var name = _name_field.text
        var type = _type_combobox.currentValue
        _name_field.counter += 1

        if (_self.type == "Parameter") {
            var doc = _doc_field.text
            let value = ""
            let args = {"label":_name_field.text}
            if(type=="Double" || type=="Int")
                value = _num_value_input.text ? _num_value_input.text : "1"
                args["min"] = _min_input.text ? _min_input.text : "0"
                args["max"] = _max_input.text ? _max_input.text : "10"
                if(type=="Double")
                    args["decimals"] = _decimals_input.text ? _decimals_input.text : "1"
            if(type=="Bool")
                value = _bool_value.checked ? "True" : "False"
            if(type=="String" || type=="StringList")
                value = _str_value_input.text ? _num_value_input.text : type=="String" ? "\"\"" : "[\"\"]"
                args["list"] = _string_list_input.text ? _string_list_input.text : "[\"\"]"
            var param_desc = { "name": name, "type": type, "doc": doc, "value": value, "args": args }
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
