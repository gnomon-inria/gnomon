import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import Qt.labs.platform  1.0 as P
import Qt.labs.settings

import gnomon.Visualization 1.0 as GV

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G


Control {
    id: _control;

    required property Item view;
    property bool collapsed: true;

    implicitWidth: G.Style.smallPanelWidth;
    height: _header.height + 3*G.Style.smallPadding + _camera_panel.height
    clip: true;

    background: Rectangle {
        color: G.Style.colors.bgColor
        radius: G.Style.panelRadius

        Rectangle {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter

             width: parent.width + 2*G.Style.smallPadding
            height: 1
            radius: G.Style.panelRadius;

            color: G.Style.colors.transparent;
            //visible: !_control.collapsed

            border.width: 1;
            border.color: G.Style.colors.gutterColor;
        }
    }

    Behavior on height {
        NumberAnimation {
            duration: 100
            easing.type: Easing.Linear;
        }
    }

    Control {
        id: _header;

        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.topMargin: G.Style.smallPadding
        anchors.bottomMargin: G.Style.smallPadding
        height: _control.collapsed ? 0 : _label.implicitHeight

        Label {
            id: _label

            visible: !_control.collapsed

            text: "View Parameters"
            font: G.Style.fonts.header
            color: G.Style.colors.textColorBase
        }
    }

    ListModel {
        id: _camera_parameters

        ListElement {
            name: "Elevation"
            from: -90
            to: 90
            target: "view.viewLogic.cameraElevation"
        }

        ListElement {
            name: "Azimuth"
            from: -180
            to: 180
            target: "view.viewLogic.cameraAzimuth"
        }

        ListElement {
            name: "Roll"
            from: -180
            to: 180
            target: "view.viewLogic.cameraRoll"
        }

        ListElement {
            name: "Distance"
            from: 0
            to: 1000000
            target: "view.viewLogic.cameraDistance"
        }
    }

    G.CollapsiblePanel {
        id: _camera_panel

        //anchors.fill: parent;
        anchors.top: _header.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.margins: G.Style.smallPadding

        title: "Camera"
        collapsed: false

        panelHeight:  _camera_parameters.count * (G.Style.mediumLabelHeight + G.Style.mediumColumnSpacing) + G.Style.mediumColumnSpacing + G.Style.iconSmall
        implicitHeight: _control.collapsed ? 0 : panelHeight + G.Style.collapsibleMinHeight + 3*G.Style.smallPadding

        visible: view.viewLogic.mode == GV.View.VIEW_MODE_3D

        ListView {
            id: _camera_list

            anchors.top: _button_container.bottom
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: -G.Style.smallPadding

            width: parent.width - G.Style.sizes.s4

            spacing: G.Style.mediumColumnSpacing

            model: _camera_parameters

            delegate: Item {
                height: G.Style.mediumLabelHeight
                width: _camera_list.width;

                Label {
                    id: _label

                    anchors.left: parent.left
                    anchors.top: parent.top

                    text: model.name.toUpperCase()
                    font: G.Style.fonts.label
                    color: G.Style.colors.textColorBase
                }

                TextField {
                    id: _text

                    anchors.top: _label.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right

                    font: G.Style.fonts.value
                    color: G.Style.colors.textColorBase
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    text: eval("view ? "+model.target+".toFixed(2) : '0'");
                    validator: DoubleValidator {
                        bottom: model.from
                        top: model.to
                        decimals: 2
                        notation: DoubleValidator.StandardNotation
                    }

                    background: Rectangle {
                        anchors.fill: parent
                        color: G.Style.colors.gutterColor
                        radius: G.Style.panelRadius
                    }

                    onEditingFinished: {
                        if (view) {
                           let d = parseFloat(text)
                           eval(model.target+" = parseFloat(d != NaN ? d : 0)");
                        }
                    }
                }
            }
        }

        Item {
            id: _button_container

            anchors.right: parent.right;
            anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.topMargin: G.Style.smallPadding;
            height: G.Style.iconMedium

            G.IconButton {
                id: _load_camera

                anchors.right: _button_container.right;
                anchors.verticalCenter: _button_container.verticalCenter
                anchors.rightMargin: G.Style.smallPadding

                size: G.Style.iconSmall;
                iconName: G.Icons.icons["file-upload-outline"]
                color: G.Style.colors.textColorBase;

                onClicked: {
                    _file_dialog_load.open()
                }
            }

            G.IconButton {
                id: _save_camera

                anchors.right: _load_camera.left;
                anchors.verticalCenter: _button_container.verticalCenter

                size: G.Style.iconSmall;
                iconName: G.Icons.icons["content-save-outline"]
                color: G.Style.colors.textColorBase;

                onClicked: {
                    _file_dialog_save.open()
                }
            }
        }
    }

    P.FileDialog {
        id: _file_dialog_load;

        title: "Load camera parameters"

        fileMode: P.FileDialog.OpenFile;
        modality: Qt.NonModal;
        nameFilters: ["Json camera files (*.json)"]

        onAccepted: {
            view.viewLogic.loadCamera(decodeURIComponent(_file_dialog_load.file));
        }
    }

    P.FileDialog {
        id: _file_dialog_save

        title: "Save camera parameters"

        fileMode: P.FileDialog.SaveFile
        modality: Qt.WindowModal;
        nameFilters: ["Json camera files (*.json)"]

        onAccepted: {
            view.viewLogic.saveCamera(decodeURIComponent(_file_dialog_save.file));
        }
    }
}
