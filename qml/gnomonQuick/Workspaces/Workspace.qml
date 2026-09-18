import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style    as G
import gnomon.Visualization as GV
import gnomon.Project   as  GP

G.Page {

    id: _self;

    required property string workspace_title;
    required property var fill;
    required property string uuid;
    objectName: uuid  // not sure if it should be kept

    // default property alias contents: _contents.children;
    property alias parameters: _params.params_model;
    property var d: undefined;
    property bool canBeDestroyed: d? d.canBeDestroyed : true;
    property var viewSelected: undefined;


    Connections {
        target: window
        function onCurrentViewChanged() {
            if (window.currentView) {
                let parent_object = window.currentView.parent
                while(parent_object != _self && parent_object != window && parent_object) {
                    parent_object = parent_object.parent
                }
                if(parent_object === _self) {
                    _self.viewSelected = window.currentView
                }
            }
        }
    }

    Component.onCompleted: {
        // propagate the workspace uuid
        if(d) {
            d.objectName = uuid
        }
        if (_self.viewSelected) {
            window.currentView = _self.viewSelected
        }
    }

    G.Parameters {
        id: _params;
        parameters: d ? d.parameters : null;
    }

    Rectangle {
        id: _banner;

        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        anchors.margins: G.Style.mediumPadding;

        radius: G.Style.panelRadius;
        color: G.Style.colors.gutterColor;
        border.color: G.Style.colors.fgColor;
        border.width: 1;

        width: G.Style.mediumPanelWidth;
        height:  G.Style.largeLabelHeight + _logs_control.height + (_logs_control.opened()? 2*G.Style.smallPadding : 0)

        visible: false;

        G.Dragger {
            id: _logs_dragger;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.verticalCenter: parent.top;
            orientation: Qt.Horizontal;

            z: _self.parent? _self.parent.z + 1 : 0;

            onClicked: {
                if (_logs_control.opened())
                    _logs_control.close();
                else
                    _logs_control.open();
            }
        }

        G.Logs {
            id: _logs_control

            anchors.top: _banner.top;
            anchors.left: parent.left;
            anchors.right: parent.right;
            anchors.margins: G.Style.smallPadding;
        }

        G.ProgressBar {
            id: _banner_progress_bar

            anchors.left: parent.left;
            anchors.right: parent.right;
            anchors.bottom: parent.bottom;
            height: G.Style.largeLabelHeight;

            value: window.load_in_progress ? GP.SessionManager.progress : d ? d.progress : 0
            visible: window.load_in_progress
            opacity: 0.5
            to: 100
            progressValueCentered: false
        }

        BusyIndicator {
            id: _banner_indicator;

            anchors.left: parent.left;
            anchors.verticalCenter: _banner_progress_bar.verticalCenter;
            anchors.margins: G.Style.smallPadding

            palette.dark: G.Style.colors.neutralColor
            height: G.Style.thumbnailLarge;
        }

        Label {
            anchors.left: _banner_indicator.right;
            anchors.right: parent.right;
            anchors.verticalCenter: _banner_progress_bar.verticalCenter;
            anchors.margins: G.Style.smallPadding
            property string progress_message: d ? d.progressMessage ? " - " + d.progressMessage : "" : ""
            property string load_text: "Session loading in progress please dont launch other computations"
            property string computation_text: "Computation in progress" + progress_message

            text: window.load_in_progress ? load_text :  computation_text;

            font: G.Style.fonts.value
        }
    }

    function updateParametersModel(from_workspace=true) {
        if (from_workspace) {
            _params.parameters = d.parameters;
        }
        _params.updateParametersModel();
    }

    G.Toast {
        id: _hibernating_toast

        property int index;
        property var view;

        parent: Overlay.overlay
        header: "Reloading form"
        message: "The form was hibernating, please wait while it is reloaded. This may take a few seconds. (You may change the hibernation threshold in the ⚙ Settings)"

        onIndexChanged: {
            header = "Reloading form " + GV.World.getDynamicFormMetadata(index).data["name"]
        }

        type: G.Style.ButtonType.Base
    }

    function idleStart() {
        _banner.z = Infinity;
        _logs_dragger.z = Infinity;
        _banner.visible = true;
        _banner_indicator.running = true;
        _logs_control.show = true;
        _banner_progress_bar.visible = true
        GV.LogServer.newPendingLogConnection.connect(_logs_control.new_connection)
    }

    function idleStop() {
        _banner.visible = false;
        _banner_progress_bar.visible = window.load_in_progress
        _logs_control.show = false;
        _logs_control.close();
        _logs_control.close_console()
    }

    function dropForm(view, index) {
        if (!GV.World.formLoaded(index)) {
            _hibernating_toast.index = index
            _hibernating_toast.view = view
            _hibernating_toast.open()
        } else {
            view.drop(index);
        }
    }

    Connections {
        target: _hibernating_toast
        function onOpened() {
            _hibernating_toast.view.drop(_hibernating_toast.index);
            _hibernating_toast.close()
        }
    }
}
