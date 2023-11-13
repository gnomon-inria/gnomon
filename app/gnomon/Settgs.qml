import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QtCore

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomon.Utils

G.Dialog {

    id: settingsDialog

    x: Math.round((parent.width - width) / 2)
    y: Math.round(parent.height / 6)

    width: G.Style.mediumDialogWidth;
    height: G.Style.largeDialogHeight;

    modal: true
    focus: true
    title: "Settings"

    standardButtons: Dialog.Ok | Dialog.Cancel

    leftMargin: 0;
    rightMargin: 0;

    leftPadding: 0;
    rightPadding: 0;
    bottomPadding: 0;
    topPadding: 0;

    spacing: 0;

    ColumnLayout {

        Layout.fillWidth: true;
        Layout.fillHeight: true;

        spacing: 0

        StackLayout {
            id: _stack

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            G.Page {

                Layout.fillWidth: true;
                Layout.fillHeight: true;

                padding: 20

                ButtonGroup { id: _flavor_group; }

                Flickable {

                    id: _flicker;

                    anchors.fill: parent

                    clip: true;
                    contentHeight: _contents.height;

                    Column {
                        id: _contents;

                        anchors.fill: parent;

                        spacing: 10;

                      Label {
                        width: parent.width;
                        color: G.Style.colors.textColorBase
                        font: G.Style.fonts.subHeader
                        text: "Variant";
                      }

                      SwitchDelegate {
                          width: parent.width;
                          text: G.Style.mode == G.Style.Mode.Dark ? "Dark" : "Light";
                          palette.text: G.Style.colors.textColorBase
                          onToggled: {
                              G.Style.mode = checked ? G.Style.Mode.Light :  G.Style.Mode.Dark;
                          }
                          checked: G.Style.mode == G.Style.Mode.Light;
                        }

                      Label {
                        width: parent.width
                        text: qsTr("Data path")
                        color: G.Style.colors.textColorBase
                        font: G.Style.fonts.subHeader
                      }

                      TextField {
                        id: data_path_edit
                        width: parent.width
                        text: GUtils.dataPath
                        font: G.Style.fonts.value
                      }
                  }

                  ScrollIndicator.vertical: ScrollIndicator { visible: _flicker.contentHeight > _flicker.height; }
                }

            }

            G.Page {

                Layout.fillWidth: true;
                Layout.fillHeight: true;

                padding: 20

                Column {

                    anchors.centerIn: parent

                    spacing: 16

                    Grid {

                        columns: 3
                        rows: 2
                        spacing: 16

                        Label {
                            text: "MEM"
                        }

                        Rectangle {
                            id: _memory_total

                            property int value: 0

                            clip: true
                            width: 300
                            height: 16
                            radius: 8

                            border.width: 1
                            border.color: "#302B27"

                            color: "#1B264F"

                            MouseArea {

                                anchors.fill: parent
                                hoverEnabled: true

                                ToolTip.timeout: 5000
                                ToolTip.visible: containsMouse
                                ToolTip.text: _memory_total.value > 1000
                                        ? _memory_total.value / 1000 + "GB total"
                                        : _memory_total.value        + "MB total"
                            }

                            Rectangle {
                                id: _memory_used

                                property int value: 0

                                x: 0
                                y: 0

                                width: (_memory_used.value - _memory_this.value) / _memory_total.value * _memory_total.width
                                height: parent.height
                                color: "#274690"
                                radius: parent.radius - 2

                                Rectangle {

                                    x: parent.width - parent.radius
                                    y: parent.y
                                    width: parent.radius
                                    height: parent.height
                                    color: parent.color
                                }

                                MouseArea {

                                    anchors.fill: parent
                                    hoverEnabled: true

                                    ToolTip.timeout: 5000
                                    ToolTip.visible: containsMouse
                                    ToolTip.text: _memory_used.value > 1000
                                        ? _memory_used.value / 1000 + "GB used"
                                        : _memory_used.value        + "MB used"
                                }
                            }

                            Rectangle {
                                id: _memory_this

                                property int value: 0

                                x: _memory_used.width
                                y: 0

                                width: _memory_this.value / _memory_total.value * _memory_total.width
                                height: parent.height
                                color: "#576CA8"

                                MouseArea {

                                    anchors.fill: parent
                                    hoverEnabled: true

                                    ToolTip.timeout: 5000
                                    ToolTip.visible: containsMouse
                                    ToolTip.text: _memory_this.value > 1000
                                        ? _memory_this.value / 1000 + "GB self"
                                        : _memory_this.value        + "MB self"
                                }
                            }
                        }

                        Label {
                            text: (_memory_total.value - _memory_used.value) > 1000
                                ? (_memory_total.value - _memory_used.value) / 1000 + "GB remaining"
                                : (_memory_total.value - _memory_used.value)        + "MB remaining"
                        }

                        // CPU

                        Label {
                            text: "CPU"
                        }

                        Rectangle {
                            id: _cpu_total

                            property int value: 100
                            property int count: 0

                            clip: true
                            width: 300
                            height: 16
                            radius: 8

                            border.width: 1
                            border.color: "#302B27"

                            color: "#1B264F"

                            MouseArea {

                                anchors.fill: parent
                                hoverEnabled: true

                                ToolTip.timeout: 5000
                                ToolTip.visible: containsMouse
                                ToolTip.text: "Using " + _cpu_total.count + " cores"
                            }

                            Rectangle {
                                id: _cpu_used

                                property int value: 0

                                x: 0
                                y: 0

                                width: (_cpu_used.value - _cpu_this.value) / _cpu_total.value * _cpu_total.width
                                height: parent.height
                                color: "#274690"
                                radius: parent.radius - 2

                                Rectangle {

                                    x: parent.width - parent.radius
                                    y: parent.y
                                    width: parent.radius
                                    height: parent.height
                                    color: parent.color
                                }

                                MouseArea {

                                    anchors.fill: parent
                                    hoverEnabled: true

                                    ToolTip.timeout: 5000
                                    ToolTip.visible: containsMouse
                                    ToolTip.text: _cpu_used.value + "% used"
                                }
                            }

                            Rectangle {
                                id: _cpu_this

                                property int value: 0

                                x: _cpu_used.width
                                y: 0

                                width: _cpu_this.value / _cpu_total.value * _cpu_total.width
                                height: parent.height
                                color: "#576CA8"

                                MouseArea {

                                    anchors.fill: parent
                                    hoverEnabled: true

                                    ToolTip.timeout: 2500
                                    ToolTip.visible: containsMouse
                                    ToolTip.text: _cpu_this.value + "% self"
                                }
                            }
                        }

                        Label {
                            text: _cpu_total.value - _cpu_used.value + "% remaining"
                        }
                    }

                Label {

                    id: _info;

                    property string host_name: ""
                    property string system_name: ""

                    text: (host_name !== "" && system_name !== "") ? "On " + host_name + ", running " + system_name : ""
                }

                }

              /*
                X.SystemFetcher {
                    id: _system_fetcher;
                }

                Timer {
                    interval: 2500
                    running: parent.visible
                    repeat: true
                    onTriggered: {
                        _system_fetcher.fetch();

                        _memory_total.value = _system_fetcher.total_memory
                        _memory_used.value = (_system_fetcher.used_memory - _system_fetcher.this_memory)
                        _memory_this.value =  _system_fetcher.this_memory

                        _cpu_total.count = _system_fetcher.total_cpu_count
                        _cpu_used.value = _system_fetcher.used_cpu
                        _cpu_this.value = _system_fetcher.this_cpu

                        _info.host_name = _system_fetcher.host_name
                        _info.system_name = _system_fetcher.system_name
                    }
                }
*/
            }
        }

        RowLayout {

            Layout.fillWidth: true

            spacing: 0

            G.TabButton { checkable: true; checked: true; text: "Interface"; Layout.fillWidth: true; onClicked: _stack.currentIndex = 0 }
            G.TabButton { checkable: true; checked: false; text: "Monitoring"; Layout.fillWidth: true; onClicked: _stack.currentIndex = 1 }
        }
    }

    Settings {
        id: settings
        property int mode: G.Style.Mode.Dark
    }

    function setDataPath(new_path) {
        if(GUtils.isValidPath(new_path)) {
            GUtils.dataPath = new_path
        }
    }

    Component.onCompleted: {
        let data_path = settings.value("data_path", "")
        GUtils.initDataPath(data_path)
    }

    onAboutToShow: {
        data_path_edit.text = GUtils.dataPath
    }

    onAccepted: {
        setDataPath(data_path_edit.text)
        settings.setValue("data_path", GUtils.dataPath)
        settings.mode = G.Style.mode
        settingsDialog.close()
    }

    onRejected: {
        G.Style.mode = settings.mode
        settingsDialog.close()
    }

    contentItem: ColumnLayout {
        id: settingsColumn
        spacing: 20
    }
}
