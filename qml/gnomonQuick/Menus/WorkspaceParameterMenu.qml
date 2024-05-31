import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P
import QtCore

import gnomonQuick.Menus as G
import gnomonQuick.Workspaces as G
import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {
    id: _menu;

    property var parameters
    property var d

    property bool headerVisible: true

    ColumnLayout {

        anchors.fill: parent
        anchors.margins: G.Style.smallPadding;

        Item {
            Layout.fillWidth: true;
            Layout.preferredHeight: G.Style.iconMedium + 2*G.Style.smallPadding

            visible: _menu.headerVisible

            G.IconButton {
                id: _info_button
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: G.Style.smallPadding

                iconName: "information-outline";
                size: G.Style.iconMedium;
                color: G.Style.colors.textColorFaded;

                onClicked: {
                    _plugin_info_dialog.pluginName = d.algorithmsData[_algos.currentIndex]['name']
                    _plugin_info_dialog.pluginMetaData = d.algoMetaData
                    _plugin_info_dialog.pluginParameters = d.parameters
                    _plugin_info_dialog.open()
                }
            }

            G.ComboBoxWithLabel {
                id: _algos;

                anchors.left: parent.left
                anchors.right: _info_button.left
                anchors.rightMargin: G.Style.smallPadding
                anchors.verticalCenter: _info_button.verticalCenter

                label: "Algorithm:"
                textRole: "name"
                valueRole: "counter"
                model: d ? d.algorithmsData : null;

                onCurrentIndexChanged: {
                    if (d && d.algorithms) {
                        d.currentIndex = _algos.currentIndex;
                        d.algoName = d.algorithms[d.currentIndex];
                    }
                }

                // creating an alias for signal handling
                property string algoName: d ? d.algoName : ""
                onAlgoNameChanged: {
                    if(d.algoName != d.algorithms[_algos.currentIndex]) {
                        for(let i=0; i<model.length; i++) {
                            if(d.algorithms[i] == d.algoName) {
                                _algos.currentIndex = i
                                d.currentIndex = i
                            }
                        }
                    }
                }
            }
        }

        ListView {
            id: _list_view;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            spacing: G.Style.smallColumnSpacing;
            clip: true;

            model: _menu.parameters

            signal valueChanged();

            delegate: G.ParameterGroup {
                required property var parameters;
                required property string group;

                title: _menu.getTitleString(group);
                model: parameters;
                width: _list_view.width;
                collapsed: title != "General";

                onValueChanged: {
                    _list_view.valueChanged();
                }
            }

            onValueChanged: {
                if (_auto_apply.checked) {
                    console.info('launching Run!')
                    d.run();
                }
            }

            ScrollIndicator.vertical: ScrollIndicator {
                visible: _list_view.contentHeight > _list_view.height;
            }

            Connections {
                target: window
                function onInsideParamFigure() {
                    _list_view.interactive = !_list_view.interactive;
                }
            }
        }

        Item {
            id: _button_container

            height: G.Style.largeButtonHeight
            Layout.fillWidth: true;

            G.Button {

                id: _apply

                anchors.right: _stop.left;
                anchors.verticalCenter: _button_container.verticalCenter
                anchors.margins: G.Style.smallPadding

                property bool paused: false
                property bool running: false
                type: !running ? G.Style.ButtonType.Base : paused? G.Style.ButtonType.OK : G.Style.ButtonType.Neutral

                text: !running ? "Run" : paused ? "Resume" : "Pause"
                iconName: !running ? "play" : paused ? "play" : "pause"

                Connections {
                    target: d
                    function onStarted() {
                        _apply.running = true
                    }
                    function onFinished() {
                        _apply.running = false
                    }
                }

                onClicked: {
                    if(!_apply.running){
                        paused=false
                        console.info("Launching run !")
                        d.run()
                    } else if(paused) {
                        d.resume()
                        paused=false
                    } else {
                        d.pause()
                        paused = true
                    }
                }
            }

            G.SquareButton {

                id: _stop

                anchors.right: _button_container.right;
                anchors.verticalCenter: _button_container.verticalCenter
                anchors.margins: G.Style.smallPadding
                type: enabled ? G.Style.ButtonType.Danger : G.Style.ButtonType.Neutral
                iconName: "stop"
                text: "stop"
                tooltip: "stop"
                enabled: _apply.running

                onClicked: {
                    console.info('stopping Run!')
                    d.stop();
                }
            }

            G.CheckBox{
                id: _auto_apply

                anchors.verticalCenter: _button_container.verticalCenter
                anchors.left: _button_container.left
                anchors.bottomMargin: G.Style.smallPadding

                text: "Auto apply"
                checked: false

                Settings {
                    property alias auto_run: _auto_apply.checked
                }

                onClicked: {
                    if (_auto_apply.checked) {
                        console.info('launching Run!')
                        d.run();
                    }
                }
            }
        }
    }

    G.PluginInfoDialog {
        id: _plugin_info_dialog
    }

    function getTitleString(group : string) : string {
        return (group.charAt(0).toUpperCase() + group.slice(1)).replace('_', ' ')
    }
}
