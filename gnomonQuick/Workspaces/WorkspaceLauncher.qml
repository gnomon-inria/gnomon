import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import Qt.labs.platform  1.0 as P
import Qt.labs.settings 1.0

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Models     1.0 as X
import xQuick.Style      1.0 as X

import gnomonQuick.Controls as G
import gnomonQuick.Workspaces as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G


G.Workspace {

    id: _workspace;

    workspace_title: "Launcher";
    fill: () => {}

    property string current_file: "";

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

    G.Panel {
        anchors.fill: parent

        RowLayout {

            anchors.centerIn: parent;
            anchors.fill: parent
            spacing: 0

            Item {
                implicitWidth: parent.width/3
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: G.Style.mediumColumnSpacing
                    spacing: G.Style.mediumColumnSpacing;

                    Item {
                        id: _gnomon_header

                        Layout.fillWidth: true;
                        Layout.fillHeight: true;

                        Image {
                            id: _logo

                            anchors.left: _gnomon_header.left;
                            anchors.top: _gnomon_header.top;
                            anchors.margins: G.Style.mediumPadding

                            height: 0.75 * parent.height
                            fillMode: Image.PreserveAspectFit

                            source: "qrc:/assets/gnomon_logo.png"
                        }

                        Label {

                            id: _version_tag

                            anchors.top: _logo.bottom;
                            anchors.left: _logo.left;
                            anchors.right: _logo.right;
                            anchors.margins: G.Style.smallPadding

                            font: G.Style.fonts.subHeader
                            color: G.Style.colors.textColorBase;

                            text: "version 0.61.0"

                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        Label {

                            id: _title

                            anchors.left: _logo.right;
                            anchors.right: _gnomon_header.right;
                            anchors.verticalCenter: _logo.verticalCenter;
                            anchors.margins: G.Style.mediumPadding

                            font: G.Style.fonts.h1
                            color: G.Style.colors.textColorBase;

                            text: "Gnomon"

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignTop
                        }

                        Label {

                            id: _tagline

                            anchors.left: _title.left;
                            anchors.top: _title.bottom;

                            font: G.Style.fonts.h3
                            color: G.Style.colors.textColorBase;

                            text: "Deciphering morphogenesis"

                            wrapMode: Text.Wrap
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignBottom
                        }
                    }

                    Item {
                        id: _start_header

                        Layout.fillWidth: true;
                        height: G.Style.sizes.s7

                        Label {
                            id: _start_label

                            anchors.fill: _start_header
                            anchors.margins: G.Style.mediumPadding

                            height: G.Style.sizes.s7

                            font: G.Style.fonts.h2
                            color: G.Style.colors.baseColor;

                            text: "Getting started"

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Repeater {

                        model: ListModel {
                           ListElement {
                               header: "Get started with tutorials"
                               paragraph: "Start your Gnomon journey with some basic tutorials. Get familiar with the philosophy of the platform with some easy use cases."
                               link: "https://gnomon.gitlabpages.inria.fr/gnomon/getting_started.html"
                           }
                           ListElement {
                               header: "Learn more about Gnomon"
                               paragraph: "Visit our online documentation to get more information, and discover how to unleash the power of algorithms in Gnomon."
                               link: "https://gnomon.gitlabpages.inria.fr/gnomon"
                           }
                           ListElement {
                               header: "What's up?"
                               paragraph: "Visit our social media to keep in touch! Stay tuned to the latest advances in morphogenesis analysis and simulation."
                               link: "https://gnomon.gitlabpages.inria.fr/gnomon"
                           }
                        }

                        Rectangle {
                            Layout.fillWidth: true;
                            Layout.fillHeight: true;

                            color: G.Style.colors.fgColor;
                            radius: G.Style.cardRadius;

                            Label {
                                anchors.fill: parent
                                anchors.margins: G.Style.mediumPadding;

                                text: header
                                font: G.Style.fonts.cardTitle

                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignTop
                                color: G.Style.colors.textColorBase;
                            }

                            Label {
                                anchors.fill: parent
                                anchors.margins: G.Style.mediumPadding;

                                text: paragraph
                                font: G.Style.fonts.cardLabel

                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignBottom
                                wrapMode: Text.Wrap
                                color: G.Style.colors.textColorBase;
                            }
                            
                            MouseArea {
                                id: _start_mouse_area
                                anchors.fill: parent;
                                hoverEnabled: true;
                                onClicked: {
                                    Qt.openUrlExternally(link);
                                }
                            }
                        }
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: G.Style.mediumColumnSpacing
                    spacing: G.Style.mediumColumnSpacing;

                    Item {
                        id: _project_header

                        Layout.fillWidth: true;
                        height: G.Style.sizes.s7

                        Label {
                            id: _project_label

                            anchors.left: _project_header.left;
                            anchors.verticalCenter: _project_header.verticalCenter;
                            anchors.margins: G.Style.mediumPadding

                            height: G.Style.sizes.s7

                            font: G.Style.fonts.h2
                            color: G.Style.colors.baseColor;

                            text: "Recent projects"

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }

                        G.Button {
                            id: _new

                            anchors.right: _project_header.right;
                            anchors.verticalCenter: _project_header.verticalCenter;
                            anchors.margins: G.Style.mediumPadding

                            height: G.Style.sizes.s7

                            text: "New"
                            iconName: G.Icons.icons["plus"]

                            onClicked: {
                                switch_from_launcher()
                            }

                        }

                        G.Button {

                            anchors.right: _new.left
                            anchors.verticalCenter: _project_header.verticalCenter;
                            anchors.margins: G.Style.mediumPadding

                            height: G.Style.sizes.s7

                            text: "Load"
                            iconName: G.Icons.icons["play"]
                            empty: true

                            onClicked: {
                                _file_dialog.open()
                            }
                        }
                    }

                    Control {
                        id: _projects

                        Layout.fillWidth: true;
                        Layout.fillHeight: true;

                        background: Rectangle {
                            radius: G.Style.panelRadius;
                            color: G.Style.colors.gutterColor;
                        }

                        GridView {
                            id: _project_grid

                            model: window.recent_projects

                            cellWidth: _projects.width/2 - G.Style.smallPadding;
                            cellHeight: G.Style.largeDelegateHeight;
                            displayMarginBeginning: G.Style.smallPadding;
                            displayMarginEnd: G.Style.smallPadding;

                            anchors.fill: parent
                            anchors.margins: G.Style.smallPadding;

                            clip: true;
                            focus: true;

                            delegate: Rectangle {
                                height: _project_grid.cellHeight - G.Style.smallPadding
                                width: _project_grid.cellWidth - G.Style.smallPadding
                                radius: G.Style.cardRadius;

                                color: _project_mouse_area.containsMouse? G.Style.colors.fgColor : G.Style.colors.bgColor;
                                border.width: 2;
                                border.color: G.Style.colors.baseColor;

                                Rectangle {
                                    id: _thumbnail;

                                    anchors.left: parent.left;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    anchors.margins: G.Style.smallPadding;

                                    height: parent.height - 2*G.Style.smallPadding
                                    width: parent.height - 2*G.Style.smallPadding
                                    radius: G.Style.panelRadius
                                    color: G.Style.colors.lightBlue

                                    Image {
                                        anchors.fill: _thumbnail;
                                        fillMode: Image.PreserveAspectFit
                                        source: "image://thumbnails/project_" + index
                                    }

                                }

                                Label {
                                    anchors.left: _thumbnail.right
                                    anchors.right: parent.right
                                    anchors.top: _thumbnail.top;
                                    anchors.bottom: _thumbnail.bottom;
                                    anchors.margins: G.Style.smallPadding;

                                    text: name
                                    font: G.Style.fonts.cardText

                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignTop
                                    color: G.Style.colors.textColorBase;
                                }

                                Label {
                                    anchors.left: _thumbnail.right
                                    anchors.right: parent.right
                                    anchors.top: _thumbnail.top;
                                    anchors.bottom: _thumbnail.bottom;
                                    anchors.margins: G.Style.smallPadding;

                                    text: "Description: "+description
                                    font: G.Style.fonts.cardLabel

                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignBottom
                                    wrapMode: Text.Wrap
                                    color: G.Style.colors.textColorBase;
                                }

                                ToolTip.visible: _project_mouse_area.containsMouse;
                                ToolTip.text: source;

                                MouseArea {
                                    id: _project_mouse_area
                                    anchors.fill: parent;
                                    hoverEnabled: true;
                                    onClicked: load_session(source)
                                }
                            }

                            ScrollIndicator.vertical: ScrollIndicator {
                              id: _scroll_indicator;

                              width: 8;
                              visible: _project_grid.contentHeight > _project_grid.height;
                            }
                        }
                    }
                }
            }
        }

        /*    GridLayout {

                columns: 3;
                rows: 2;

                Layout.leftMargin: 40;

                Repeater {
                    model: window.recent_projects
                    Rectangle {
                        color: X.Style.baseColor;
                        height: 200;
                        width: 200;
                        radius: 10;


                        border.width: 2;
                        border.color: X.Style.accentColor;

                        X.Label {
                            anchors.fill: parent
                            anchors.margins: 10
                            text: "<b>" + name + "</b> <br> <br> <i>description: </i> <br>" + description
                            font {
                                pointSize: 12;
                            }

                            wrapMode: Text.Wrap
                            color: X.Style.foregroundColor;

                            ToolTip.visible: _project_mouse_area.containsMouse;
                            ToolTip.text: source;
                        }

                        MouseArea {
                            id: _project_mouse_area
                            anchors.fill: parent;
                            hoverEnabled: true;
                            onClicked: load_session(source)
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
                    border.width: 2;
                    border.color: X.Style.accentColor;

                    X.Icon {
                        icon: X.Icons.icons.play_arrow;
                        size: 120;
                        color: _area_1.containsMouse ? Qt.lighter(X.Style.accentColor) : X.Style.accentColor;
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
                    border.width: 2;
                    border.color: X.Style.accentColor;

                    X.Icon {
                        icon: X.Icons.icons.add
                        size: 120;
                        color: _area_2.containsMouse ? Qt.lighter(X.Style.accentColor) : X.Style.accentColor;
                        anchors.centerIn: parent;
                    }

                    MouseArea {
                        id: _area_2;
                        anchors.fill: parent;
                        hoverEnabled: true;

                        onClicked: switch_from_launcher()
                    }
                    X.ToolTip {
                        visible: _area_2.containsMouse
                        text: "Create a new project";
                    }
                }
            }
        }*/
    }

    /*X.Dialog {

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
    }*/

    Component.onCompleted:  window.drawelr_closed = true;
}
