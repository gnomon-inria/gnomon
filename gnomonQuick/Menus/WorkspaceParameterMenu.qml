import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P
import Qt.labs.settings

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import gnomonQuick.Menus as G
import gnomonQuick.Workspaces as G
import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G

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

        ProgressBar {
            Layout.fillWidth: true;
            indeterminate: d.progress == -1
            from: 0
            to: 100
            value: d.progress
        }

        Item {
            id: _controll_button_container

            height: G.Style.largeButtonHeight
            Layout.fillWidth: true;

            G.Button {

                id: _pause

                anchors.left: _controll_button_container.left;
                anchors.verticalCenter: _controll_button_container.verticalCenter
                anchors.margins: G.Style.smallPadding
                property bool paused: false
                type: paused? G.Style.ButtonType.OK : G.Style.ButtonType.Neutral

                text: paused ? "Resume" : "Pause"
                iconName: paused ? G.Icons.icons["play"] : G.Icons.icons["pause"]

                onClicked: {
                    if(paused) {
                        d.resume()
                        paused = false
                    } else {
                        d.pause()
                        paused = true
                    }
                }
            }

            G.Button {

                id: _stop

                anchors.right: _controll_button_container.right;
                anchors.verticalCenter: _controll_button_container.verticalCenter
                anchors.margins: G.Style.smallPadding
                type: G.Style.ButtonType.Danger
                iconName: G.Icons.icons["stop"]
                text: "Stop"

                onClicked: {
                    console.info('stopping Run!')
                    d.stop();
                }
            }
        }

        Item {
            id: _button_container

            height: G.Style.largeButtonHeight
            Layout.fillWidth: true;

            G.Button {

                id: _apply

                anchors.right: _button_container.right;
                anchors.verticalCenter: _button_container.verticalCenter
                anchors.margins: G.Style.smallPadding

                text: "Apply"

                onClicked: {
                    console.info('launching Run!')
                    d.run();
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
