import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import Qt.labs.platform  1.0 as P

import gnomonQuick.Style as G
import gnomonQuick.Controls as G
import gnomon.Project         1.0 as GP

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
            _path_tree_view.requestFileSelection(_control.param?.path)
            _path_dialog.open();
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

    G.Dialog {
        id: _path_dialog

        parent: Overlay.overlay

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.mediumDialogWidth
        height: G.Style.mediumDialogHeight

        modal: true
        title: "Select path"
        padding: 0

        standardButtons:  Dialog.Ok | Dialog.Cancel

        ScrollView {
            id: _scroll_view

            anchors.fill: parent

            clip: true
            contentWidth: availableWidth
            contentHeight: _path_tree_view.implicitHeight

            ScrollBar.vertical.policy: ScrollBar.AlwaysOn

            G.TreeView {
                id: _path_tree_view

                anchors.fill: parent

                interactive: true

                data_paths: [decodeURIComponent(P.StandardPaths.writableLocation(P.StandardPaths.HomeLocation)).slice(7)]
                expanded: true
                selectFolder: true
                singleSelection: false
                dragEnabled: false
            }
        }

        onAccepted: {
            let paths = "";
            for (let i=0; i<_path_tree_view.selectedPaths.length; i++) {
                if (i>0) {
                    paths += ","
                }
                paths += _path_tree_view.selectedPaths[i]
            }
            _control.param.path = paths;
        }
    }
}
