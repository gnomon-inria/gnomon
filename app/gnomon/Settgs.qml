import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QtCore

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomon.Visualization as GV
import gnomon.Utils

G.Dialog {

    id: settingsDialog

    x: Math.round((parent.width - width) / 2)
    y: Math.round(parent.height / 6)

    width: G.Style.mediumDialogWidth;
    height: G.Style.hugeDialogHeight;

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

                        Label {
                            width: parent.width
                            text: qsTr("Memory")
                            color: G.Style.colors.textColorBase
                            font: G.Style.fonts.subHeader
                        }
                        
                        Row {
                            spacing: 15

                            Rectangle {
                                id: _memory_total

                                property int value: 0

                                clip: true
                                width: 300
                                height: 16
                                radius: 8

                                border.width: 1
                                border.color: G.Style.colors.gutterColor

                                color: G.Style.colors.gutterColor

                                MouseArea {

                                    anchors.fill: parent
                                    hoverEnabled: true

                                    ToolTip.timeout: 5000
                                    ToolTip.visible: containsMouse
                                    ToolTip.text: _memory_total.value / 1000 + "GB total"                                              
                                }

                                Rectangle {
                                    id: _memory_used

                                    property int value: 0

                                    x: 0
                                    y: 0

                                    width: (_memory_used.value - _memory_this.value) / _memory_total.value * _memory_total.width
                                    height: parent.height
                                    color: G.Style.colors.baseColor
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
                                            ? _memory_used.value / 1024 + "GB used"
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
                                    color: _memory_this.value > GV.World.maxMemory
                                        ? G.Style.colors.dangerColor
                                        : G.Style.colors.warningColor

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
                                text: _memory_total.value  / 1000 + "GB in Total"
                            }
                        }
                        Row {
                            //anchors.fill: parent;
                            spacing: 15;

                            Label {
                                text: "gnomon: " + (_memory_this.value > 1000
                                    ? _memory_this.value / 1000 + "GB"
                                    : _memory_this.value        + "MB")
                                color: _memory_this.value > GV.World.maxMemory
                                    ? G.Style.colors.dangerColor
                                    : G.Style.colors.warningColor
                            }

                            Label {
                                text: "remaining: " + ((_memory_total.value - _memory_used.value) > 1000
                                    ? (_memory_total.value - _memory_used.value) / 1000 + "GB"
                                    : (_memory_total.value - _memory_used.value)        + "MB")
                                color: G.Style.colors.textColorNeutral
                            }
                        }
                        Label {
                            width: parent.width
                            text: qsTr("Maximum memory")
                            color: G.Style.colors.textColorBase
                            font: G.Style.fonts.subHeader
                        }

                        G.NumericSlider {
                            id: max_memory_edit
                            width: parent.width
                            value: GV.World.maxMemory
                            min: 1000
                            max: GV.World.systemStat()[0]
                            decimals: 0
                            stepSize: 100
                            label: qsTr("Memory Threshold (MB)")
                            doc: qsTr("Memory value above which gnomon tries to limit its size in memory")
                        }
                    }
                    
                    Timer {
                        interval: 2500
                        running: parent.visible
                        repeat: true
                        onTriggered: {
                            let system_stats = GV.World.systemStat()
                            
                            _memory_total.value = system_stats[0]
                            _memory_this.value = system_stats[2]
                            _memory_used.value = (system_stats[1] - system_stats[2])
                            //console.log(_memory_total.value, _memory_used.value, _memory_this.value)
                        }
                    }

                  ScrollIndicator.vertical: ScrollIndicator { visible: _flicker.contentHeight > _flicker.height; }
                }
            }
        }

        RowLayout {

            Layout.fillWidth: true

            spacing: 0

            G.TabButton { checkable: true; checked: true; text: "Interface"; Layout.fillWidth: true; onClicked: _stack.currentIndex = 0 }
            //G.TabButton { checkable: true; checked: false; text: "Monitoring"; Layout.fillWidth: true; onClicked: _stack.currentIndex = 1 }
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
        GV.World.maxMemory = settings.value("maxMemory", 3000)
    }

    onAboutToShow: {
        data_path_edit.text = GUtils.dataPath
    }

    onAccepted: {
        setDataPath(data_path_edit.text)
        GV.World.maxMemory = max_memory_edit.value
        settings.setValue("data_path", GUtils.dataPath)
        settings.mode = G.Style.mode
        settings.setValue("maxMemory", GV.World.maxMemory)
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
