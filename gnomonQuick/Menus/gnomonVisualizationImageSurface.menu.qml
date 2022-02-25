import QtQuick              2.15
import QtQuick.Controls     2.15
import QtGraphicalEffects   1.15
import QtQuick.Layouts      1.15

import xQuick.Controls      1.0 as X
import xQuick.Fonts         1.0 as X
import xQuick.Style         1.0 as X
import xQuick.Vis           1.0 as XVis

import crossQuick        1.0 as C
import crossParameters   1.0 as C

import gnomonQuick.Controls 1.0 as G

Item {

    id: _self;

    property var parameters;

    signal valueChanged()

    C.InListStringList {
        id: _channel_options;

        anchors.top: parent.top;
        anchors.topMargin: 10;

        width: 330;
        height: 70;

        visible: parameters["channel"]

        param: parameters["channel"]

        Connections {
            target: parameters["channel"]
            function onValueChanged() {
                _self.valueChanged();
            }
        }

    }

    Rectangle {

        id: _surface_generation_options;
        color: X.Style.backgroundColor;
        radius: 3;

        anchors.top: parameters["channel"] ? _channel_options.top : parent.top;
        anchors.topMargin: 10;
        width: _self.width;
        height: 170;

        X.Label {

            id: _surface_generation_label;

            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.topMargin: 5;
            anchors.leftMargin: 5;

            text: "Surface generation"
        }

        Column {

            anchors.top: _surface_generation_label.bottom;
            anchors.bottom: parent.bottom;
            spacing: 10;

            C.Numeric {

                height: 70;
                width: 330;

                param: parameters["threshold"]
                paramType: parameters["threshold"].type

                Connections {
                    target: parameters["threshold"]
                    function onValueChanged() {
                        _self.valueChanged();
                    }
                }

            }

            C.Numeric {

                height: 70;
                width: 330;

                param: parameters["gaussian_sigma"]
                paramType: parameters["gaussian_sigma"].type

                Connections {
                    target: parameters["gaussian_sigma"]
                    function onValueChanged() {
                        _self.valueChanged();
                    }
                }

            }

        }

    }

    Rectangle {

        id: _signal_protection_options;
        color: X.Style.backgroundColor;
        radius: 3;

        anchors.top : _surface_generation_options.bottom;
        anchors.topMargin: 10;
        width: _self.width;
        height: 90;

        X.Label {

            id: _signal_protection_label;

            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.topMargin: 5;
            anchors.leftMargin: 5;

            text: "Signal protection"
        }

        Column {

            anchors.top: _signal_protection_label.bottom;
            anchors.bottom: parent.bottom;
            spacing: 10;

            C.Numeric {

                height: 70;
                width: 330;

                param: parameters["cell_radius"]
                paramType: parameters["cell_radius"].type

                Connections {
                    target: parameters["cell_radius"]
                    function onValueChanged() {
                        _self.valueChanged();
                    }
                }

            }

        }
    }

    Rectangle {

        id: _rendering_options;
        color: X.Style.backgroundColor;
        radius: 3;

        anchors.top: _signal_protection_options.bottom;
        anchors.topMargin: 10;
        width: _self.width;
        height: 270;

        X.Label {

            id: _rendering_label;

            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.topMargin: 5;
            anchors.leftMargin: 5;

            text: "Rendering"
        }

        Column {

            anchors.top: _rendering_label.bottom;
            anchors.bottom: parent.bottom;
            spacing: 10;

            G.Colormap {

                height: 70;
                width: 330;

                param: parameters["colormap"]

                Connections {
                    target: parameters["colormap"]
                    function onValueChanged() {
                        _self.valueChanged();
                    }
                }

            }

            C.Range {

                height: 70;
                width: 330;

                param: parameters["intensity_range"]
                decimals: 2

                Connections {
                    target: parameters["intensity_range"]
                    function onValueChanged() {
                        _self.valueChanged();
                    }
                }

            }

            C.Numeric {

                height: 70;
                width: 330;

                param: parameters["opacity"]
                paramType: parameters["opacity"].type

                Connections {
                    target: parameters["opacity"]
                    function onValueChanged() {
                        _self.valueChanged();
                    }
                }

            }
        }
    }
}
