import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P
import Qt.labs.settings

import gnomonQuick.Menus as G
import gnomonQuick.Workspaces as G
import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {
    id: _menu;

    property var parameters
    property var d

    property alias algo_combobox: _algos;

    ColumnLayout {

        anchors.fill: parent
        anchors.margins: G.Style.smallPadding;

        G.ComboBoxWithLabel {
            id: _algos;

            label: "Algorithm:"
            textRole: "name"
            valueRole: "counter"
            model: d ? d.algorithmsData : null;

            Layout.fillWidth: true;
            /* Layout.leftMargin: 20 */
            /* Layout.rightMargin: 20 */

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

        ListView {
            id: _control;

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
                width: _control.width;
                collapsed: title != "General";

                onValueChanged: {
                    _control.valueChanged();
                }
            }

            onValueChanged: {
                if (_auto_apply.checked) {
                    console.info('launching Run!')
                    d.run();
                }
            }

            ScrollIndicator.vertical: ScrollIndicator {
                visible: _control.contentHeight > _control.height;
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

    function getTitleString(group : string) : string {
        return (group.charAt(0).toUpperCase() + group.slice(1)).replace('_', ' ')
    }
}
