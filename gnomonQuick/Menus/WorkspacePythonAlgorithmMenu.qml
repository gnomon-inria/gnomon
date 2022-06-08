import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import Qt.labs.platform  1.0 as P

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G
import gnomonQuick.Icons      1.0 as G

Item {

    id: _self;

    property var parameters
    property var d;
    property string _current_file: "";

    P.FileDialog {
        id: _file_dialog;

        currentFile: _self._current_file;
        folder: d.defaultReadPath();
        fileMode: P.FileDialog.OpenFile;

        modality: Qt.NonModal;
        nameFilters: ["Python source files (*.py)"]

        onAccepted: {
            d.read(decodeURIComponent(_file_dialog.file));
            _self._current_file = _file_dialog.file;
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
            d.save(decodeURIComponent(_file_dialog_save.file));
            _self._current_file = _file_dialog_save.file;
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

        TabButton {
            text: qsTr("Edit")
            contentItem: Label {
                anchors.fill: parent

                font: G.Style.fonts.header;
                color: G.Style.colors.textColorBase;
                text: parent.text;
                verticalAlignment: Text.AlignVCenter;
                horizontalAlignment: Text.AlignHCenter;
          }
        }
        TabButton {
            text: qsTr("Run")
            contentItem: Label {
                anchors.fill: parent

                font: G.Style.fonts.header;
                color: G.Style.colors.textColorBase;
                text: parent.text;
                verticalAlignment: Text.AlignVCenter;
                horizontalAlignment: Text.AlignHCenter;
          }
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

                    G.Icon { id: _update_icon;
                        icon: G.Icons.icons["refresh"];
                        size: 32;
                        color: _update_mouse_area.containsMouse ? G.Style.colors.hoveredBaseColor : G.Style.colors.neutralColor;

                        MouseArea { id: _update_mouse_area;
                            anchors.fill: parent;
                            hoverEnabled: true;

                            onClicked: {
                                d.code.updateCode();
                            }
                        }

                        ToolTip.visible: _update_mouse_area.containsMouse;
                        ToolTip.text: "Update code";
                    }
                }

                TextField {
                    id: _plugin_name

                    Layout.fillWidth: true
                    text: d.code.pluginName

                    onTextChanged: {
                        d.code.pluginName = _plugin_name.text
                    }
                }

                Flickable {
                    Layout.fillWidth: true
                    implicitHeight: 60

                    contentWidth: availableWidth

                    TextArea.flickable: TextArea {
                        id: _plugin_doc

                        wrapMode: Text.Wrap
                        text: d.code.pluginDocumentation

                        onTextChanged: {
                            d.code.pluginDocumentation = _plugin_doc.text
                        }
                    }

                    ScrollIndicator.vertical: ScrollIndicator { }
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

                RowLayout {
                    anchors.margins: G.Style.smallPadding;
                    spacing: G.Style.smallPadding

                    Item {
                        Layout.fillWidth: true
                    }

                    G.Button {
                        text: "Save";

                        type: G.Style.ButtonType.Base
                        iconName: G.Icons.icons["content-save"]
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
                        text: "Load";

                        type: G.Style.ButtonType.Base
                        iconName: G.Icons.icons["folder-open"]
                        empty: true

                        onClicked: {
                            _file_dialog.open();
                        }
                    }
                }
            }
        }

        G.WorkspaceParameterMenu {

            id: _parameters_menu;
            d: _self.d
            parameters: _self.parameters

            algo_combobox.model: [d.algorithm]
            algo_combobox.enabled: false
        }
    }
}
