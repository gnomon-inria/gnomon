import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

import gnomon.MetaData 1.0 as GM


G.Dialog {
    id: _self

    parent: Overlay.overlay

    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)

    width: G.Style.smallDialogWidth
    height: G.Style.smallDialogHeight

    focus: true
    modal: true

    standardButtons:  Dialog.Ok | Dialog.Cancel

    title: "New Parameter"

    Label {
        id: _name_label

        anchors.left: parent.left
        anchors.top:  _name_edit.top
        anchors.margins: G.Style.smallPadding
        anchors.topMargin: 0

        width: G.Style.shortButtonWidth

        text: "NAME"
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
        horizontalAlignment: Text.AlignRight
    }

    G.TextField {
        id: _name_edit

        anchors.left: _name_label.right
        anchors.top: parent.top
        anchors.margins: G.Style.smallPadding

        width: parent.width/2
        height: G.Style.mediumLabelHeight

        text: ""
        color: G.Style.colors.hoveredBaseColor
        font: G.Style.fonts.header

        background: Rectangle {
            color: G.Style.colors.gutterColor;
            radius: G.Style.panelRadius;
        }
    }

    Label {
        id: _type_label

        anchors.left: parent.left
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
        anchors.top: _name_edit.bottom
        anchors.margins: G.Style.smallPadding

        width: parent.width/2
        height: G.Style.mediumLabelHeight

        model: _parameter_types
        textRole: "name"

        currentIndex: 0
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
