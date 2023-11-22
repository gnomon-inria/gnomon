import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style    as G
import gnomon.Visualization as GV

Control {
        id: _self

    enabled: false
    visible: false

    property int minimumSize:  2;
    property int maximumSize: G.Style.smallPanelHeight;

    property var log_connection: undefined;
    property bool show: false;

    function opened() { return state === "open"; }
    function open() { _self.state = "open"; }
    function close() { _self.state = "closed"; }

    background: Rectangle {
        anchors.fill: parent
        color: G.Style.colors.fgColor
        opacity: _self.opened() ? 0.7 : 0
    }

    Label {
        id: _logs_title
        anchors.top: _self.top
        anchors.left: _self.left
        anchors.right: _self.right

        anchors.margins: G.Style.mediumPadding;
        text: qsTr("Logs")
        font: G.Style.fonts.formLabel

        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        visible: _self.opened()

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

            text: _self.log_connection ? _self.log_connection.text : "/!\\ Disconnected /!\\"
            readOnly: true
            font: G.Style.fonts.value

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignTop
            visible: _self.opened()

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

    states: [
        State{
            name: "closed"
            PropertyChanges {
                target: _self;
                implicitHeight: _self.minimumSize;
            }
        },
        State{
            name: "open"
            PropertyChanges {
                target: _self;
                implicitHeight: _self.maximumSize;
            }
        }
    ]

    transitions: [
        Transition {
            to: "closed"
            NumberAnimation{
                duration: 200
                properties: "implicitHeight, opacity"
                easing.type: Easing.InCubic
            }
        },
        Transition {
            to: "open"
            NumberAnimation{
                duration: 200
                properties: "implicitHeight, opacity"
                easing.type: Easing.OutCubic
            }
        }
    ]

    function display_console() {
        _self.z = Infinity;
        _self.enabled = true;
        _self.visible = true;
    }

    function close_console() {
        _self.visible = false;
        _self.enabled = false;
        if(_self.log_connection) {
            _self.log_connection.close()
        }
        _self.log_connection = undefined
    }

    function new_connection() {
        _self.log_connection = GV.LogServer.getPendingConnection();
        GV.LogServer.newPendingLogConnection.disconnect(_self.new_connection)
        if(_self.log_connection && _self.log_connection.alive) {
            if(_self.show){
                _self.display_console()
            }
        }
    }
}