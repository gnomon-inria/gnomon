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

import gnomon.Pipeline  1.0 as GP


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
        nameFilters: ["Json files (*.json)"]

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
                implicitWidth: G.Style.mediumPanelWidth
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: G.Style.mediumColumnSpacing
                    spacing: G.Style.mediumColumnSpacing;

                    Item {
                        id: _gnomon_header

                        Layout.fillWidth: true;
                        implicitHeight: G.Style.largeDelegateHeight;

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

                            text: "version 0.72.0"

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

                            // text: "Deciphering morphogenesis"
                            text: ""

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

                        G.Card {
                            Layout.fillWidth: true;
                            Layout.fillHeight: true;

                            type: G.Style.CardType.Foreground
                            title: header
                            body: paragraph

                            onClicked: {
                                Qt.openUrlExternally(link);
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
                            anchors.right: _new.left
                            anchors.verticalCenter: _project_header.verticalCenter;
                            anchors.margins: G.Style.mediumPadding

                            size: G.Style.ButtonSize.Large

                            text: "Load"
                            iconName: G.Icons.icons["play"]
                            empty: true

                            onClicked: {
                                _file_dialog.open()
                            }
                        }

                        G.Button {
                            id: _new

                            anchors.right: _project_header.right;
                            anchors.verticalCenter: _project_header.verticalCenter;
                            anchors.margins: G.Style.mediumPadding

                            size: G.Style.ButtonSize.Large
                            type: new_pipeline_info.visible ? G.Style.ButtonType.OK : G.Style.ButtonType.Base

                            text: "New"
                            iconName: G.Icons.icons["plus"]

                            onClicked: {
                                if(new_pipeline_info.visible) {
                                    new_pipeline_info.close()
                                    GP.PipelineManager.pipeline.name = new_pipeline_info.pipeline_title
                                    GP.PipelineManager.pipeline.description = new_pipeline_info.pipeline_descr
                                    switch_from_launcher()
                                } else {
                                    new_pipeline_info.open()
                                }
                            }
                            onDoubleClicked: {
                                new_pipeline_info.close()
                                GP.PipelineManager.pipeline.name = new_pipeline_info.pipeline_title
                                GP.PipelineManager.pipeline.description = new_pipeline_info.pipeline_descr
                                switch_from_launcher()
                            }
                        }
                    }

                    Control {
                        id: _projects

                        Layout.fillWidth: true;
                        Layout.fillHeight: true;

                        Popup {
                            id: new_pipeline_info
                            height: _projects.height
                            width: G.Style.mediumPanelWidth
                            x: _projects.x + _projects.width - width
                            //x: _projects.x
                            y: _project_header.y
                            modal: false
                            topInset: 0

                            property alias pipeline_title: _pipeline_title.text
                            property alias pipeline_descr: _pipeline_description.text

                            enter.enabled: false
                            exit.enabled: false

                            background: Rectangle {
                                anchors.fill: parent
                                color: G.Style.colors.bgColor;
                                border.color: G.Style.colors.gutterColor
                                border.width: G.Style.borderWidth
                            }


                            ColumnLayout {
                                id: _layout
                                anchors.fill: parent

                                Label {
                                    Layout.fillWidth: true;
                                    text: "New project infos"
                                    font: G.Style.fonts.h3

                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignTop

                                    wrapMode: Text.Wrap
                                    color: G.Style.colors.textColorBase

                                }

                                Label {
                                    Layout.fillWidth: true;
                                    text: "Title"
                                    font: G.Style.fonts.formLabel

                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignTop

                                    wrapMode: Text.Wrap
                                    color: G.Style.colors.textColorBase
                                }

                                TextField {
                                    id: _pipeline_title
                                    Layout.fillWidth: true;
                                    text: ""
                                    placeholderText: "Pipeline's title"
                                    font: G.Style.fonts.value

                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignTop

                                    wrapMode: Text.Wrap
                                    color: G.Style.colors.textColorBase
                                }

                                Label {
                                    Layout.fillWidth: true;
                                    text: "Description"
                                    font: G.Style.fonts.formLabel

                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignTop

                                    wrapMode: Text.Wrap
                                    color: G.Style.colors.textColorBase
                                }

                                G.TextArea {
                                    id: _pipeline_description
                                    Layout.fillWidth: true;
                                    Layout.fillHeight: true;
                                    text: ""
                                    placeholderText: "Enter the desciption of the pipeline here ..."
                                    font: G.Style.fonts.value

                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignTop

                                    wrapMode: Text.Wrap
                                    color: G.Style.colors.textColorBase
                                }
                            }
                        }

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

                            ScrollIndicator.vertical: ScrollIndicator {
                                id: _scroll_indicator;

                                width: 8;
                                visible: _project_grid.contentHeight > _project_grid.height;
                            }

                            delegate: G.Card {
                                height: _project_grid.cellHeight - G.Style.smallPadding
                                width: _project_grid.cellWidth - G.Style.smallPadding

                                type: G.Style.CardType.Background
                                outline: true
                                title: name
                                body: description
                                tooltip: source
                                background: Rectangle {
                                    color: _getBgColor()
                                    radius: G.Style.cardRadius

                                    border.width: G.Style.borderWidth
                                    border.color: _getBorderColor()
                                }
                                // there are no thumbnails for now
                                //thumbnail: "image://thumbnails/project_" + index

                                onDoubleClicked: {
                                    load_session(source)
                                }

                                G.IconButton {
                                    id: _load_icon;
                                    iconName: G.Icons.icons["arrow-down"];
                                    size: G.Style.iconLarge;
                                    color: G.Style.colors.fgColor;
                                    tooltip: "Load"

                                    anchors.top: parent.top
                                    anchors.topMargin: G.Style.smallPadding
                                    anchors.right: _trash_icon.left
                                    anchors.rightMargin: G.Style.smallPadding

                                    onClicked: {
                                        load_session(source)
                                    }
                                }
                                G.IconButton {
                                    id: _trash_icon;
                                    iconName: G.Icons.icons["trash-can"];
                                    size: G.Style.iconLarge;
                                    color: G.Style.colors.fgColor;
                                    tooltip: "Delete"

                                    anchors.top: parent.top
                                    anchors.topMargin: G.Style.smallPadding
                                    anchors.right: parent.right
                                    anchors.rightMargin: G.Style.smallPadding

                                    onClicked: {
                                        remove_from_history(source)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted:  window.drawelr_closed = true;
}
