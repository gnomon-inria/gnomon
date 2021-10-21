import QtQuick            2.15
import QtQuick.Controls   2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts    1.15

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

import gnomon.Visualization 1.0 as GV

Rectangle {

    id: _control;

    color: Qt.darker(X.Style.alternateBaseColor);

    property alias view: _view;

    signal droppedFromFile(string path)
    signal droppedFromManager(int index)

    signal transmit();
    signal switchTo2D();
    signal switchTo3D();

    XVis.Viewer {

        id: _view;

        anchors.fill: parent;

        mouseEnabled: true;
        /* XVis.Actor { */
        /*     XVis.PolyDataMapper { */
        /*         XVis.SphereSource { */
        /*             id: source; */
        /*         } */
        /*     } */
        /* } */
    }

    DropArea {

        id: _drop;

        anchors.fill: parent;

        X.Icon {
            icon: X.Icons.icons.arrow_circle_down;
            size: 56;
            color: "#44999999";
            visible: _drop.containsDrag;
            anchors.centerIn: parent;
        }

        onDropped: {
            if (drop.hasUrls) {
                drop.urls.forEach(function (item, index) {
                    _control.droppedFromFile(item);
                });
            } else {
                _control.droppedFromManager(drag.source.ref);
            }

            drop.accept();
        }
    }

    X.Icon { id: _2d_icon;
        icon: X.Icons.icons.crop_square;
        size: 32;
        color: X.Style.foregroundColor;

        anchors.top: _view.top
        anchors.topMargin: 10
        anchors.left: _view.left
        anchors.leftMargin: 10

        MouseArea { id: _2d_mouse_area;
            anchors.fill: parent;
            hoverEnabled: true;

            onClicked: {
                _control.switchTo2D();
            }
        }

        ToolTip.visible: _2d_mouse_area.containsMouse;
        ToolTip.text: "2D mode";
    }

    X.Icon { id: _3d_icon;
        icon: X.Icons.icons._3d_rotation;
        size: 32;
        color: X.Style.foregroundColor;

        anchors.top: _view.top
        anchors.topMargin: 10
        anchors.left: _view.left
        anchors.leftMargin: 50

        MouseArea { id: _3d_mouse_area;
            anchors.fill: parent;
            hoverEnabled: true;

            onClicked: {
                _control.switchTo3D();
            }
        }

        ToolTip.visible: _3d_mouse_area.containsMouse;
        ToolTip.text: "3D mode";
    }

    X.Icon { id: _export_icon;
        icon: X.Icons.icons.arrow_circle_up;
        size: 32;
        color: X.Style.foregroundColor;

        anchors.top: _view.top
        anchors.topMargin: 10
        anchors.right: _view.right
        anchors.rightMargin: 10

        MouseArea { id: _export_mouse_area;
            anchors.fill: parent;
            hoverEnabled: true;

            onClicked: {
                _control.transmit();
            }
        }

        ToolTip.visible: _export_mouse_area.containsMouse;
        ToolTip.text: "Export";
    }

    layer.enabled: true
    layer.effect: OpacityMask
    {
        maskSource: Rectangle
        {
            width: _control.width
            height: _control.height
            radius: 4;
        }
    }
}
