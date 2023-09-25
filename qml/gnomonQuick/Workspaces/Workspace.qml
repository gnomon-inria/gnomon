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
    property string uuid: undefined;

    // default property alias contents: _contents.children;
    property alias parameters: _params.params_model;
    property var d: undefined;
    property bool canBeDestroyed: d? d.canBeDestroyed : true;

    G.Parameters {
        id: _params;
        parameters: d ? d.parameters : null;
    }

    Control {
        id: _logs_control
        anchors.top: parent.top
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: _banner.top;
        anchors.margins: G.Style.mediumPadding;

        enabled: false
        visible: false

        property var log_connection: undefined;
        property bool show: false;

        background: Rectangle {
            anchors.fill: parent
            color: G.Style.colors.fgColor
            opacity: 0.7
        }

        Label {
            id: _logs_title
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            //anchors.bottom: parent.bottom
            anchors.margins: G.Style.mediumPadding;
            text: qsTr("Logs")
            font: G.Style.fonts.formLabel

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            wrapMode: Text.Wrap
            color: G.Style.colors.textColorBase
        }

        ScrollView {
            anchors.top: _logs_title.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: G.Style.mediumPadding
            contentWidth: availableWidth

            G.TextArea {
                id: _console

                text: _logs_control.log_connection ? _logs_control.log_connection.text : "/!\\ Disconnected /!\\"
                readOnly: true
                font: G.Style.fonts.value

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop

                wrapMode: Text.Wrap
                color: G.Style.colors.textColorBase
                background: G.Gutter {
                    //anchors.fill: parent
                    opacity: 0.7
                }
                onTextChanged: {
                    _console.cursorPosition = _console.length-1
                }
            }
        }



        function display_console() {
            _logs_control.z = Infinity;
            _logs_control.enabled = true;
            _logs_control.visible = true;
        }

        function close_console() {
            _logs_control.visible = false;
            _logs_control.enabled = false;
            if(_logs_control.log_connection) {
                _logs_control.log_connection.close()
            }
            _logs_control.log_connection = undefined
        }

        function new_connection() {
            _logs_control.log_connection = GV.LogServer.getPendingConnection();
            GV.LogServer.newPendingLogConnection.disconnect(_logs_control.new_connection)
            if(_logs_control.log_connection && _logs_control.log_connection.alive) {
                if(_logs_control.show){
                    _logs_control.display_console()
                }
            }
        }
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
        height: G.Style.largeLabelHeight;

        visible: false;

        G.ProgressBar {
            id: _banner_progress_bar
            anchors.fill: parent
            value: window.load_in_progress ? GP.SessionManager.progress : d ? d.progress : 0
            visible: window.load_in_progress
            opacity: 0.5
            to: 100
        }

        BusyIndicator {
            id: _banner_indicator;

            anchors.left: parent.left;
            anchors.verticalCenter: parent.verticalCenter;
            anchors.margins: G.Style.smallPadding

            palette.dark: G.Style.colors.neutralColor
            height: G.Style.thumbnailLarge;
        }

        Label {
            anchors.left: _banner_indicator.right;
            anchors.right: parent.right;
            anchors.verticalCenter: parent.verticalCenter;
            anchors.margins: G.Style.smallPadding

            text: window.load_in_progress ? "Session loading in progress please dont launch other computations" : "Computation in progress" ;

            font: G.Style.fonts.value
        }
    }

    function updateParametersModel() {
        _params.parameters = d.parameters;
        _params.updateParametersModel();
    }

    function idleStart() {
        _banner.z = Infinity;
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
        _logs_control.close_console()
    }
}
