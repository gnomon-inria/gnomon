import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls 1.0 as G
import gnomonQuick.Style    1.0 as G


Control {
    id: _self

    required property var param;
    required property var component;
    property string paramName : ""

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

        onEditingFinished: {
            _self.paramName = text
        }

        background: Rectangle {
            color: G.Style.colors.gutterColor;
            radius: G.Style.panelRadius;
        }
    }

    Label {
        id: _type_label

        anchors.left: _self.left
        anchors.top:  _type.top
        anchors.margins: G.Style.smallPadding

        width: G.Style.shortButtonWidth

        text: "TYPE"
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
        horizontalAlignment: Text.AlignRight
    }

    Label {
        id: _type

        anchors.left: _type_label.right
        anchors.top: _name_edit.bottom
        anchors.margins: G.Style.smallPadding

        width: _self.width/2
        height: G.Style.smallLabelHeight

        property int param_index: _self.param ? findParamIndex(_parameter_types, _self.param): -1
        text : param_index !== -1 ? _parameter_types.get(param_index).name : ""
        font: G.Style.fonts.value
        color: G.Style.colors.textColorBase
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
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

    G.ParameterArgumentsEditor {
        id: _arguments_panel

        param: _self.param

        anchors.left: _args_label.right
        anchors.right: _self.right
        anchors.top:  _type.bottom
        anchors.margins: G.Style.smallPadding

        height: G.Style.mediumDelegateHeight
    }

    Label {
        id: _preview_label

        anchors.left: _self.left
        anchors.top:  _preview_panel.top
        anchors.margins: G.Style.smallPadding

        width: G.Style.shortButtonWidth

        text: "PREVIEW"
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
        horizontalAlignment: Text.AlignRight
    }

    G.Gutter {
        id: _preview_panel

        anchors.left: _preview_label.right
        anchors.right: _self.right
        anchors.top: _arguments_panel.bottom
        anchors.bottom: _self.bottom
        anchors.margins: G.Style.smallPadding

        Loader {
            id: _parameter_preview

            property var param: _self.param;
            property var component: _self.component;

            property var lparam: param
            sourceComponent: component

            anchors.centerIn: parent
            anchors.margins: G.Style.smallPadding
            height: implicitHeight ? implicitHeight : G.Style.controlHeight
            width: G.Style.mediumPanelWidth

            enabled: false
        }
    }

    ListModel {
        id: _parameter_types

        ListElement {
            type: "dtkCoreParameterNumeric<qlonglong>"
            subType: ""
            name: "Integer Number"
        }

        ListElement {
            type: "dtkCoreParameterNumeric<double>"
            subType: ""
            name: "Real Number"
        }

        ListElement {
            type: "dtkCoreParameterNumeric<bool>"
            subType: ""
            name: "Boolean"
        }

        ListElement {
            type: "dtkCoreParameterSimple<QString>"
            subType: ""
            name: "String"
        }

        ListElement {
            type: "dtkCoreParameterPath"
            subType: ""
            name: "Path"
        }

        ListElement {
            type: "dtkCoreParameterRange<qlonglong>"
            subType: ""
            name: "Integer Range"
        }

        ListElement {
            type: "dtkCoreParameterRange<double>"
            subType: ""
            name: "Real Range"
        }

        ListElement {
            type: "dtkCoreParameterInList<QString>"
            subType: ""
            name: "String in List"
        }

        ListElement {
            type: "dtkCoreParameterInListStringList"
            subType: ""
            name: "String List in List"
        }

        ListElement {
            type: "gnomonCoreParameterColorMap"
            subType: ""
            name: "Color Map"
        }

        ListElement {
            type: "gnomonCoreParameterLookupTable"
            subType: ""
            name: "Color Lookup Table"
        }

        ListElement {
            type: "gnomonCoreParameterColorTable"
            subType: ""
            name: "Material Table"
        }

        ListElement {
            type: "gnomonCoreParameterNurbs"
            subType: "Curve"
            name: "Nurbs Curve"
        }

        ListElement {
            type: "gnomonCoreParameterNurbs"
            subType: "Surface"
            name: "Nurbs Surface"
        }

        ListElement {
            type: "gnomonCoreParameterNurbs"
            subType: "Function"
            name: "Nurbs Function"
        }

        ListElement {
            type: "gnomonCoreParameterGraphical"
            subType: ""
            name: "Graphical"
        }
    }

    function findParamIndex(model, param) {
        let type = param.type
        type = type.replace(',void', '')
        const nurbstype = type === "gnomonCoreParameterNurbs" ? param.nurbsType : -1
        let subtype = ""
        if(nurbstype === 0) {
            subtype = "Curve"
        } else if(nurbstype === 1) {
            subtype = "Function"
        } else if(nurbstype === 2) {
            subtype = "Surface"
        }
        for(var i=0; i<model.count; i++) {
            if (model.get(i)["type"] === type && model.get(i)["subType"] === subtype) {
                return i
            }
        }
        return -1
    }
}
