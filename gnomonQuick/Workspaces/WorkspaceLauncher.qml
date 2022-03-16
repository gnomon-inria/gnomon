import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import Qt.labs.platform  1.0 as P
import Qt.labs.settings 1.0

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Models     1.0 as X
import xQuick.Style      1.0 as X

import gnomonQuick.Workspaces 1.0 as G

G.Workspace {

    id: _workspace;

    workspace_title: "Launcher";
    fill: () => {}

    property string current_file: "";
    property string _opened_files: "";

    P.FileDialog {
        id: _file_dialog;

        currentFile: _workspace.current_file;
        folder: P.StandardPaths.writableLocation(P.StandardPaths.HomeLocation);
        modality: Qt.NonModal;

        onAccepted: {
            console.log('Loading an existing project');
            load_session(_file_dialog.file);
            add_to_history(_file_dialog.file)  
        }
    }

    Pane {
        anchors.fill: parent;

        RowLayout {

            anchors.centerIn: parent;
            anchors.fill: parent
            spacing: 10;

            Rectangle {

                width: parent.width/3;
                height: parent.height/3;

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left;
                anchors.margins: 20;
                color: X.Style.backgroundColor;

                Rectangle {
                    id: _header_1
                    width: parent.width
                    height:42
                    color: Qt.darker(X.Style.backgroundColor, 1.2)
                    X.Label {
                        anchors.centerIn: parent
                        text: "Recentely opened projects"
                        font {
                            weight: Font.Bold
                            pointSize: 14;
                        }
                    }
                }
                ListView {
                    id: _recent_projects_lview

                    anchors.top: _header_1.bottom;
                    anchors.bottom: parent.bottom;
                    width: parent.width
                    clip: true;
                    focus: true;
                    currentIndex: -1

                    verticalLayoutDirection: ListView.BottomToTop

                    model: _recent_projects
                    delegate: ItemDelegate {
                        width: _recent_projects_lview.width
                        height: 42;
                        highlighted: _recent_projects_lview.currentIndex == index

                        onClicked: {
                            _recent_projects_lview.currentIndex = index
                            load_session(model.source)
                        }

                        text: model.name;
                        font.pointSize: 14;
                        background: Rectangle {
                            opacity: enabled ? 0.8 : 0.1
                            color: (down || highlighted || hovered) ? Qt.lighter(X.Style.backgroundColor, 1.2) : Qt.darker(X.Style.backgroundColor, 1.2)

                            Rectangle {
                                width: parent.width
                                height: 1
                                color: X.Style.borderColor;
                                anchors.bottom: parent.bottom
                            }
                        }

                    }
                }
            }

            ColumnLayout {

                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight;
                Layout.rightMargin: 20;

                width: parent.width/2;
                spacing: 10

                Rectangle {
                    width: 200;
                    height: 200;
                    radius: 10;
                    color: "transparent";
                    border.width: 3;
                    border.color: X.Style.baseColor;

                    X.Icon {
                        icon: X.Icons.icons.folder;
                        size: 112;
                        color: _area_1.containsMouse ? Qt.lighter(X.Style.baseColor) : X.Style.baseColor;
                        anchors.centerIn: parent;
                    }

                    MouseArea {
                        id: _area_1;
                        anchors.fill: parent;
                        hoverEnabled: true;

                        onClicked: _file_dialog.open();
                    }

                    X.ToolTip {
                        visible: _area_1.containsMouse
                        text: "Load an existing project";
                    }
                }

                Rectangle {
                    width: 200;
                    height: 200;
                    radius: 10;
                    color: "transparent";
                    border.width: 3;
                    border.color: X.Style.baseColor;

                    X.Icon {
                        icon: X.Icons.icons.create_new_folder;
                        size: 112;
                        color: _area_2.containsMouse ? Qt.lighter(X.Style.baseColor) : X.Style.baseColor;
                        anchors.centerIn: parent;
                    }

                    MouseArea {
                        id: _area_2;
                        anchors.fill: parent;
                        hoverEnabled: true;

                        // onClicked: _workspace.state = 'CONF'
                        // onClicked: _create_project_dialog.open();
                        onClicked: add_workspace("qrc:/qml/gnomonQuick/Workspaces/WorkspaceBrowsing.qml");
                    }
                    X.ToolTip {
                        visible: _area_2.containsMouse
                        text: "Create a new project";
                    }
                }
            }
        }
    }

    X.Dialog {

        id: _create_project_dialog;

        x: (parent.width - width)/2
        y: (parent.height - height)/2
        parent: Overlay.overlay

        focus: true
        modal: true
        title: "Create a project"
        standardButtons: Dialog.Ok | Dialog.Cancel

        width: window.width * 3/4;
        height: window.height * 3/4;

        Flickable {

            id: _contents;

            anchors.fill: parent;
            anchors.margins: 20;

            contentHeight: _layout.height;

            clip: true;

            ColumnLayout {

                id: _layout;

                spacing: 10;

                anchors.top: parent.top;
                anchors.left: parent.left;
                anchors.right: parent.right;

                // width: _layout.width/2 - _layout.spacing/2;
                // height: parent.height;

                GridLayout {

                    id: _form;

                    columns: 2;
                    rows: 4;

                    width: parent.width;

                    X.LabelCaption {
                        text: 'Name';
                    }
                    X.TextField {
                        id: new_p_name;
                        Layout.fillWidth: true;
                    }
                    X.LabelCaption {
                        text: 'Icon';
                    }
                    X.ComboBox {
                        Layout.fillWidth: true;
                    }
                    X.LabelCaption {
                        text: 'Tags';
                    }
                    X.TextField {
                        id: new_p_tags;
                        Layout.fillWidth: true;
                    }
                    X.LabelCaption {
                        text: 'Context';
                    }
                    X.ComboBox {
                        id: new_p_context;
                        Layout.fillWidth: true;
                    }
                }

                Item { Layout.fillHeight: true; }

                TextField {

                    id: _plugins_field;

                    height: 32;

                    Layout.fillWidth: true;

                    onTextChanged: _plugins_model.refilter();

                    X.Icon {
                        icon: X.Icons.icons.search;
                        color: X.Style.foregroundColor;

                        anchors.right: parent.right;
                        anchors.rightMargin: 10;
                        anchors.verticalCenter: parent.verticalCenter;
                    }
                }

                GridView {

                    id: _plugins_view;

                    Layout.fillWidth: true;

                    height: _plugins_view,contentHeight;

                    cellWidth: parent.width / 4;
                    cellHeight: cellWidth;

                    clip: true;

                    model: X.FilterProxyModel
                    {
                        id: _plugins_model

                        model: ListModel {
                            id: _plugins_list;

                            ListElement { name: "Plugin 1"; }
                            ListElement { name: "Plugin 2"; }
                            ListElement { name: "Plugin 3"; }
                            ListElement { name: "Plugin 4"; }
                            ListElement { name: "Plugin 5"; }
                            ListElement { name: "Plugin 6"; }
                            ListElement { name: "Plugin 7"; }
                            ListElement { name: "Plugin 8"; }
                            ListElement { name: "Plugin 9"; }
                            ListElement { name: "Plugin 10"; }
                            ListElement { name: "Plugin 11"; }
                            ListElement { name: "Plugin 12"; }
                            ListElement { name: "Plugin 13"; }
                            ListElement { name: "Plugin 14"; }
                        }

                        delegate: X.Card {

                            topInset: 5;
                            leftInset: 5;
                            rightInset: 5;
                            bottomInset: 5;

                            width: _plugins_view.cellWidth;
                            height: _plugins_view.cellHeight;

                            X.Icon {
                                id: _plugins_icon;
                                // icon: Object.values(Icons.icons)[index];
                                icon: X.Icons.icons.plumbing; // Object.values(Icons.icons)[index];
                                size: _plugins_view.cellWidth / 3;
                                color: X.Style.textColor;
                                anchors.centerIn: parent;
                            }
                            X.LabelHint1 {
                                text: _plugins_list.get(model.index).name;
                                anchors.top: _plugins_icon.bottom;
                                anchors.topMargin: 10;
                                anchors.horizontalCenter: parent.horizontalCenter;
                            }
                            X.Icon {
                                id: _plugins_icon_checked;
                                icon: X.Icons.icons.radio_button_unchecked;
                                size: 20;
                                color: X.Style.accentColor;
                                anchors.top: parent.top;
                                anchors.topMargin: 10;
                                anchors.right: parent.right;
                                anchors.rightMargin: 10;
                            }

                            MouseArea {
                                anchors.fill: parent;
                                onClicked: _plugins_icon_checked.icon = (_plugins_icon_checked.icon == X.Icons.icons.radio_button_unchecked) ? X.Icons.icons.check_circle : X.Icons.icons.radio_button_unchecked;
                            }
                        }

                        filterAccepts: function(item) {

                            return _plugins_list.get(item.index).name.includes(_plugins_field.text);
                        }
                    }
                }
            }

            ScrollBar.vertical: ScrollBar { visible: _contents.contentHeight > _contents.height; }

        }

        onAccepted: {
            window.create_project(new_p_name.text, new_p_context.text, new_p_tags.text);
        }
    }

    ListModel {
        id: _recent_projects;
    }

    Settings {
        id: stt
        property alias opened_projects: _workspace._opened_files;
    }

    Component.onCompleted: {
        _recent_projects.clear()
        let files = JSON.parse(stt.opened_projects)
        for(let i=0; i<files.length; i++){
            _recent_projects.append(files[i])
        }
    }

    function add_to_history(_file){

        let file_path = _file.toString()
        let file_name = file_path.slice(file_path.lastIndexOf("/")+1)
        let file_source = _file.toString();

        if(_recent_projects.count > 4) _recent_projects.remove(0)
        _recent_projects.append({name: file_name, 
                                source : file_source
                                })
        let _projects = []
        for(let i=0; i<_recent_projects.count; i++){
            _projects.push(_recent_projects.get(i))
        }
        _workspace._opened_files = JSON.stringify(_projects)
    }    
}
