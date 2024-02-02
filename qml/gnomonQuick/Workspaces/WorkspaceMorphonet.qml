import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import gnomon.Utils as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G

import gnomon.Workspaces    1.0 as GW

G.Workspace {

    id: _self;

    workspace_title: "MorphoNet";

    property alias mn_ds_info: mn_datasets_info;

    fill: () => {
        if(world.currentRef >= 0)
            _view.droppedFromManager(world.currentRef);
    }
    focus: true;

    viewSelected: _view

    G.SessionSettings {
        category: uuid
        property alias state: d.state
    }

    d: GW.WorkspaceMorphonet {
        id: d;

        onStarted: idleStart();
        onFinished: idleStop();

        onMessage: {
            console.log("message: ", msg)
            _message_dialog.text = msg;
            _message_dialog.open();

        }

        onConnectionStatusChanged: {
            setDatasetInfos();
            d.connected ? drawer.open() : drawer.close()
        }

    }

    Timer {
        id: timer
    }

    ListModel {
        id: mn_datasets_info
    }

    G.Button {
        x: parent.width / 2 - 10;
        y: parent.height /2 - 10;
        visible: !d.connected;
        text: "Connect";
        onClicked: {
            _connection_dialog.open();
        }
    }

    G.View {
        id: _view;

        anchors.fill: parent;
        anchors.margins: G.Style.smallPadding;

        visible: d.connected
        onDroppedFromManager: (index) => {
            window.currentView = _view
            d.view.drop(index);
            d.uploadMode = true;
        }

        viewLogic: d.view;

        Component.onCompleted: G.Associator.associate(_view, d.view);

    }

    Component.onCompleted: {
        timer.interval= 500;
        timer.repeat= false;
        timer.triggered.connect(function ff () {
            if(!d.connected) {
                _connection_dialog.open();
                drawer.close()
            }
            timer.triggered.disconnect(ff);
        });
        timer.start();

        drawel.close();
    }

    G.Dialog {
        id: _message_dialog;

        property string text: "";

        y: parent.height/3
        x: parent.width/6
        parent: Overlay.overlay

        Label {
            anchors.fill: parent
            text: _message_dialog.text;
            font {
                weight: Font.Bold
                pointSize: 14
            }
        }
    }

    G.Dialog {
        id: _connection_dialog

        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)

        width: Math.round(window.width / 3 * 2)

        parent: Overlay.overlay
        focus: true
        modal: true

        standardButtons: Dialog.Ok | Dialog.Cancel

        title: " Connect to Morphonet"
        GridLayout {

            columns: 2
            width: parent.width

            Label {
                text: "login";
                font {
                    pointSize: 14;
                    weight: Font.Bold
                }
            }

            G.TextField {
                id: _morphonet_login
                placeholderText: qsTr("Enter login here")
                text: ""
                implicitWidth: Math.round(_connection_dialog.width/2)
                implicitHeight: Math.round(_connection_dialog.height/8)
                wrapMode: TextInput.WrapAnywhere
                Keys.onReturnPressed: {
                    _connection_dialog.accept()
                }

            }

            Label {
                text: "Password";
                font {
                    pointSize: 14;
                    weight: Font.Bold
                }
            }

            G.TextField {
                id: _morphonet_passwd
                placeholderText: qsTr("Enter password here")
                text: ""
                echoMode: TextInput.PasswordEchoOnEdit
                implicitWidth: Math.round(_connection_dialog.width/2)
                implicitHeight: Math.round(_connection_dialog.height/4)
                wrapMode: TextInput.WrapAnywhere
                Keys.onReturnPressed: {
                    _connection_dialog.accept()
                }
            }
        }

        onRejected: {
            _morphonet_passwd = ""
        }

        onAccepted: {
            let res = d.login(_morphonet_login.text, _morphonet_passwd.text)
            if(!res) {
                _connection_dialog.open()
            }
        }
    }

    function setDatasetInfos() {
        let str_ds = d.importDatasetInfos();
        let ds_info_json = JSON.parse(str_ds);

        mn_datasets_info.clear();

        for(let i in ds_info_json) {
            mn_datasets_info.append({
                "textRole": "name", // to display in ComboBox
                "name": ds_info_json[i]["name"],
                "morpho_id": ds_info_json[i]["id"],
                "comments": ds_info_json[i]["comments"] || "",
                "date": ds_info_json[i]["date"],
                "owner": ds_info_json[i]["owner"],
                "own": ds_info_json[i]["own"]
                });
        }
    }
}
