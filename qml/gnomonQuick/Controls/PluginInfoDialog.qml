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

    parent: Overlay.overlay

    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)

    width: G.Style.mediumDialogWidth
    height: G.Style.mediumDialogHeight

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
        id: source_label

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: G.Style.smallPadding

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
}
