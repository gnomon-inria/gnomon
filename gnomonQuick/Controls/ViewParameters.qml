import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import Qt.labs.settings

import xQuick.Controls      1.0 as X
import xQuick.Fonts         1.0 as X
import xQuick.Style         1.0 as X
import xQuick.Vis           1.0 as XVis

import gnomonQuick.Controls as G
import gnomonQuick.Style as G


Control {
    id: _control;

    required property Item view;

    implicitHeight: 4*G.Style.mediumLabelHeight

    ColumnLayout {
        anchors.fill: parent;

        Item {
            height: G.Style.mediumLabelHeight
            Layout.fillWidth: true;

            Label {
                id: _elevation_label

                anchors.left: parent.left;
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: G.Style.smallPadding

                text: "Elevation";
                font: G.Style.fonts.formLabel
                verticalAlignment: Text.AlignVCenter
            }

            TextInput {
                id: _elevation

                anchors.left: _elevation_label.right;
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: G.Style.smallPadding

                font: G.Style.fonts.value
                color: G.Style.colors.textColorBase

                text: view ? view.viewLogic.cameraElevation.toFixed(2) : "0";
                validator: DoubleValidator {
                    bottom: -90
                    top: 90
                    decimals: 2
                    notation: DoubleValidator.StandardNotation
                }

                onEditingFinished: {
                    if (view) {
                        let d = parseFloat(text)
                        view.viewLogic.cameraElevation = parseFloat(d != NaN ? d : 0);
                    }
                }
            }
        }

        Item {
            height: G.Style.mediumLabelHeight
            Layout.fillWidth: true;

            Label {
                id: _azimuth_label

                anchors.left: parent.left;
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: G.Style.smallPadding

                text: "Azimuth";
                font: G.Style.fonts.formLabel
                verticalAlignment: Text.AlignVCenter
            }

            TextInput {
                id: _azimuth

                anchors.left: _azimuth_label.right;
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: G.Style.smallPadding

                font: G.Style.fonts.value
                color: G.Style.colors.textColorBase

                text: view ? view.viewLogic.cameraAzimuth.toFixed(2) : "0";
                validator: DoubleValidator {
                    bottom: -180
                    top: 180
                    decimals: 2
                    notation: DoubleValidator.StandardNotation
                }

                onEditingFinished: {
                    if (view) {
                        let d = parseFloat(text)
                        view.viewLogic.cameraAzimuth = parseFloat(d != NaN ? d : 0);
                    }
                }
            }
        }

        Item {
            height: G.Style.mediumLabelHeight
            Layout.fillWidth: true;

            Label {
                id: _roll_label

                anchors.left: parent.left;
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: G.Style.smallPadding

                text: "Roll";
                font: G.Style.fonts.formLabel
                verticalAlignment: Text.AlignVCenter
            }

            TextInput {
                id: _roll

                anchors.left: _roll_label.right;
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: G.Style.smallPadding

                font: G.Style.fonts.value
                color: G.Style.colors.textColorBase

                text: view ? view.viewLogic.cameraRoll.toFixed(2) : "0";
                validator: DoubleValidator {
                    bottom: -180
                    top: 180
                    decimals: 2
                    notation: DoubleValidator.StandardNotation
                }

                onEditingFinished: {
                    if (view) {
                        let d = parseFloat(text)
                        view.viewLogic.cameraRoll = parseFloat(d != NaN ? d : 0);
                    }
                }
            }
        }

        Item {
            height: G.Style.mediumLabelHeight
            Layout.fillWidth: true;

            Label {
                id: _distance_label

                anchors.left: parent.left;
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: G.Style.smallPadding

                text: "Distance";
                font: G.Style.fonts.formLabel
                verticalAlignment: Text.AlignVCenter
            }

            TextInput {
                id: _distance

                anchors.left: _distance_label.right;
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: G.Style.smallPadding

                font: G.Style.fonts.value
                color: G.Style.colors.textColorBase

                text: view ? view.viewLogic.cameraDistance.toFixed(2) : "0";
                validator: DoubleValidator {
                    bottom: 0
                    decimals: 2
                    notation: DoubleValidator.StandardNotation
                }

                onEditingFinished: {
                    if (view) {
                        let d = parseFloat(text)
                        view.viewLogic.cameraDistance = parseFloat(d != NaN ? d : 0);
                    }
                }
            }
        }
    }
}