import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

import gnomon.MetaData 1.0 as GM


G.Dialog {
    id: _self

    parent: Overlay.overlay

    property bool isGroup: false

    property alias name: _name_edit.text
    property alias type_index: _type_combobox.currentIndex

    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)

    width: G.Style.smallDialogWidth
    height: G.Style.smallDialogHeight

    focus: true
    modal: true

    footer: DialogButtonBox {
        alignment: Qt.AlignRight
        spacing: G.Style.smallPadding

        background: Rectangle {
            anchors.fill: parent
            color: G.Style.colors.gutterColor
        }

        G.Button {
            text: 'Cancel';
            flat: true
            type: G.Style.ButtonType.Neutral
            width: G.Style.buttonWidth
            onClicked: _self.reject();
        }

        G.Button {
            text: 'OK';
            type: _name_edit.text != "" ? G.Style.ButtonType.OK : G.Style.ButtonType.Danger
            width: G.Style.buttonWidth
            enabled: _name_edit.text != ""
            flat: _name_edit.text == ""
            onClicked: _self.accept();
        }
    }

    title: _self.isGroup ? "New Parameter Group" : "New Parameter"

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

        backgroundHighlightColor: text != "" ? G.Style.colors.okColor : G.Style.colors.dangerColor

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
        visible: !_self.isGroup

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
        visible: !_self.isGroup

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
            type: "gnomonCoreParameterNurbs"
            name: "Nurbs Curve/Surface"
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
