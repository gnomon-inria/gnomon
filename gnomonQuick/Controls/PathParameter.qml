import QtQuick
import QtQuick.Controls

import Qt.labs.platform  1.0 as P

import gnomonQuick.Style as G
import gnomonQuick.Controls as G

Control {
    id: _control

    required property var param

    implicitHeight: _label.implicitHeight + _button.implicitHeight + 2*G.Style.smallPadding

    G.ToolTip {
        visible: _control.hovered && !_button.pressed
        text: _control.param ? _control.param.doc : ""
    }

    Label {
        id: _label

        anchors.left: _control.left
        anchors.top: _control.top

        text: _control.param? _control.param.label.toUpperCase() : ""
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
    }

    G.IconButton {
        id: _button;

        anchors.top: _label.bottom
        anchors.right: _control.right
        anchors.margins: G.Style.smallPadding

        iconName: "folder-open"
        enabled: _control.param

        size: G.Style.iconMedium;
        color: G.Style.colors.fgColor;
        tooltip: "Browse local files"

        onClicked: {
            _file_dialog.open();
        }
    }

    Label {
        id: _value

        anchors.right: _button.left
        anchors.verticalCenter: _button.verticalCenter
        anchors.margins: G.Style.smallPadding

        text: _control.param ? _control.param.path : ""
        color: G.Style.colors.hoveredBaseColor
        font: G.Style.fonts.value
    }

    P.FileDialog {
        id: _file_dialog

        currentFile: _control.param ? _control.param.baseName : "";
        folder: _control.param ? _control.param.dirName : "";
        nameFilters: _control.param ? _control.param.filters : [];

        modality: Qt.NonModal;

        onAccepted: {
            _control.param.path = decodeURIComponent(_file_dialog.file);
        }
    }
}
