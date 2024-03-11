import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

import gnomon.MetaData 1.0 as GM


G.Dialog {
    id: _self

    property string pluginName: ""
    property var pluginMetaData
    property var pluginParameters

    parent: Overlay.overlay

    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)

    width: G.Style.mediumDialogWidth
    height: G.Style.largeDialogHeight

    focus: true
    modal: true

    header.height: G.Style.largeLabelHeight
    footer.visible: false

    title: "Plugin Info : " + _self.pluginName

    G.Icon {
        parent: header

        anchors.top: parent.top;
        anchors.right: parent.right;
        anchors.margins: G.Style.mediumPadding

        icon: "information-outline";
        size: G.Style.largeLabelHeight - 2*G.Style.mediumPadding;
        color: G.Style.colors.neutralColor
    }

    Label {
        anchors.verticalCenter: version_label.verticalCenter
        anchors.left: parent.left
        anchors.right: version_label.left
        anchors.margins: G.Style.smallPadding

        text: "`" + (_self.pluginMetaData? _self.pluginMetaData["name"] : " ") + "` Plugin"
        textFormat: TextEdit.MarkdownText

        font: G.Style.fonts.formLabel
        color: G.Style.colors.textColorBase
        wrapMode: Text.Wrap
    }

    Label {
        id: version_label

        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: G.Style.smallPadding
        width: G.Style.buttonWidth

        text: "Version " + (_self.pluginMetaData? _self.pluginMetaData["version"] : "?.??")

        font: G.Style.fonts.value
        color: G.Style.colors.textColorBase
        horizontalAlignment: Text.AlignRight
    }

    Label {
        id: documentation_label

        anchors.top: version_label.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: G.Style.smallPadding

        text: _self.pluginMetaData? _self.pluginMetaData["documentation"] : ""

        font: G.Style.fonts.value
        color: G.Style.colors.textColorBase
        wrapMode: Text.Wrap
    }

    Label {
        id: parameter_label

        anchors.top: documentation_label.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: G.Style.smallPadding
        anchors.topMargin: -G.Style.smallPadding

        height: G.Style.smallLabelHeight

        text: "Parameters"

        font: G.Style.fonts.subHeader
        color: G.Style.colors.textColorBase
    }

    ListView {
        id: parameter_list

        anchors.top: parameter_label.bottom
        anchors.bottom: source_label.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: G.Style.smallPadding
        anchors.bottomMargin: 2*G.Style.smallPadding

        clip: true;

        model: params_model

        delegate: G.ListItemDelegate {
            id: _parameter_delegate

            width: parameter_list.width
            height: G.Style.mediumLabelHeight

            Label {
                id: _label

                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: G.Style.smallPadding

                text: label
                font: G.Style.fonts.subHeader
                color: G.Style.colors.textColorBase
            }

            Label {
                anchors.left: _label.right
                anchors.right: parent.right
                anchors.verticalCenter: _label.verticalCenter
                anchors.margins: G.Style.smallPadding

                text: doc
                font: G.Style.fonts.label
                color: G.Style.colors.textColorBase
                wrapMode: Text.Wrap
            }
        }


        ScrollBar.vertical: ScrollBar { visible: _console_list.contentHeight > _console_list.height; }
    }

    Label {
        id: source_label

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: G.Style.smallPadding
        anchors.bottomMargin: -G.Style.smallPadding

        text: ""
        textFormat: TextEdit.MarkdownText

        font: G.Style.fonts.value
        color: G.Style.colors.textColorBase
        wrapMode: Text.Wrap
    }

    onPluginMetaDataChanged: {
        if (_self.pluginMetaData) {
            let md = GM.MetaData.pluginMetaData(_self.pluginMetaData["group"], _self.pluginMetaData["name"])
            source_label.text = "Source package: `" + md["package"] + "` (Conda channel: `" + md["conda_channel"] + "`)"
        }
    }

    ListModel {
        id: params_model;
        dynamicRoles: true;
    }

    onPluginParametersChanged: {
        params_model.clear();
        for (var param_name in _self.pluginParameters) {
            var prop_dict = {};
            prop_dict["name"] = param_name;
            prop_dict["doc"] = _self.pluginParameters[param_name].doc;
            prop_dict["label"] = _self.pluginParameters[param_name].label;
            params_model.append(prop_dict)
        }
    }
}
