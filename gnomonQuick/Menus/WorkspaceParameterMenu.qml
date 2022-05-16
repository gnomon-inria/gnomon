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

    QtObject {
        id: _internal;

        property var expanded: [];
    }


    ColumnLayout {

        anchors.fill: parent
        anchors.margins: 10

        X.ComboBox {
            id: _algos
            Layout.fillWidth: true;
            Layout.preferredHeight: 28;
            model: d ? d.algorithms : null;
            currentIndex: d ? d.currentIndex : undefined;
            onCurrentIndexChanged: {
                _auto_apply.checked = false

                if (d && d.algorithms) {
                    d.currentIndex = _algos.currentIndex;
                    d.algoName = d.algorithms[d.currentIndex];
                }
            }
        }

        Component {
            id: _section_heading;

            Rectangle {
                id: _section_rectangle;

                required property string section;

                width: _l.width;
                height: section ? 33 : 0 
                z: -1;

                color: X.Style.backgroundColor;
                radius: 3;

                clip: true;

                Behavior on height {
                    NumberAnimation { duration: 200 }
                }

                MouseArea {
                    anchors.fill: parent;
                    onClicked: _l.toggleCollapse(parent.section);
                }

                X.Icon {
                    id: _icon;

                    anchors.right: parent.right;
                    anchors.top: parent.top;
                    anchors.topMargin: 5;
                    anchors.rightMargin: 5;
                    size: 30;

                    icon: X.Icons.icons.keyboard_arrow_up;

                    rotation: _l.isSectionExpanded(parent.section) ? 0 : 180;

                    Behavior on rotation {
                        NumberAnimation { duration: 200 }
                    }
                }

                X.Label {
                    anchors.left: parent.left;
                    anchors.verticalCenter: parent.verticalCenter;
                    anchors.leftMargin: 5;

                    text: parent.section //sectionTitle(parent.section);
                    font.pixelSize: 12;
                    font.bold: true;
                }
            }
        }


        ListView {
            id: _l;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            clip: true;

            model: parameters

            spacing: 10;


            section.property: "group"
            section.criteria: ViewSection.FullString
            section.delegate: _section_heading;


            delegate: Loader {
                property var lparam: param;

                anchors.topMargin: 10;

                height: _l.isSectionExpanded(group) ? 70 : 0;
                width: _l.width;

                clip: true;
                z: 1;   

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
                Behavior on height {
                    NumberAnimation { duration: 200 }
                }
            }

            ScrollIndicator.vertical: ScrollIndicator {
                visible: _l.contentHeight > _l.height;
            }

            function isSectionExpanded(group) {

                if(group) {
                    return _internal.expanded.includes(group)
                }

                return true;
            }

            function toggleCollapse(group) {
                if(_internal.expanded.includes(group)) _l.collapseSection(group)
                else _l.expandSection(group)
            }

            function collapseSection(group) {
                _internal.expanded = _internal.expanded.filter((item) => item !== group)
            }

            function expandSection(group) {
                _internal.expanded = _internal.expanded.concat([group])
            }

            function sectionTitle(s) {
                let title = s.replace('_', ' ');
                return title.charAt(0).toUpperCase() + title.slice(1);
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
                contentItem: null;

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


    Component.onCompleted: {
        _internal.expanded = []
    }


    background: Rectangle { color: "#00000000"; }
}
