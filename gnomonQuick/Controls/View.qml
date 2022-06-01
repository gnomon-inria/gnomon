import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15

import Qt5Compat.GraphicalEffects

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

import gnomonQuick.Controls  1.0 as G
import gnomonQuick.Style  1.0 as G

import gnomon.Visualization 1.0 as GV
import gnomon.MetaData    1.0 as GM

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Rectangle {

    id: self;

    color: G.Style.colors.bgColor; //Qt.darker(X.Style.alternateBaseColor);

    focus: true;

    property alias view: _view;
    property alias ts_slider: _ts_slider
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

        onDropped: (drop) => {
            _view.focus = true
            if (drop.hasUrls) {
                self.droppedFromFile(drop.urls);
            } else {
                self.droppedFromManager(drag.source.ref);
            }

            drop.accept();
        }
    }

    X.Dialog {
        id: _bad_form_warning_dialog;

        property string badform_name: "";
        property string accepted_forms: "";

        y: parent.height/3
        x: parent.width/6

        parent: Overlay.overlay


            X.Label {
                anchors.fill: parent
                text: "You are trying to add a form of type: " + _bad_form_warning_dialog.badform_name + "\n , please select a suitted one: " + _bad_form_warning_dialog.accepted_forms;
                font {
                    weight: Font.Bold
                    pointSize: 14;
                }
            }
    }

    Slider { id: _2d_slider
        from: _2d_xy.active? viewLogic.zMin : _2d_xz.active? viewLogic.yMin : viewLogic.xMin;
        to:   _2d_xy.active? viewLogic.zMax : _2d_xz.active? viewLogic.yMax : viewLogic.xMax;
        value: 10
        stepSize: 1
        snapMode: Slider.SnapAlways

        rotation: 90
        visible: _2d_icon.active;
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

    G.TimeSeriesSlider { id: _ts_slider;
        times: viewLogic.times
        visible: viewLogic.times.length > 1

        onValueChanged: {
            viewLogic.currentTime = value
        }
    }

    X.Icon { id: _2d_icon;
        property bool active: viewLogic.mode == GV.View.VIEW_MODE_2D;
        icon: X.Icons.icons.crop_square;
        size: 32;
        color: active? X.Style.foregroundColor : X.Style.backgroundColor;

        anchors.top: _view.top
        anchors.topMargin: 10
        anchors.left: _view.left
        anchors.leftMargin: 10

        MouseArea { id: _2d_mouse_area;
            anchors.fill: parent;
            hoverEnabled: true;

            onClicked: {
                self.switchTo2D();
                self.sliceChange(_2d_slider.value);
            }
        }

        ToolTip.visible: _2d_mouse_area.containsMouse;
        ToolTip.text: "2D mode";
    }


    // G.ButtonViewer {}

    Image {
        id: _2d_xy;
        property bool active: viewLogic.orientation == GV.View.SLICE_ORIENTATION_XY;
        source: active? "qrc:/qml/gnomonQuick/assets/View-XY.png" : "qrc:/qml/gnomonQuick/assets/View-XY-off.png";
        visible: _2d_icon.active
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
        property bool active: viewLogic.orientation == GV.View.SLICE_ORIENTATION_XZ;
        source: active? "qrc:/qml/gnomonQuick/assets/View-XZ.png" : "qrc:/qml/gnomonQuick/assets/View-XZ-off.png";
        visible: _2d_icon.active;
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
                self.sliceChange(_2d_slider.value);
            }
        }
    }

    Image {
        id: _2d_yz;
        property bool active: viewLogic.orientation == GV.View.SLICE_ORIENTATION_YZ;
        source: active? "qrc:/qml/gnomonQuick/assets/View-YZ.png" : "qrc:/qml/gnomonQuick/assets/View-YZ-off.png";
        visible: _2d_icon.active;
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
                self.sliceChange(_2d_slider.value);
            }
        }
    }

    X.Icon { id: _3d_icon;
        property bool active: viewLogic.mode == GV.View.VIEW_MODE_3D;
        icon: X.Icons.icons._3d_rotation;
        size: 32;
        color: active? X.Style.foregroundColor : X.Style.backgroundColor;

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
        function onSliceChanged(value) {
            _2d_slider.value = value;
        }
        function onBadFormDropped(badFormName, acceptedForms) {
            _bad_form_warning_dialog.badform_name = badFormName;
            _bad_form_warning_dialog.accepted_forms = acceptedForms;
            _bad_form_warning_dialog.open();
        }
    }

    X.Icon {
        id: _link;
        icon: viewLogic.synced ? X.Icons.icons.lock_outline : X.Icons.icons.lock_open;
        size: 32;
        color: viewLogic.synced ? X.Style.foregroundColor : X.Style.backgroundColor;
        visible: viewLogic.inPool

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

    Keys.onPressed: (event) => {
        console.log(event.key)
        event.accepted = false
        if (event.key == Qt.Key_E && event.modifiers & Qt.ControlModifier) {
            event.accepted = true
            if(event.modifiers & Qt.ShiftModifier) {
                viewLogic.transmit()
            } else if (_list_view.count > 0){
                _form_export_dialog.open();
                _form_export_dialog.reset();
            }
        }
    }

    X.Icon { id: _export_icon;
        icon: viewLogic.inputView ? X.Icons.icons.arrow_circle_down : X.Icons.icons.arrow_circle_up;
        enabled: !viewLogic.inputView
        size: 32;
        color: viewLogic.inputView ? X.Style.backgroundColor : X.Style.foregroundColor;

        anchors.top: _view.top
        anchors.topMargin: 10
        anchors.right: _view.right
        anchors.rightMargin: 10

        MouseArea { id: _export_mouse_area;
            anchors.fill: parent;
            hoverEnabled: true;

            onClicked: {
                if(_list_view.count > 1) {
                    _form_export_dialog.open();
                    _form_export_dialog.reset();
                } else {
                    viewLogic.transmit()
                }
            }
        }

        G.Dialog {
            id: _form_export_dialog;

            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: G.Style.mediumDialogWidth;
            height: G.Style.mediumDialogHeight;

            padding: 10;

            parent: Overlay.overlay
            modal: true
            title: "Export forms"
            standardButtons:  Dialog.Ok | Dialog.Cancel

            onAccepted: {
                _form_export_dialog.enabled = false;
                for(let i = 0; i < _list_view.count; i++) {
                    var item_delegate = _list_view.itemAtIndex(i);
                    item_delegate.save_metadata();
                }

                viewLogic.transmit();
                _form_export_dialog.close();
                //_form_export_dialog.destroy();
            }

            onRejected: {
                _form_export_dialog.enabled = false;
                _form_export_dialog.close();
                //_form_export_dialog.destroy();

            }

            function reset(){
                for(let i = 0; i < _list_view.count; i++) {
                    var item_delegate = _list_view.itemAtIndex(i);
                    item_delegate.load_metadata();
                }
                _list_view.currentIndex = 0;
                _list_view.select_field();
                _form_export_dialog.enabled = true;
                //form_name.selectAll();
                //form_name.forceActiveFocus();
            }

            G.Gutter {
                id: _form_selection_panel;

                width: _form_export_dialog.width / 3;

                anchors.top: parent.top;
                anchors.bottom: parent.bottom;
                anchors.left: parent.left;
                anchors.right: parent.right;
                anchors.margins: 10;


                Component {  id: _delegate;
                    ItemDelegate {
                        id: _form_metadata_panel;

                        width: _list_view.width

                        //color: X.Style.backgroundColor;
                        property var metadata: viewLogic.formMetadata(modelData);
                        property var form_name: _form_name;

                        onClicked: {
                            _list_view.currentIndex = index;
                        }

                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 60
                            opacity: enabled ? 0.8 : 0.3
                            color: getBgColor()

                            Rectangle {
                                width: parent.width
                                height: 1
                                color: getEmbossColor();
                                anchors.bottom: parent.bottom
                            }
                        }

                        function getEmbossColor() {
                          return G.Style.colors.embossColorNeutral;
                        }

                        function getBgColor() {
                          if(hovered) return G.Style.colors.neutralColor;
                          return G.Style.colors.fgColor;
                        }

                        function save_metadata() {
                            metadata.set("name", _form_name.text);
                        }

                        function load_metadata() {
                            metadata = viewLogic.formMetadata(modelData);
                            _form_name.text = metadata.get("name");
                        }

                        Label {
                            id: _name_label
                            anchors.left: parent.left;
                            anchors.bottom: parent.bottom;
                            anchors.verticalCenter: _form_name.verticalCenter;
                            anchors.margins: 5;

                            width: parent.width/3

                            font: G.Style.fonts.value
                            verticalAlignment: Text.AlignVCenter
                            text: modelData ? modelData.replace('gnomon', '') :"";
                        }

                        TextField {
                            id: _form_name;

                            anchors.left: _name_label.right;
                            anchors.right: parent.right;
                            anchors.top: parent.top;
                            anchors.bottom: parent.bottom;
                            anchors.margins: 5;

                            width: 3*parent.width/5

                            font: G.Style.fonts.value
                            text: "";
                            //focus: true;
                            onAccepted: {
                                if(index != _list_view.count - 1) {
                                    _list_view.incrementCurrentIndex();
                                } else {
                                    _form_export_dialog.accept();
                                }
                            }

                            onActiveFocusChanged: {
                                if (focus) {
                                    _list_view.currentIndex = index;
                                }
                            }
                        }
                    }
                }


                ListView {
                    id: _list_view

                    anchors.top: _form_selection_panel.top;
                    anchors.bottom: _form_selection_panel.bottom;
                    anchors.right: _form_selection_panel.right;
                    anchors.left: _form_selection_panel.left;
                    clip: true;
                    //focus: true;
                    currentIndex: -1
                    keyNavigationWraps: false;

                    model: viewLogic.formNames

                    ScrollIndicator.vertical: ScrollIndicator { }

                    delegate: _delegate;


                    Component.onCompleted: {

                        for(let i = 0; i < model.count; i++)
                            if(model.get(i).available) {
                                currentIndex = i;
                                break;
                            }
                        /* currentIndex = 0 */
                    }

                    onCurrentIndexChanged: {
                        select_field();
                    }

                    function select_field() {
                        if(currentItem) {
                            currentItem.form_name.selectAll();
                            currentItem.form_name.forceActiveFocus();
                        }
                    }
                }
            }
        }

        ToolTip.visible: !viewLogic.inputView && _export_mouse_area.containsMouse;
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
        border.color: G.Style.colors.baseColor;

        visible: window.currentView == self;
    }

    Component.onCompleted: window.currentView = self;
}
