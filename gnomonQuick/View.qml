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
    signal sliceChange(int value);

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

    ToolBar { id: _menubar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        //situation: X.Style.Position.Left;

        width: 42

        Behavior on width {
            NumberAnimation {
                easing {
                    type: Easing.OutElastic
                    amplitude: 1.0
                    period: 0.5
                }
            }
        }

        X.LabelHint2 {
            text: "View";
            horizontalAlignment: Text.AlignHCenter;
            Layout.fillHeight: true;

            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    if (_menubar.width <50) {
                        _menubar.width = 300;
                    } else {
                        _menubar.width = 42;
                    }
                }
            }
        }
    }

    Slider { id: _2d_slider
        from: 0
        to: 100
        value: 50
        stepSize: 1
        snapMode: Slider.SnapAlways

        rotation: 90
        visible: false
        width: _view.height

        anchors.verticalCenter: _view.verticalCenter
        anchors.left: _menubar.right
        anchors.leftMargin: 5 - _view.height / 2

        handle.implicitWidth: 10
        handle.implicitHeight: 10

        onValueChanged: {
            _control.sliceChange(value);
        }
    }

    X.Icon { id: _2d_icon;
        icon: X.Icons.icons.crop_square;
        size: 32;
        color: X.Style.foregroundColor;

        anchors.top: _view.top
        anchors.topMargin: 10
        anchors.left: _menubar.right
        anchors.leftMargin: 10

        MouseArea { id: _2d_mouse_area;
            anchors.fill: parent;
            hoverEnabled: true;

            onClicked: {
                _control.switchTo2D();
                _control.sliceChange(_2d_slider.value);
                _2d_slider.visible = true;
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
        anchors.left: _menubar.right
        anchors.leftMargin: 50

        MouseArea { id: _3d_mouse_area;
            anchors.fill: parent;
            hoverEnabled: true;

            onClicked: {
                _control.switchTo3D();
                _2d_slider.visible = false;
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
