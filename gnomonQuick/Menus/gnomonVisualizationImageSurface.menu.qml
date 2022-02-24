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

    Rectangle {

        id: _color_map_options;
        color: X.Style.backgroundColor;
        radius: 3;

        anchors.top: parent.top;
        anchors.topMargin: 10;
        width: _self.width;
        height: 270;

        X.Label {

            id: _color_map_label;

            anchors.top: parent.top;
            anchors.left: parent.left;

            text: "Color map"
        }

        Column {

            anchors.top: _color_map_label.bottom;
            anchors.bottom: parent.bottom;
            spacing: 10;

            G.Colormap {

                height: 70;
                width: 330;

                param: parameters["colormap"]
            }

            C.Range {

                height: 70;
                width: 330;

                param: parameters["intensity_range"]
                decimals: 2
            }

            C.Numeric {

                height: 70;
                width: 330;

                param: parameters["threshold"]
                paramType: parameters["threshold"].type
            }
        }

    }

    Rectangle {

        id: _other_options;
        color: X.Style.backgroundColor;
        radius: 3;

        anchors.top: _color_map_options.bottom;
        anchors.topMargin: 10;
        width: _self.width;
        height: 270;

        X.Label {

            id: _other_label;

            anchors.top: parent.top;
            anchors.left: parent.left;

            text: "Other options"
        }

        Column {

            anchors.top: _other_label.bottom;
            anchors.bottom: parent.bottom;
            spacing: 10;

            C.Numeric {

                height: 70;
                width: _self.width;

                param: parameters["cell_radius"]
                paramType: parameters["cell_radius"].type

            }


            C.Numeric {

                height: 70;
                width: _self.width;

                param: parameters["gaussian_sigma"]
                paramType: parameters["gaussian_sigma"].type
            }

            C.Numeric {

                height: 70;
                width: _self.width;

                param: parameters["opacity"]
                paramType: parameters["opacity"].type
            }
        }
    }
}
