import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.platform  as P

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G

Item {

    id: _self;

    property alias parameters : _parameters_menu.parameters
    property var d;

    TabBar {
        id: _bar;

        anchors.top: _self.top;
        anchors.left: _self.left;
        anchors.right: _self.right;

        currentIndex: d.editMode ? 0 : 1;

        background: Rectangle {
            anchors.fill: parent
            color: G.Style.colors.bgColor
        }

        G.TabButton {
            text: qsTr("Edit")
        }

        G.TabButton {
            text: qsTr("Run")
        }

        onCurrentIndexChanged: {
            if(d.code.fileName.endsWith(".py")) {
                d.editMode = currentIndex == 0;
            } else {
                _bar.setCurrentIndex(0);
                _file_dialog_save.open();
                _non_py_toast.open()
            }
        }
    }

    StackLayout {
        id: _container;

        anchors.top: _bar.bottom;
        anchors.bottom: _self.bottom;
        anchors.right: _self.right;
        anchors.left: _self.left;
        anchors.topMargin: 15;

        currentIndex: _bar.currentIndex;

        Control {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent;
                anchors.margins: 10;

                RowLayout {

                    Layout.fillWidth: true

                    Label {
                        Layout.fillWidth: true;
                        text: "Python Plugin Code";
                        color: G.Style.colors.textColorNeutral;
                        font: G.Style.fonts.header;
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        height: G.Style.iconLarge
                        padding: G.Style.smallPadding
                    }

                    G.IconButton {
                        id: _update_icon;
                        iconName: "refresh";
                        size: G.Style.iconLarge;
                        tooltip: "Update code";

                        visible: !d.readOnly

                        onClicked: {
                            d.code.updateCode();
                        }
                    }

                }

                G.TextField {
                    id: _plugin_name

                    Layout.fillWidth: true
                    text: d.code.pluginName
                    font: G.Style.fonts.formLabel;

                    visible: !d.readOnly

                    onTextChanged: {
                        d.code.pluginName = _plugin_name.text
                    }
                }

                Label {
                    Layout.fillWidth: true
                    height: _plugin_name.height
                    padding: G.Style.smallPadding
                    bottomPadding: G.Style.tinyPadding+2

                    text: d.code.pluginName
                    visible: d.readOnly

                    color: G.Style.colors.fgColor;
                    font: G.Style.fonts.formLabel;
                }

                ScrollView {
                    Layout.fillWidth: true
                    implicitHeight: 60

                    G.TextArea {
                        id: _plugin_doc

                        wrapMode: Text.Wrap
                        text: d.code.pluginDocumentation

                        visible: !d.readOnly

                        onTextChanged: {
                            d.code.pluginDocumentation = _plugin_doc.text
                        }
                    }

                    Label {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.margins: G.Style.smallPadding

                        text: d.code.pluginDocumentation
                        wrapMode: Text.Wrap
                        visible: d.readOnly

                        color: G.Style.colors.fgColor;
                        font: G.Style.fonts.value;
                    }
                }

                G.PythonPluginMenu {
                    type: "Input Form"
                    code: d.code
                    readOnly: d.readOnly
                }

                G.PythonPluginMenu {
                    type: "Output Form"
                    code: d.code
                    readOnly: d.readOnly
                }

                G.PythonPluginMenu {
                    type: "Parameter"
                    code: d.code
                    readOnly: d.readOnly
                }
            }
        }

        Control {
            Layout.fillWidth: true
            Layout.fillHeight: true
            anchors.margins: G.Style.smallPadding;

            G.ComboBoxWithLabel {
                id: _combo_box

                label: "Algorithm:"
                model: d ? [d.algorithm] : null;

                anchors.left: parent.left
                anchors.right: parent.right
                enabled: false
            }

            G.WorkspaceParameterMenu {
                id: _parameters_menu;

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: _combo_box.bottom
                anchors.bottom: parent.bottom

                property alias model: _parameters_menu.algo_combobox.model

                d: _self.d
                algo_combobox.visible: false
            }
        }
    }

    P.FileDialog {
        id: _file_dialog_save

        title: "Save Python algorithm"

        folder: d.defaultReadPath();
        fileMode: P.FileDialog.SaveFile

        modality: Qt.WindowModal;
        nameFilters: ["Python source files (*.py)"]

        onAccepted: {
            let file_path = decodeURIComponent(_file_dialog_save.file);
            let file_name = file_path.split('/').pop()
            d.code.fileName = file_name;
            d.save(file_path);
            _self._current_file = _file_dialog_save.file;
        }
    }

    G.Toast {
        id: _non_py_toast

        parent: Overlay.overlay
        header: "Not a saved file"
        message: "The file you opened is not saved, \nplease save it as python file before running."

        type: G.Style.ButtonType.Warning
    }
}
