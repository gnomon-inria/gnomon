import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import Qt.labs.settings

import xQuick.Controls      1.0 as X
import xQuick.Fonts         1.0 as X
import xQuick.Style         1.0 as X
import xQuick.Vis           1.0 as XVis

import gnomon.Visualization 1.0 as GV

import gnomonQuick.Controls as G
import gnomonQuick.Style as G


Control {
    id: _control;

    required property Item view;

    implicitHeight: _camera_panel.implicitHeight

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

        anchors.fill: parent;

        title: "Camera"
        collapsed: false
        panelHeight: _camera_parameters.count * (G.Style.mediumLabelHeight + G.Style.mediumColumnSpacing) + G.Style.mediumColumnSpacing

        visible: view.viewLogic.mode == GV.View.VIEW_MODE_3D

        ListView {
            id: _camera_list

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: G.Style.smallColumnSpacing

            width: _control.width - G.Style.sizes.s4

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
    }
}