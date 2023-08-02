import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import Qt.labs.platform  1.0 as P

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G

Item {

    id: _self;

    property alias parameters : _parameters_menu.parameters
    property var d;
    property string _current_file: "";

    FileDialog {
        id: _file_dialog;

        currentFile: _self._current_file;
        currentFolder: d.defaultReadPath();
        fileMode: FileDialog.OpenFile;

        modality: Qt.NonModal;
        nameFilters: ["Python source files (*.py)"]

        onAccepted: {
            d.read(decodeURIComponent(_file_dialog.selectedFfile));
            _self._current_file = _file_dialog.selectedFfile;
        }
    }

    FileDialog {
        id: _file_dialog_save

        title: "Save Python algorithm"

        currentFolder: d.defaultReadPath();
        fileMode: FileDialog.SaveFile

        modality: Qt.WindowModal;
        nameFilters: ["Python source files (*.py)"]

        onAccepted: {
            d.save(decodeURIComponent(_file_dialog_save.selectedFfile));
            _self._current_file = _file_dialog_save.selectedFfile;
            if (_message_dialog.visible) {
                _message_dialog.close()
            }
        }
    }

    G.Dialog {
        id: _message_dialog;

        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)

        width: Math.round(window.width / 3 * 2)
        height: 200

        parent: Overlay.overlay
        focus: true
        modal: true

        title: "Existing file"

        Label {
            anchors.fill: parent;

            text: "The file already exists, do you want to replace it or save as new ?";
            font: G.Style.fonts.value;
        }

        footer: DialogButtonBox {
            visible: true

            G.Button {
                text: "Cancel"
                type: G.Style.ButtonType.Neutral
                flat: true;

                onClicked: {
                    _message_dialog.close();
                }
            }

            G.Button {
                text: "Replace"
                type: G.Style.ButtonType.Warning
                flat: true;

                onClicked: {
                    d.save(_file_dialog_save.file);
                    _message_dialog.close();
                }
            }

            G.Button {
                text: "Save as"
                type: G.Style.ButtonType.Base
                flat: false;

                onClicked: {
                    _self._current_file = "";
                    _file_dialog_save.open();
                }
            }
        }
    }

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
            d.editMode = currentIndex == 0;
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
                        leftPadding: G.Style.smallPadding
                    }

                    G.IconButton { id: _update_icon;
                        iconName: "refresh";
                        size: G.Style.iconLarge;
                        tooltip: "Update code";

                        onClicked: {
                            d.code.updateCode();
                        }
                    }

                }

                G.TextField {
                    id: _plugin_name

                    Layout.fillWidth: true
                    text: d.code.pluginName

                    onTextChanged: {
                        d.code.pluginName = _plugin_name.text
                    }
                }

                ScrollView {
                    Layout.fillWidth: true
                    implicitHeight: 60

                    G.TextArea {
                        id: _plugin_doc

                        wrapMode: Text.Wrap
                        text: d.code.pluginDocumentation

                        onTextChanged: {
                            d.code.pluginDocumentation = _plugin_doc.text
                        }
                    }
                }

                G.PythonPluginMenu {
                    type: "Input Form"
                    code: d.code
                }

                G.PythonPluginMenu {
                    type: "Output Form"
                    code: d.code
                }

                G.PythonPluginMenu {
                    type: "Parameter"
                    code: d.code
                }

                Item {
                    id: _button_container

                    height: G.Style.largeButtonHeight
                    Layout.fillWidth: true

                    G.Button {
                        anchors.right: _load_button.left;
                        anchors.verticalCenter: _button_container.verticalCenter
                        anchors.margins: G.Style.smallPadding;

                        text: "Save";

                        type: G.Style.ButtonType.Base
                        iconName: "content-save"
                        empty: true

                        onClicked: {
                            if(_self._current_file == "") {
                                _file_dialog_save.open()

                            } else {
                                _message_dialog.open();
                            }
                        }
                    }

                    G.Button {
                        id: _load_button

                        anchors.right: _button_container.right;
                        anchors.verticalCenter: _button_container.verticalCenter
                        anchors.margins: G.Style.smallPadding;

                        text: "Load";

                        type: G.Style.ButtonType.Base
                        iconName: "folder-open"
                        empty: true

                        onClicked: {
                            _file_dialog.open();
                        }
                    }
                }
            }
        }

        Control {
            anchors.fill: parent;
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
}
