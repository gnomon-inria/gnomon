import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

G.Dialog {
    id: _self

    property string message: ""
    property string caption: ""

    standardButtonsStyle: [
        [Dialog.Yes, G.Style.ButtonType.Base, true],
        [Dialog.No, G.Style.ButtonType.Base, true],
    ]

    parent: Overlay.overlay
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    width: G.Style.smallDialogWidth
    height: G.Style.largeDelegateHeight + (caption != "" ? G.Style.mediumLabelHeight : 0)
    header.height: 0

    modal: true

    Label {
        id: _message
        text: _self.message
        font: G.Style.fonts.cardLabel
    }

    Text {
        anchors.top: _message.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: G.Style.smallPadding

        text: _self.caption
        font: G.Style.fonts.value
        color: G.Style.colors.textColorBase
        wrapMode: Text.Wrap
    }

    standardButtons:  Dialog.Yes | Dialog.No
    
}
