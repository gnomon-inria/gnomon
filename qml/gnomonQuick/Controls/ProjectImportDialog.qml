import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import Qt.labs.platform as P

import gnomonQuick.Controls   as G
import gnomonQuick.Style      as G
import gnomon.Project    as GP


G.Dialog {
    id: _self

    modal: true

    property alias importPath : _copy_path.text
    property alias message: _message.text

    parent: Overlay.overlay
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: G.Style.smallDialogWidth
    height: G.Style.smallDialogHeight

    title: "Import file to the project?"

    Text {
        id: _message
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: G.Style.smallPadding
        anchors.bottomMargin: G.Style.smallPadding

        text: "You have to import this file to your project to edit it, otherwise, it will remain read-only. Please select where you want to copy it."
        font: G.Style.fonts.value
        color: G.Style.colors.textColorBase
        wrapMode: Text.Wrap
    }

    G.TextField {
        id: _copy_path

        property var isValid: _copy_path.text.includes(GP.ProjectManager.project.currentDir)

        backgroundColor: G.Style.colors.gutterColor
        backgroundHighlightColor: isValid? G.Style.colors.okColor : G.Style.colors.warningColor

        anchors.top: _message.bottom
        anchors.right: _check_path_icon.left
        anchors.left: parent.left
        anchors.margins: G.Style.smallPadding
    }

    G.Icon {
        id: _check_path_icon

        anchors.right: _edit_path_button.left
        anchors.verticalCenter: _copy_path.verticalCenter;
        anchors.rightMargin: G.Style.smallPadding
        color: _copy_path.isValid? G.Style.colors.okColor : G.Style.colors.warningColor
        size: G.Style.iconSmall;
        icon: _copy_path.isValid? "folder-check" : "folder-alert"
    }

    G.IconButton {
        id: _edit_path_button

        anchors.right: parent.right
        anchors.verticalCenter: _copy_path.verticalCenter;
        anchors.rightMargin: G.Style.smallPadding
        size: G.Style.iconSmall;
        iconName: "folder-open"

        onClicked: {
            _copy_path_dialog.currentFolder = "file://" + GP.ProjectManager.project.currentDir
            _copy_path_dialog.open();
        }
    }

    P.FolderDialog {
        id: _copy_path_dialog

        title: "Select folder"
        modality: Qt.WindowModal;

        onAccepted: {
            _self.importPath = decodeURIComponent(_copy_path_dialog.folder).slice(7)
        }
    }

    footer: DialogButtonBox
    {
        visible: true

        alignment: Qt.AlignRight
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
            flat: true
            enabled: _copy_path.isValid
            type: _copy_path.isValid ? G.Style.ButtonType.OK : G.Style.ButtonType.Warning
            width: G.Style.buttonWidth
            onClicked: _self.accept();
        }
    }
}