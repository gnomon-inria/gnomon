import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Dialog {

    id: _self

    padding: G.Style.dialogPadding
    topPadding: G.Style.dialogPadding
    enter: Transition
    {
        NumberAnimation { property: "scale";from: 0.9;to: 1.0;easing.type: Easing.OutQuint;duration: 220 }
        NumberAnimation { property: "opacity";from: 0.0;to: 1.0;easing.type: Easing.OutCubic;duration: 150 }
    }

    exit: Transition
    {
        NumberAnimation { property: "scale";from: 1.0;to: 0.9;easing.type: Easing.OutQuint;duration: 220 }
        NumberAnimation { property: "opacity";from: 1.0;to: 0.0;easing.type: Easing.OutCubic;duration: 150 }
    }

    property var standardButtonsStyle: [
        [Dialog.Ok, G.Style.ButtonType.Base, false],
        [Dialog.Open, G.Style.ButtonType.Base, false],
        [Dialog.Save, G.Style.ButtonType.Base, false],
        [Dialog.Cancel, G.Style.ButtonType.Neutral, true],
        [Dialog.Close, G.Style.ButtonType.Neutral, true],
        [Dialog.Discard, G.Style.ButtonType.Neutral, true],
        [Dialog.Apply, G.Style.ButtonType.OK, false],
        [Dialog.Reset, G.Style.ButtonType.Neutral, true],
        [Dialog.RestoreDefaults, G.Style.ButtonType.Neutral, true],
        [Dialog.Help, G.Style.ButtonType.Base, true],
        [Dialog.SaveAll, G.Style.ButtonType.OK, false],
        [Dialog.Yes, G.Style.ButtonType.OK, false],
        [Dialog.YesToAll, G.Style.ButtonType.OK, false],
        [Dialog.No, G.Style.ButtonType.Danger, true],
        [Dialog.NoToAll, G.Style.ButtonType.Danger, false],
        [Dialog.Abort, G.Style.ButtonType.Danger, false],
        [Dialog.Retry, G.Style.ButtonType.Base, false],
        [Dialog.Ignore, G.Style.ButtonType.Neutral, false],
        [Dialog.NoButton, G.Style.ButtonType.OK, false],
    ]

    background: Rectangle {
        anchors.fill: parent
        radius: G.Style.panelRadius
        color: G.Style.colors.bgColor
    }

    Overlay.modal: Rectangle
    {
        color: G.Style.colors.overlayColor
        Behavior on opacity { NumberAnimation { duration: 150 } }
    }

    header: ToolBar {
        height: footer.height
        padding: G.Style.mediumPadding

        Label {
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Qt.AlignVCenter
            text: _self.title
            font: G.Style.fonts.header
            color: G.Style.colors.neutralColor
        }

        background: Rectangle {
            anchors.fill: parent
            color: G.Style.colors.gutterColor
        }
    }


    footer: DialogButtonBox {
        id: _dbox
        alignment: Qt.AlignRight
        spacing: G.Style.smallPadding

        background: Rectangle {
            anchors.fill: parent
            color: G.Style.colors.gutterColor
        }

        delegate: G.Button {
            id: _bdel
            width: G.Style.buttonWidth

        }
        Component.onCompleted: {
            for (const [buttonCode, style, flat] of standardButtonsStyle) {
                var button = _dbox.standardButton(buttonCode)
                if(button) {
                    button.type = Qt.binding(function() {return style})
                    button.flat = Qt.binding(function() {return flat})
                }
            }
        }
    }
}
