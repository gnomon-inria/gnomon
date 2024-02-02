import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import Qt.labs.platform as P

import xQuick.Vis as XVis

import gnomonQuick.Controls as G
import gnomonQuick.Style    as G

import gnomon.Visualization as GV
import gnomon.MetaData      as GM

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

    property bool icons_enabled: true;
    property bool ts_enabled: true;

    signal droppedFromFile(string path)
    signal droppedFromManager(int index)

    signal transmit();

    signal switchTo2D();
    signal switchTo2DXY();
    signal switchTo2DXZ();
    signal switchTo2DYZ();
    signal switchTo3D();
    signal sliceChange(double value);
    signal clear();

    function forceFocus() {
        _view.forceActiveFocus()
    }

    XVis.Viewer {

        id: _view;

        anchors.fill: parent;

        mouseEnabled: true;

        // onCaptured: {
        //     self.transmit();
        // }

        onActiveFocusChanged: {
            if (_view.activeFocus) {
                window.currentView = self;
            }
        }
    }

    DropArea {

        id: _drop;

        anchors.fill: parent;

        G.Icon {
            icon: "arrow-down-drop-circle";
            size: G.Style.smallDelegateHeight;
            color: G.Style.colors.fgColor
            visible: _drop.containsDrag && viewLogic.inputView;
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
        stepSize: (to - from) < 5. ? (to - from)/20 : 1; // ensure that stepSize is somewhat ok for small data
        snapMode: Slider.SnapAlways

        useRadius: true

        onValueChanged: {
          self.sliceChange(value);
        }
    }

    G.TimeSeriesSlider {
        id: _ts_slider;
        times: viewLogic.times
        visible: viewLogic.times.length > 1 && ts_enabled
        enabled: visible

        onValueChanged: {
            viewLogic.currentTime = times[value]
        }
    }

    G.IconButton {
        id: _2d_icon;
        property bool active: viewLogic.mode == GV.View.VIEW_MODE_2D;
        iconName: "crop-free";
        size: G.Style.iconLarge;
        color: active? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        tooltip: "2D mode"
        enabled: icons_enabled
        visible: enabled

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
        source: active? "View-XY.png" : "View-XY-off.png";
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
        source: active? "View-XZ.png" : "View-XZ-off.png";
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
        source: active? "View-YZ.png" : "View-YZ-off.png";
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
        iconName: "cube-outline";
        size: G.Style.iconLarge;
        color: active? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        tooltip: "3D mode";
        enabled: icons_enabled
        visible: enabled

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

        iconName: "palette"
        size: G.Style.iconLarge;
        color: G.Style.colors.textColorNeutral;
        tooltip: "Choose background color"
        enabled: icons_enabled
        visible: enabled

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
        iconName: "video";
        size: G.Style.iconLarge;
        color: active? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        visible: viewLogic.mode == GV.View.VIEW_MODE_3D && icons_enabled
        tooltip: "Reset camera to default positions"
        enabled: visible

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
        iconName: "axis-z-arrow";
        size: G.Style.iconLarge;
        rotation: self.shift_pressed? 180 : 0
        flip: self.ctrl_pressed
        color: G.Style.colors.textColorNeutral;
        visible: viewLogic.mode == GV.View.VIEW_MODE_3D && _camera_icon.active && icons_enabled
        tooltip: "Reset camera to XY axes"
        enabled: visible

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

        iconName: "axis-y-arrow";
        size: G.Style.iconLarge;
        rotation: self.ctrl_pressed ? 180 : 0
        flip: (self.shift_pressed ? !self.ctrl_pressed : self.ctrl_pressed)
        color: G.Style.colors.textColorNeutral;
        visible: viewLogic.mode == GV.View.VIEW_MODE_3D && _camera_icon.active && icons_enabled
        tooltip: "Reset camera to XZ"
        enabled: visible

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
        iconName: "axis-x-arrow"
        size: G.Style.iconLarge;
        rotation: self.ctrl_pressed ? 180 : 0
        flip: (self.shift_pressed ? !self.ctrl_pressed : self.ctrl_pressed)
        color: G.Style.colors.textColorNeutral;
        visible: viewLogic.mode == GV.View.VIEW_MODE_3D && _camera_icon.active && icons_enabled
        tooltip: "Reset camera to YZ"
        enabled: visible

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
        iconName: ["dots-triangle", "vector-triangle", "triangle"][viewLogic.representation]
        size: G.Style.iconLarge;
        color: G.Style.colors.textColorNeutral;
        tooltip: "Set representation to Point/Wireframe/Surface"
        enabled: icons_enabled
        visible: enabled

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
        iconName: viewLogic.gridVisible ? "grid" : "grid-off";
        size: G.Style.iconLarge;
        color: viewLogic.gridVisible ? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        tooltip: "Show/Hide the grid around the objects"
        enabled: icons_enabled
        visible: enabled

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
        iconName: viewLogic.gridType == 0 ? "cube" : ["alpha-x-box", "alpha-y-box", "alpha-z-box"][viewLogic.gridOrientation];
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
        iconName: "axis-arrow"
        size: G.Style.iconLarge;
        color: viewLogic.axesVisible ? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        tooltip: "Show/Hide the axes orientation widget"
        enabled: icons_enabled
        visible: enabled

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
        iconName: "axis-lock"
        size: G.Style.iconLarge;
        color: viewLogic.cameraFixed ? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        tooltip: "Forbid/Allow visualization updates to change the field of view"
        enabled: icons_enabled
        visible: enabled

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
        iconName: viewLogic.synced ? "lock" : "lock-open";
        size: G.Style.iconLarge;
        color: viewLogic.synced ? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        visible: viewLogic.inPool && icons_enabled
        tooltip: "(Un)Link with other views for this workspace"
        enabled: visible

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

        iconName: "camera";
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
        iconName: viewLogic.inputView ? "arrow-down-drop-circle" : "arrow-up-drop-circle";
        enabled: self.export_enabled && icons_enabled
        visible: !viewLogic.inputView && icons_enabled
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

                            G.TextField {
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

    //layer.enabled: true
    //layer.effect: OpacityMask
    //{
    //    maskSource: Rectangle
    //    {
    //         width: self.width
    //        height: self.height
    //        radius: G.Style.panelRadius;
    //    }
    //}

    Rectangle {
        id: _icon_banner

        anchors.left: _view.left
        anchors.right: _view.right
        anchors.top: _view.top
        anchors.margins: G.Style.smallPadding/2

        height: G.Style.iconLarge + G.Style.smallPadding
        radius: G.Style.cardRadius;

        color: G.Style.colors.fgColor
        opacity: 0.33
    }

    Rectangle {
        id: _2d_banner

        anchors.left: _view.left
        anchors.top: _icon_banner.bottom
        anchors.bottom: _2d_slider.bottom
        anchors.margins: G.Style.smallPadding/2

        width: G.Style.iconLarge + G.Style.smallPadding;
        radius: G.Style.cardRadius;

        color: G.Style.colors.fgColor
        opacity: 0.33
        visible: _2d_slider.visible
    }

    Rectangle {
        id: _camera_banner

        anchors.horizontalCenter: _camera_icon.horizontalCenter
        anchors.top: _icon_banner.bottom
        anchors.bottom: _camera_yz_icon.bottom
        anchors.margins: G.Style.smallPadding/2
        anchors.bottomMargin: -G.Style.smallPadding/2

        width: G.Style.iconLarge + G.Style.smallPadding;
        radius: G.Style.cardRadius;

        color: G.Style.colors.fgColor
        opacity: 0.33
        visible: _camera_yz_icon.visible
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

    Component.onCompleted: {
        window.currentView = self;
    }
}
