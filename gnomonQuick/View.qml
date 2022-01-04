import QtQuick            2.15
import QtQuick.Controls   2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts    1.15

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

import gnomonQuick        1.0 as G

import gnomon.Visualization 1.0 as GV

Rectangle {

    id: self;

    color: Qt.darker(X.Style.alternateBaseColor);

    focus: true;

    property alias view: _view;
    property var viewLogic;
    property var visualizations;

    signal droppedFromFile(string path)
    signal droppedFromManager(int index)

    signal transmit();

    signal switchTo2D();
    signal switchTo2DXY();
    signal switchTo2DXZ();
    signal switchTo2DYZ();
    signal switchTo3D();
    signal sliceChange(int value);
    signal clear();

    XVis.Viewer {

        id: _view;

        anchors.fill: parent;

        mouseEnabled: true;

        // onCaptured: {
        //     self.transmit();
        // }

        onActiveFocusChanged: {
            if (_view.activeFocus)
                window.currentView = self;
        }
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
                console.log("DROP", drop.mimeData)
                drop.urls.forEach(function (item, index) {
                    console.log("DROP FILE", item)
                    self.droppedFromFile(item);
                });
            } else {
                self.droppedFromManager(drag.source.ref);
            }

            drop.accept();
        }
    }

    Slider { id: _2d_slider
        from: _2d_xy.active? viewLogic.zMin : _2d_xz.active? viewLogic.yMin : viewLogic.xMin;
        to:   _2d_xy.active? viewLogic.zMax : _2d_xz.active? viewLogic.yMax : viewLogic.xMax;
        value: 10
        stepSize: 1
        snapMode: Slider.SnapAlways

        rotation: 90
        visible: false
        width: _view.height

        anchors.verticalCenter: _view.verticalCenter
        anchors.left: _view.left
        anchors.leftMargin: 5 - _view.height / 2

        handle.implicitWidth: 10
        handle.implicitHeight: 10

        onValueChanged: {
            self.sliceChange(value);
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
                self.switchTo2D();
            }
        }

        ToolTip.visible: _2d_mouse_area.containsMouse;
        ToolTip.text: "2D mode";
    }

    
    // G.ButtonViewer {}

    Image {
        id: _2d_xy;
        property bool active: true
        source: active? "qrc:/qml/gnomonQuick/View-XY.png" : "qrc:/qml/gnomonQuick/View-XY-off.png";
        visible: false
        // size: 32;
        // color: X.Style.foregroundColor;

        anchors.top: _view.top
        anchors.topMargin: 50
        anchors.left: _view.left
        anchors.leftMargin: 13

        MouseArea { id: _2d_xy_area;
            anchors.fill: parent;

            onClicked: {
                self.switchTo2DXY();
                self.sliceChange(_2d_slider.value);
            }
        }
    }

    Image {
        id: _2d_xz;
        property bool active: false
        source: active? "qrc:/qml/gnomonQuick/View-XZ.png" : "qrc:/qml/gnomonQuick/View-XZ-off.png";
        visible: false
        // size: 32;
        // color: X.Style.foregroundColor;

        anchors.top: _view.top
        anchors.topMargin: 90
        anchors.left: _view.left
        anchors.leftMargin: 13

        MouseArea { id: _2d_xz_area;
            anchors.fill: parent;

            onClicked: {
                self.switchTo2DXZ();
            }
        }
    }

    Image {
        id: _2d_yz;
        property bool active: false
        source: active? "qrc:/qml/gnomonQuick/View-YZ.png" : "qrc:/qml/gnomonQuick/View-YZ-off.png";
        visible: false;
        // size: 32;
        // color: X.Style.foregroundColor;

        anchors.top: _view.top
        anchors.topMargin: 130
        anchors.left: _view.left
        anchors.leftMargin: 13

        MouseArea { id: _2d_yz_area;
            anchors.fill: parent;

            onClicked: {
                self.switchTo2DYZ();
            }
        }
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
                self.switchTo3D();
            }
        }

        ToolTip.visible: _3d_mouse_area.containsMouse;
        ToolTip.text: "3D mode";
    }

    Connections {
        target: viewLogic

        function onSwitchedTo3D() {
            _2d_slider.visible = false;
            _2d_xy.visible = false;
            _2d_xz.visible = false;
            _2d_yz.visible = false;
        }

        function onSwitchedTo2D() {
            _2d_slider.visible = true;
            self.sliceChange(_2d_slider.value);
            _2d_xy.visible = true;
            _2d_xz.visible = true;
            _2d_yz.visible = true;
        }

        function onSwitchedTo2DXY() {
            self.sliceChange(_2d_slider.value);
            _2d_xy.active = true;
            _2d_xz.active = false;
            _2d_yz.active = false;
        }

        function onSwitchedTo2DXZ() {
            self.sliceChange(_2d_slider.value);
            _2d_xy.active = false;
            _2d_xz.active = true;
            _2d_yz.active = false;
        }

        function onSwitchedTo2DYZ() {
            self.sliceChange(_2d_slider.value);
            _2d_xy.active = false;
            _2d_xz.active = false;
            _2d_yz.active = true;
        }
    }

    X.Icon {
        id: _link;
        icon: viewLogic.synced ? X.Icons.icons.lock_outline : X.Icons.icons.lock_open;
        size: 32;
        color: X.Style.foregroundColor;

        anchors.top: _view.top
        anchors.topMargin: 10
        anchors.left: _view.left
        anchors.leftMargin: 90

        MouseArea {
            id: _link_area;

            anchors.fill: parent;

            hoverEnabled: true;

            onClicked: {
                viewLogic.tryLinking();
            }
        }

        ToolTip.visible: _link_area.containsMouse;
        ToolTip.text: "(Un)Link with other views for this workspace";
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

                viewLogic.transmit();

                // _view.requestCapture();
                // _view.update();
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
             width: self.width
            height: self.height
            radius: 4;
        }
    }

    Rectangle {

        id: _focus_indicator;

         width: self.width - 1
        height: self.height - 1
        radius: 4;

        color: "#00000000";

        border.width: 2;
        border.color: X.Style.accentColor;

        visible: window.currentView == self;
    }

    Component.onCompleted: window.currentView = self;
}
