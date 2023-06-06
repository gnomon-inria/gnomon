import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15
import Qt.labs.platform  1.0 as P

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
import gnomonQuick.Icons as G

Rectangle {

    id: self;

    color: G.Style.colors.bgColor;

    focus: true;

    property alias view: _view;
    property alias ts_slider: _ts_slider
    property var viewLogic;
    property var visualizations;

    property bool shift_pressed: false;
    property bool ctrl_pressed: false;
    property bool export_enabled: !viewLogic.inputView;

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

        G.Icon {
            icon: G.Icons.icons["arrow-down-drop-circle"];
            size: G.Style.smallDelegateHeight;
            color: G.Style.colors.fgColor
            visible: _drop.containsDrag & viewLogic.inputView;
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

    G.Toast {
        id: _badform_toast

        property string badform_name: "";
        property string accepted_forms: "";

        parent: Overlay.overlay
        header: "Unsupported Form !"
        message: "You are trying to load  a form of type: " + _badform_toast.badform_name + ", please select a supported one (" + _badform_toast.accepted_forms + ")";

        type: G.Style.ButtonType.Warning
    }

    G.Slider {
        id: _2d_slider

        x: _2d_yz.x + _2d_slider.height / 2
        y: _2d_yz.y + _2d_slider.width + G.Style.mediumPadding + _2d_slider.height / 2

        transformOrigin: Item.Left
        rotation: -90
        visible: _2d_icon.active;

        from: _2d_xy.active? viewLogic.zMin : _2d_xz.active? viewLogic.yMin : viewLogic.xMin;
        to:   _2d_xy.active? viewLogic.zMax : _2d_xz.active? viewLogic.yMax : viewLogic.xMax;

        value: 10
        stepSize: 1
        snapMode: Slider.SnapAlways

        useRadius: true

        onValueChanged: {
            self.sliceChange(value);
        }
    }

    G.TimeSeriesSlider {
        id: _ts_slider;
        times: viewLogic.times
        visible: viewLogic.times.length > 1

        onValueChanged: {
            viewLogic.currentTime = times[value]
        }
    }

    G.IconButton {
        id: _2d_icon;
        property bool active: viewLogic.mode == GV.View.VIEW_MODE_2D;
        iconName: G.Icons.icons["crop-free"];
        size: G.Style.iconLarge;
        color: active? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        tooltip: "2D mode"

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _view.left
        anchors.leftMargin: G.Style.smallPadding

        onClicked: {
            self.switchTo2D();
            self.sliceChange(_2d_slider.value);
        }
    }

    Image {
        id: _2d_xy;
        property bool active: viewLogic.orientation == GV.View.SLICE_ORIENTATION_XY;
        source: active? "qrc:/qml/gnomonQuick/assets/View-XY.png" : "qrc:/qml/gnomonQuick/assets/View-XY-off.png";
        visible: _2d_icon.active

        height: G.Style.iconLarge
        fillMode: Image.PreserveAspectFit

        anchors.top: _2d_icon.bottom
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _view.left
        anchors.leftMargin: 1.5*G.Style.smallPadding

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

        height: G.Style.iconLarge
        fillMode: Image.PreserveAspectFit

        anchors.top: _2d_xy.bottom
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _view.left
        anchors.leftMargin: 1.5*G.Style.smallPadding

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

        height: G.Style.iconLarge
        fillMode: Image.PreserveAspectFit

        anchors.top: _2d_xz.bottom
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _view.left
        anchors.leftMargin: 1.5*G.Style.smallPadding

        MouseArea { id: _2d_yz_area;
            anchors.fill: parent;

            onClicked: {
                self.switchTo2DYZ();
                self.sliceChange(_2d_slider.value);
            }
        }
    }

    G.IconButton {
        id: _3d_icon;
        property bool active: viewLogic.mode == GV.View.VIEW_MODE_3D;
        iconName: G.Icons.icons["cube-outline"];
        size: G.Style.iconLarge;
        color: active? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        tooltip: "3D mode";

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _2d_icon.right
        anchors.leftMargin: G.Style.smallPadding

        onClicked: {
            self.switchTo3D();
        }
    }

    Connections {
        target: viewLogic
        function onSliceChanged(value) {
            _2d_slider.value = value;
        }

        function onCurrentTimeChanged(time) {
            if (_ts_slider.times.includes(time)) {
                _ts_slider.value = _ts_slider.times.indexOf(time)
            }
        }

        function onBadFormDropped(badFormName, acceptedForms) {
            _badform_toast.badform_name = badFormName;
            _badform_toast.accepted_forms = acceptedForms;
            _badform_toast.open();
        }
    }

    G.IconButton {
        id: _color_icon;

        iconName: G.Icons.icons["palette"]
        size: G.Style.iconLarge;
        color: G.Style.colors.textColorNeutral;
        tooltip: "Choose background color"

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _3d_icon.right
        anchors.leftMargin: G.Style.smallPadding

        P.ColorDialog {
            id: _color_dialog
            onAccepted: {
               viewLogic.bgColor = _color_dialog.color
            }
        }

        onClicked: {
            _color_dialog.color = viewLogic.bgColor
            _color_dialog.open()
        }
    }

    G.IconButton {
        id: _camera_icon;
        property bool active: false;
        iconName: G.Icons.icons["video"];
        size: G.Style.iconLarge;
        color: active? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        visible: viewLogic.mode == GV.View.VIEW_MODE_3D
        tooltip: "Reset camera to default positions"

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _color_icon.right
        anchors.leftMargin: G.Style.smallPadding

        onClicked: {
            active = !active
        }
    }

    G.IconButton {
        id: _camera_xy_icon;
        iconName: G.Icons.icons["axis-z-arrow"];
        size: G.Style.iconLarge;
        rotation: self.shift_pressed? 180 : 0
        flip: self.ctrl_pressed
        color: G.Style.colors.textColorNeutral;
        visible: viewLogic.mode == GV.View.VIEW_MODE_3D && _camera_icon.active
        tooltip: "Reset camera to XY axes"

        anchors.top: _camera_icon.bottom
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _color_icon.right
        anchors.leftMargin: G.Style.smallPadding

        onClicked: {
            viewLogic.setCameraXY(self.shift_pressed, self.ctrl_pressed)
        }
    }

    G.IconButton {
        id: _camera_xz_icon;

        iconName: G.Icons.icons["axis-y-arrow"];
        size: G.Style.iconLarge;
        rotation: self.ctrl_pressed ? 180 : 0
        flip: (self.shift_pressed ? !self.ctrl_pressed : self.ctrl_pressed)
        color: G.Style.colors.textColorNeutral;
        visible: viewLogic.mode == GV.View.VIEW_MODE_3D && _camera_icon.active
        tooltip: "Reset camera to XZ"

        anchors.top: _camera_xy_icon.bottom
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _color_icon.right
        anchors.leftMargin: G.Style.smallPadding

        onClicked: {
            viewLogic.setCameraXZ(self.shift_pressed, self.ctrl_pressed)
        }
    }

    G.IconButton {
        id: _camera_yz_icon;
        iconName: G.Icons.icons["axis-x-arrow"];
        size: G.Style.iconLarge;
        rotation: self.ctrl_pressed ? 180 : 0
        flip: (self.shift_pressed ? !self.ctrl_pressed : self.ctrl_pressed)
        color: G.Style.colors.textColorNeutral;
        visible: viewLogic.mode == GV.View.VIEW_MODE_3D && _camera_icon.active
        tooltip: "Reset camera to YZ"

        anchors.top: _camera_xz_icon.bottom
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _color_icon.right
        anchors.leftMargin: G.Style.smallPadding

        onClicked: {
            viewLogic.setCameraYZ(self.shift_pressed, self.ctrl_pressed)
        }
    }

    G.IconButton {
        id: _representation_button
        iconName: G.Icons.icons[["dots-triangle", "vector-triangle", "triangle"][viewLogic.representation]]
        size: G.Style.iconLarge;
        color: G.Style.colors.textColorNeutral;
        tooltip: "Set representation to Point/Wireframe/Surface"

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _camera_icon.visible? _camera_icon.right : _color_icon.right
        anchors.leftMargin: G.Style.smallPadding

        onClicked: {
            viewLogic.representation = (viewLogic.representation + 1)%3
        }
    }

    G.IconButton {
        id: _grid_button;
        iconName: viewLogic.gridVisible ? G.Icons.icons["grid"] : G.Icons.icons["grid-off"];
        size: G.Style.iconLarge;
        color: viewLogic.gridVisible ? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        tooltip: "Show/Hide the grid around the objects"

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _representation_button.right
        anchors.leftMargin: G.Style.smallPadding

        onClicked: {
            viewLogic.gridVisible = !viewLogic.gridVisible
        }
    }

    G.IconButton {
        id: _grid_type_button;
        visible: viewLogic.gridVisible
        iconName: viewLogic.gridType == 0 ? G.Icons.icons["cube"] : G.Icons.icons[["alpha-x-box", "alpha-y-box", "alpha-z-box"][viewLogic.gridOrientation]];
        size: G.Style.iconLarge;
        color: viewLogic.gridVisible ? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        tooltip: "Display grid cube or only X/Y/Z grid plane"

        anchors.top: _grid_button.bottom
        anchors.topMargin: G.Style.smallPadding
        anchors.horizontalCenter: _grid_button.horizontalCenter

        onClicked: {
            if (viewLogic.gridType == 0) {
                viewLogic.gridType = 1
                viewLogic.gridOrientation = 0
            } else {
                if (viewLogic.gridOrientation == 2) {
                    viewLogic.gridType = 0
                } else {
                    viewLogic.gridOrientation += 1
                }
            }
        }
    }

    G.IconButton {
        id: _axes_button;
        iconName: G.Icons.icons["axis-arrow"]
        size: G.Style.iconLarge;
        color: viewLogic.axesVisible ? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        tooltip: "Show/Hide the axes orientation widget"

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _grid_button.right
        anchors.leftMargin: G.Style.smallPadding

        onClicked: {
            viewLogic.axesVisible = !viewLogic.axesVisible
        }
    }

    G.IconButton {
        id: _fixed_camera_button;
        iconName: G.Icons.icons["axis-lock"]
        size: G.Style.iconLarge;
        color: viewLogic.cameraFixed ? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        tooltip: "Forbid/Allow visualization updates to change the field of view"

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _axes_button.right
        anchors.leftMargin: G.Style.smallPadding

        onClicked: {
            viewLogic.cameraFixed = !viewLogic.cameraFixed
        }
    }

    G.IconButton {
        id: _link;
        iconName: viewLogic.synced ? G.Icons.icons["lock"] : G.Icons.icons["lock-open"];
        size: G.Style.iconLarge;
        color: viewLogic.synced ? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        visible: viewLogic.inPool
        tooltip: "(Un)Link with other views for this workspace"

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.left: _fixed_camera_button.right
        anchors.leftMargin: G.Style.smallPadding

        onClicked: {
            viewLogic.tryLinking();
        }
    }

    Keys.onReleased: (event) => {
        event.accepted = false

        self.shift_pressed = (event.modifiers & Qt.ShiftModifier) != 0
        self.ctrl_pressed = (event.modifiers & Qt.ControlModifier) != 0

        event.accepted = _view.keyPressed(event.key)
    }

    Keys.onPressed: (event) => {
        event.accepted = false

        self.shift_pressed = (event.modifiers & Qt.ShiftModifier) != 0
        self.ctrl_pressed = (event.modifiers & Qt.ControlModifier) != 0

        // ctrl + E
        if (event.key == Qt.Key_E && event.modifiers & Qt.ControlModifier) {
            event.accepted = true
            if(event.modifiers & Qt.ShiftModifier) {
                viewLogic.transmit()
            } else if (_list_view.count > 0){
                _form_export_dialog.open();
                _form_export_dialog.reset();
            }
        }

        if (event.key == Qt.Key_S) {
            viewLogic.representation = 2
        } else if (event.key == Qt.Key_W) {
            viewLogic.representation = 1
        }

        if (event.key == Qt.Key_R    //ResetCamera
            || event.key == Qt.Key_S //Surface
            || event.key == Qt.Key_W //Wireframe
            || event.key == Qt.Key_P //Point?
            || event.key == Qt.Key_U) { //userEvent
            // if(viewLogic.acceptKey() // can do like this to restrict to certain views only
            event.accepted = _view.keyPressed(event.key)
            //event.accepted = viewLogic.keyPressed(event.key)
        }
    }

    G.IconButton {
        id: _screenshot_icon;

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.right: _export_icon.visible? _export_icon.left : _view.right
        anchors.rightMargin: G.Style.smallPadding

        iconName: G.Icons.icons["camera"];
        size: G.Style.iconLarge;
        color: G.Style.colors.textColorNeutral

        onClicked: {
            _screenshot_dialog.open()
        }

        P.FileDialog {
            id: _screenshot_dialog

            nameFilters: [ "Image files (*.png)" ]
            title: "Save screenshot"
            modality: Qt.WindowModal;
            fileMode: P.FileDialog.SaveFile

            onAccepted: {
                viewLogic.saveScreenshot(decodeURIComponent(_screenshot_dialog.file));
            }
        }
    }

    G.IconButton {
        id: _export_icon;
        iconName: viewLogic.inputView ? G.Icons.icons["arrow-down-drop-circle"] : G.Icons.icons["arrow-up-drop-circle"];
        enabled: self.export_enabled
        visible: !viewLogic.inputView
        size: G.Style.iconLarge;
        color: self.export_enabled ? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        hoverColor : self.export_enabled ? G.Style.colors.hoveredBaseColor : G.Style.colors.fgColor;
        tooltip: self.export_enabled? "Export" : ""

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.right: _view.right
        anchors.rightMargin: G.Style.smallPadding

        onClicked: {
            if(_list_view.count > 1) {
                _form_export_dialog.open();
                _form_export_dialog.reset();
            } else {
                viewLogic.transmit()
            }
        }

        G.Dialog {
            id: _form_export_dialog;

            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: G.Style.mediumDialogWidth;
            height: G.Style.mediumDialogHeight;

            padding: G.Style.smallPadding;

            parent: Overlay.overlay
            modal: true
            title: "Upload forms"
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
                anchors.margins: G.Style.smallPadding;


                Component {  id: _delegate;
                    G.ListItemDelegate {
                        id: _form_metadata_panel;

                        width: _list_view.width

                        property var metadata: viewLogic.formMetadata(modelData);
                        property var form_name: _form_name;

                        onClicked: {
                            _list_view.currentIndex = index;
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
                            anchors.top: parent.top;
                            anchors.margins: G.Style.smallPadding;

                            width: parent.width/4

                            font: G.Style.fonts.value
                            verticalAlignment: Text.AlignVCenter
                            text: modelData ? modelData.replace('gnomon', '') :"";
                        }

                        G.Gutter {
                            anchors.left: _name_label.right;
                            anchors.right: parent.right;
                            anchors.top: parent.top;
                            anchors.bottom: parent.bottom;
                            anchors.margins: G.Style.smallPadding

                            TextField {
                                id: _form_name;
                                anchors.fill: parent
                                leftPadding: G.Style.smallPadding

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
    }

    layer.enabled: true
    layer.effect: OpacityMask
    {
        maskSource: Rectangle
        {
             width: self.width
            height: self.height
            radius: G.Style.panelRadius;
        }
    }

    Rectangle {

        id: _focus_indicator;

         width: self.width - 1
        height: self.height - 1
        radius: G.Style.panelRadius;

        color: G.Style.colors.transparent;

        border.width: G.Style.borderWidth;
        border.color: G.Style.colors.baseColor;

        visible: window.currentView == self;
    }

    Component.onCompleted: window.currentView = self;
}
