import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

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

    QtObject {
        id: _internal;
        property var expanded: [
            "surface",
            "signal",
            "rendering"
        ]
    }

    signal valueChanged()

    function isSectionExpanded(name) {
        return _internal.expanded.includes(name);
    }

    function toggleCollapse(group) {
        if(_internal.expanded.includes(group)) _self.collapseSection(group)
        else _self.expandSection(group)
    }

    function collapseSection(group) {
        _internal.expanded = _internal.expanded.filter((item) => item !== group)
    }

    function expandSection(group) {
        _internal.expanded = _internal.expanded.concat([group])
    }

    C.InListStringList {
        id: _channel_options;

        anchors.top: parent.top;
        anchors.topMargin: 10;

        width: 330;
        height: 70;

        visible: parameters["channel"] !== undefined

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
        height: _self.isSectionExpanded("surface") ? 170 : 10 + _surface_generation_header.height;

        clip: true;

        Behavior on height {
            NumberAnimation { duration: 200 }
        }

        Item {

            id: _surface_generation_header;

            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.right: parent.right;

            height: childrenRect.height;

            X.Label {

                id: _surface_generation_label;

                anchors.top: parent.top;
                anchors.left: parent.left;
                anchors.topMargin: 5;
                anchors.leftMargin: 5;

                text: "Surface generation"
                font.pixelSize: 20;
            }

            X.Icon {
                id: _icon_surface;

                anchors.right: parent.right;
                anchors.top: parent.top;
                anchors.topMargin: 5;
                anchors.rightMargin: 5;
                size: 30;

                icon: X.Icons.icons.keyboard_arrow_up;

                rotation: _self.isSectionExpanded("surface") ? 0 : 180;

                Behavior on rotation {
                    NumberAnimation { duration: 200 }
                }

            }

            MouseArea {
                anchors.fill: parent;
                onClicked: _self.toggleCollapse("surface")
            }

        }


        Column {

            anchors.top: _surface_generation_header.bottom;
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
        height: _self.isSectionExpanded("signal") ? 90 : 10 + _signal_protection_header.height;

        clip: true;

        Behavior on height {
            NumberAnimation { duration: 200 }
        }

        Item {

            id: _signal_protection_header;

            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.right: parent.right;

            height: childrenRect.height;

            X.Label {

                id: _signal_protection_label;

                anchors.top: parent.top;
                anchors.left: parent.left;
                anchors.topMargin: 5;
                anchors.leftMargin: 5;

                text: "Signal protection"
                font.pixelSize: 20;
            }

            X.Icon {
                id: _icon_signal;

                anchors.right: parent.right;
                anchors.top: parent.top;
                anchors.topMargin: 5;
                anchors.rightMargin: 5;
                size: 30;

                icon: X.Icons.icons.keyboard_arrow_up;

                rotation: _self.isSectionExpanded("signal") ? 0 : 180;

                Behavior on rotation {
                    NumberAnimation { duration: 200 }
                }

            }

            MouseArea {
                anchors.fill: parent;
                onClicked: _self.toggleCollapse("signal")
            }
        }

        Column {

            anchors.top: _signal_protection_header.bottom;
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
        height: _self.isSectionExpanded("rendering") ? 270 : 10 + _rendering_options_header.height;

        clip: true;

        Behavior on height {
            NumberAnimation { duration: 200 }
        }

        Item {

            id: _rendering_options_header;

            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.right: parent.right;

            height: childrenRect.height;

            X.Label {

                id: _rendering_label;

                anchors.top: parent.top;
                anchors.left: parent.left;
                anchors.topMargin: 5;
                anchors.leftMargin: 5;

                text: "Rendering"
                font.pixelSize: 20;
            }

            X.Icon {
                id: _icon_rendering;

                anchors.right: parent.right;
                anchors.top: parent.top;
                anchors.topMargin: 5;
                anchors.rightMargin: 5;
                size: 30;

                icon: X.Icons.icons.keyboard_arrow_up;

                rotation: _self.isSectionExpanded("rendering") ? 0 : 180;

                Behavior on rotation {
                    NumberAnimation { duration: 200 }
                }

            }

            MouseArea {
                anchors.fill: parent;
                onClicked: _self.toggleCollapse("rendering")
            }

        }

        Column {

            anchors.top: _rendering_options_header.bottom;
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
