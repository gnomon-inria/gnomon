import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls 1.0 as G
import gnomonQuick.Style    1.0 as G


Control {
    id: _self

    required property var param;


    Label {
        id: _name_label

        anchors.left: _self.left
        anchors.top:  _name_edit.top
        anchors.margins: G.Style.smallPadding

        width: G.Style.shortButtonWidth

        text: "NAME"
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
        horizontalAlignment: Text.AlignRight
    }

    G.TextField {
        id: _name_edit

        anchors.left: _name_label.right
        anchors.top: _self.top
        anchors.margins: G.Style.smallPadding

        width: _self.width/2
        height: G.Style.mediumLabelHeight

        text: _self.param ? _self.param.label : ""
        color: G.Style.colors.hoveredBaseColor
        font: G.Style.fonts.header

        background: Rectangle {
            color: G.Style.colors.gutterColor;
            radius: G.Style.panelRadius;
        }
    }

    Label {
        id: _doc_label

        anchors.left: _self.left
        anchors.top:  _doc_edit.top
        anchors.margins: G.Style.smallPadding

        width: G.Style.shortButtonWidth

        text: "DOCUMENTATION"
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
        horizontalAlignment: Text.AlignRight
    }

    G.TextArea {
        id: _doc_edit

        anchors.left: _doc_label.right
        anchors.right: _self.right
        anchors.top: _name_edit.bottom
        anchors.margins: G.Style.smallPadding

        height: G.Style.largeLabelHeight

        text: _self.param ? _self.param.doc : ""
        color: G.Style.colors.hoveredBaseColor
        font: G.Style.fonts.value
    }

    Label {
        id: _type_label

        anchors.left: _self.left
        anchors.top:  _type_combobox.top
        anchors.margins: G.Style.smallPadding

        width: G.Style.shortButtonWidth

        text: "TYPE"
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
        horizontalAlignment: Text.AlignRight
    }

    G.ComboBox {
        id: _type_combobox

        anchors.left: _type_label.right
        anchors.top: _doc_edit.bottom
        anchors.margins: G.Style.smallPadding

        width: _self.width/2
        height: G.Style.mediumLabelHeight

        model: _parameter_types
        textRole: "name"

        currentIndex: _self.param ? findIndex(model, "type", _self.param.type) : -1
    }

    Label {
        id: _args_label

        anchors.left: _self.left
        anchors.top:  _arguments_panel.top
        anchors.margins: G.Style.smallPadding

        width: G.Style.shortButtonWidth

        text: "ARGUMENTS"
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
        horizontalAlignment: Text.AlignRight
    }

    G.Gutter {
        id: _arguments_panel

        anchors.left: _args_label.right
        anchors.right: _self.right
        anchors.top:  _type_combobox.bottom
        anchors.bottom: _preview_panel.top
        anchors.margins: G.Style.smallPadding
    }

    G.Gutter {
        id: _preview_panel

        anchors.left: _self.left
        anchors.right: _self.right
        anchors.bottom: _self.bottom
        anchors.margins: G.Style.smallPadding

        height: G.Style.largeLabelHeight
    }

    ListModel {
        id: _parameter_types

        ListElement {
            type: "dtkCoreParameterNumeric<qlonglong>"
            name: "Integer Number"
        }

        ListElement {
            type: "dtkCoreParameterNumeric<double>"
            name: "Real Number"
        }

        ListElement {
            type: "dtkCoreParameterNumeric<bool>"
            name: "Boolean"
        }

        ListElement {
            type: "dtkCoreParameterSimple<QString>"
            name: "String"
        }

        ListElement {
            type: "dtkCoreParameterPath"
            name: "Path"
        }

        ListElement {
            type: "dtkCoreParameterRange<qlonglong>"
            name: "Integer Range"
        }

        ListElement {
            type: "dtkCoreParameterRange<double>"
            name: "Real Range"
        }

        ListElement {
            type: "dtkCoreParameterInList<QString>"
            name: "String in List"
        }

        ListElement {
            type: "dtkCoreParameterInListStringList"
            name: "String List in List"
        }

        ListElement {
            type: "gnomonCoreParameterColorMap"
            name: "Color Map"
        }

        ListElement {
            type: "gnomonCoreParameterLookupTable"
            name: "Color Lookup Table"
        }

        ListElement {
            type: "gnomonCoreParameterColorTable"
            name: "Material Table"
        }

        ListElement {
            type: "gnomonCoreParameterNurbs"
            name: "Nurbs Curve/Surface"
        }

        ListElement {
            type: "gnomonCoreParameterGraphical"
            name: "Graphical"
        }
    }

    function findIndex(model, field, value) {
        for(var i=0; i<model.count; i++) {
            if (model.get(i)[field] === value) {
                return i
            }
        }
        return -1
    }
}
