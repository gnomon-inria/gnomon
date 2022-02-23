import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G

Control {

    id: _menu;

    property var parameters
    property var d

    property alias algo_combobox: _algos;

    ColumnLayout {

        anchors.fill: parent
        anchors.margins: 10

        X.ComboBox {
            id: _algos
            Layout.fillWidth: true;
            Layout.preferredHeight: 28;
            model: d.algorithms
            currentIndex: d.currentIndex
            onCurrentIndexChanged: {
                _auto_apply.checked = false
                d.currentIndex = currentIndex
                d.algoName = d.algorithms[currentIndex]
            }
        }

        ListView {
            id: _l;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            clip: true;

            model: parameters

            spacing: 10;

            delegate: Loader {
                property var lparam: param;
                height: 70;
                width: _l.width;
                sourceComponent: component

                Connections {
                    target: param
                    function onValueChanged() {
                        if (_auto_apply.checked) {
                            console.info('launching Run!')
                            d.run();
                        }
                    }
                }
            }

            ScrollIndicator.vertical: ScrollIndicator {
                visible: _l.contentHeight > _l.height;
            }
        }

        X.ButtonRaw {
            text: "Apply";

            onClicked: {
                //_progress.open();
                //_progress.start();
                console.info('launching Run!')
                d.run();
            }

            X.CheckBox{ id: _auto_apply

                text: ""
                contentItem: { }

                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 5

                height: parent.height - 10;


                MouseArea { id: _auto_apply_mouse_area;
                    anchors.fill: parent;
                    hoverEnabled: true;

                    propagateComposedEvents: true

                    onClicked: mouse.accepted = false;
                    onPressed: mouse.accepted = false;
                    onReleased: mouse.accepted = false;
                    onDoubleClicked: mouse.accepted = false;
                    onPositionChanged: mouse.accepted = false;
                    onPressAndHold: mouse.accepted = false;
                }

                ToolTip.visible: _auto_apply_mouse_area.containsMouse;
                ToolTip.text: "Auto-apply";

                onClicked: {
                    if (_auto_apply.checked) {
                        console.info('launching Run!')
                        d.run();
                    }
                }
            }

            Layout.fillWidth: true;
        }
    }

    background: Rectangle { color: "#00000000"; }
}
