import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.platform
import Qt.labs.folderlistmodel
import Qt.labs.settings

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control
{
    id: _self;

    property alias folder: _folder_model.folder;
    property alias extensionFilters: _filter_combobox.model;
    property string selectedFolder: "";
    property var selectedFiles: [];
    property bool shiftPressed: false;

    signal folderClicked(string folderUrl)
    signal fileClicked(string fileUrl)
    signal folderDoubleClicked(string folderUrl)
    signal fileDoubleClicked(string fileUrl)
    signal fileRightClicked(string fileUrl)

    hoverEnabled: true;

    background: Rectangle {
        color: G.Style.colors.gutterColor;
    }

    Settings {
        id: _settings;
        property string last_path;
        property string finderMode: "grid";

    }

    QtObject {
        id: _internal;

        property var history: [_self.folder];
        property int history_index: 0;

        property var icons: [
            {
                name: "left",
                iconName: "arrow-left",
                isActive: _internal.history_index > 0,
            },
            {
                name: "right",
                iconName: "arrow-right",
                isActive: _internal.history_index < _internal.history.length - 1,
            },
            {
                name: "up",
                iconName: "arrow-up",
                isActive: true,
            },
            {
                name: "home",
                iconName: "home",
                isActive: true,
            },
        ]

        property var extensions: [];

    }

    FolderListModel {
        id: _folder_model
        nameFilters: _internal.extensions;
    }


    Control {
        id: _tool_bar;

        anchors.right: _self.right;
        anchors.left: _self.left;

        height: 33;

        hoverEnabled: true;

        background: Rectangle {
            color: G.Style.colors.gutterColor;
        }

        Rectangle {
            anchors.bottom: _tool_bar.bottom;
            anchors.left: _tool_bar.left;
            anchors.right: _tool_bar.right;

            color: G.Style.colors.bgColor;

            implicitWidth: 1;
            implicitHeight: 1;
        }

        RowLayout {

            anchors.fill: _tool_bar;
            anchors.centerIn: _tool_bar;

            Row {
                id: _tool_bar_icons;
                Layout.fillHeight: true;
                padding: 3;

                Repeater {
                    model: _internal.icons;
                    Control {
                        id: _icon_rectangle;
                        required property var model;

                        height: 24;
                        width: 24;

                        background: Rectangle {
                            color: _icon_rectangle.hovered && model.modelData.isActive ? G.Style.colors.baseColor : G.Style.colors.gutterColor;
                            radius: 9;
                        }

                        hoverEnabled: true;


                        MouseArea {
                            id: _icon_mouse_area

                            anchors.fill: _icon_rectangle;

                            onClicked: {
                                if(model.modelData.isActive) _self.toolBarButtonHandler(model.modelData.name)
                            }

                        }

                        G.Icon {
                            id: _icon;

                            anchors.centerIn: _icon_rectangle;

                            icon: model.modelData.iconName;
                            color: G.Style.colors.fgColor;
                            opacity: model.modelData.isActive ? 1 : 0.4;
                            size: _icon_rectangle.width * 0.8;

                        }
                    }
                }

            }


            Rectangle {
                Layout.fillHeight: true;

                color: G.Style.colors.bgColor;

                implicitWidth: 1;
                implicitHeight: 1;
            }


            G.TextField {
                id: _text_field;

                Layout.fillWidth: true;
                Layout.fillHeight: true;

                font.pointSize: 10;

                text: _folder_model.folder;
                color: G.Style.colors.textColorNeutral

            }

            Rectangle {
                Layout.fillHeight: true;

                color: G.Style.colors.bgColor;

                implicitWidth: 1;
                implicitHeight: 1;
            }

            Row {
                id: _tool_bar_icons_r;
                Layout.fillHeight: true;
                padding: 3;

                Repeater {
                    model: ListModel {
                        ListElement {
                            name: "grid"
                            iconName: "apps"
                        }
                        ListElement {
                            name: "list"
                            iconName: "view-list"
                        }
                    }
                    Control {
                        id: _mode_rectangle;
                        required property string name;
                        required property string iconName;
                        height: 24;
                        width: 24;

                        background: Rectangle {
                            color: _settings.finderMode === name ? G.Style.colors.hoveredBaseColor : G.Style.colors.gutterColor;
                            radius: 9;
                        }

                        hoverEnabled: true;


                        MouseArea {
                            id: _mode_mouse_area

                            anchors.fill: _mode_rectangle;

                            onClicked: {
                                if(name !== _settings.finderMode) _self.toolBarButtonHandler(name)
                            }

                        }

                        G.Icon {
                            id: _icon;

                            anchors.centerIn: _mode_rectangle;

                            icon: iconName;
                            color: G.Style.colors.embossColor;
                            size: _mode_rectangle.width * 0.6;

                        }

                    }
                }
            }
        }
    }


    Loader {
        id: _loader

        anchors.top: _tool_bar.bottom;
        anchors.bottom: _filter_combobox.top;
        anchors.right: _self.right;
        anchors.left: _self.left;
    }

    G.ComboBox {
        id: _filter_combobox;

        textRole: "text";
        valueRole: "regexp";

        height: 24;
        //anchors.top: _loader.bottom;
        anchors.bottom: _self.bottom;
        anchors.right: _self.right;
        anchors.left: _self.left;

        visible: _self.extensionFilters ? true : false;

        onActivated: {
            _internal.extensions = _filter_combobox.currentValue.split(",")
        }
    }

    Connections {
        target: _loader.item

        function onFileClicked(fileUrl) {
            _self.selectedFolder = ""
            if( _self.shiftPressed) {
                console.log("shift pressed ..... ")
                _self.selectedFiles.append(fileUrl)
            } else {
                _self.selectedFiles = [fileUrl]
            }
            _self.fileClicked(_self.selectedFiles)
        }

        function onFileDoubleClicked(fileUrl) {
            if( _self.shiftPressed) {
                console.log("shift pressed ..... ")
                _self.selectedFiles.append(fileUrl)
            } else {
                _self.selectedFiles = [fileUrl]
            }

            //TODO check duplicates!!!
            _self.fileDoubleClicked(_self.selectedFiles)
        }

        function onFileRightClicked(fileUrl) {
            _self.selectedFolder = ""
            _self.selectedFiles = [fileUrl]
            _self.fileRightClicked(fileUrl)
        }

        function onFolderClicked(folderUrl) {
            _self.selectedFolder = folderUrl
            _self.selectedFiles = []
            _self.folderClicked(folderUrl)
        }

        function onFolderDoubleClicked(folderUrl) {
            _self.openFolder(folderUrl)
        }

    }

    function openFolder(folderUrl) {
        _self.folderDoubleClicked(folderUrl)
        _self.pushHistory(folderUrl)
        _self.folder = folderUrl
    }

    function pushHistory(folderName) {
        _internal.history_index = _internal.history_index + 1
        _internal.history  = _internal.history.slice(0, _internal.history_index).concat([folderName])
    }

    function next() {
        _internal.history_index = _internal.history_index + 1
        _self.folder = _internal.history[_internal.history_index]
    }

    function previous() {
        _internal.history_index = _internal.history_index - 1
        _self.folder = _internal.history[_internal.history_index]
    }

    function toolBarButtonHandler(buttonName) {
        if(buttonName === "up") {
            _self.folder=_folder_model.parentFolder
            _self.pushHistory(_self.folder)
        }

        if(buttonName === "home") {
            _self.folder=StandardPaths.standardLocations(StandardPaths.HomeLocation)[0];
            _self.pushHistory(_self.folder)
        }

        if(buttonName === "left") {
            _self.previous()
        }


        if(buttonName === "right") {
            _self.next()
        }

        if(buttonName === "grid") {
            _settings.finderMode = buttonName
            _loader.setSource("FinderGridView.qml", {"foldermodel": _folder_model})
        }

        if(buttonName === "list") {
            _settings.finderMode = buttonName
            _loader.setSource("FinderListView.qml", {"foldermodel": _folder_model})
        }

    }

    Component.onCompleted: {
        // if(_settings.last_path) _self.folder = _settings.last_path;
        // else _self.folder = StandardPaths.standardLocations(StandardPaths.HomeLocation)[0];

        if(_settings.finderMode === "grid") _loader.setSource("FinderGridView.qml", {"foldermodel": _folder_model})
        if(_settings.finderMode === "list") _loader.setSource("FinderListView.qml", {"foldermodel": _folder_model})
    }


    Component.onDestruction: {
        _settings.last_path = _self.folder;
    }

}
