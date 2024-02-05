import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import QtCore
import Qt.labs.platform as P

import gnomonQuick.Controls as G
import gnomonQuick.Workspaces as G
import gnomonQuick.Style as G

import gnomon.Pipeline  1.0 as GP
import gnomon.Project   1.0 as GP


G.Workspace {

    id: _workspace;

    uuid: "launcher"

    workspace_title: "Launcher";
    fill: () => {}

    property string current_file: "";
    property string _dialog_source : "";
    property bool _blank_session : false

    P.FolderDialog {
        id: _open_project_folder_dialog;

        folder: P.StandardPaths.writableLocation(P.StandardPaths.HomeLocation);
        modality: Qt.NonModal;

        onAccepted: {
            if(GP.ProjectManager.isExistingProject(_open_project_folder_dialog.folder)) {
                console.log('Loading an existing project');
                load_project(_open_project_folder_dialog.folder);
                add_to_history(_open_project_folder_dialog.folder)
            } else {
                _folder_path.text = _open_project_folder_dialog.folder
                not_project_dialog.open()
            }
        }
    }

    P.FolderDialog {
        id: folderDialog
        folder: P.StandardPaths.standardLocations(P.StandardPaths.PicturesLocation)[0]
        onAccepted : {
            _folder_path.text = folderDialog.currentFolder;
        }

        Settings {
            property alias last_open_folder: folderDialog.folder
        }
    }

    P.FileDialog {
        id: loadFileDialog

        nameFilters: ["Json files (*.json)"]

        onAccepted: {
            open_blank_project(loadFileDialog.folder, true)
            load_session(loadFileDialog.file)
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
                            // it's in the app resources. to move in this module?
                            source: "qrc:/qt/qml/gnomon/assets/gnomon_logo.png"
                        }

                        Label {

                            id: _version_tag

                            anchors.top: _logo.bottom;
                            anchors.left: _logo.left;
                            anchors.right: _logo.right;
                            anchors.margins: G.Style.smallPadding

                            font: G.Style.fonts.subHeader
                            color: G.Style.colors.textColorBase;

                            text: "version 0.81.0"

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
                            id: _load

                            anchors.right: _new.left
                            anchors.verticalCenter: _project_header.verticalCenter;
                            anchors.margins: G.Style.mediumPadding

                            size: G.Style.ButtonSize.Large

                            text: "Open"
                            iconName: "folder-open"
                            empty: true

                            onClicked: {
                                _open_project_folder_dialog.open()
                            }
                        }

                        G.Button {
                            id: _new

                            anchors.right: _project_header.right;
                            anchors.verticalCenter: _project_header.verticalCenter;
                            anchors.margins: G.Style.mediumPadding

                            size: G.Style.ButtonSize.Large
                            type: G.Style.ButtonType.Base

                            text: "New"
                            iconName: "plus"

                            onClicked: {
                                _folder_path.text = ""
                                new_project_dialog.open()
                            }
                        }
                    }

                    G.Dialog {
                        id: new_project_dialog

                        parent: Overlay.overlay
                        x: (parent.width - width) / 2
                        y: (parent.height - height) / 2
                        width: G.Style.mediumDialogWidth
                        height: G.Style.largeDialogHeight

                        modal: true
                        title: "Create a new Project"
                        standardButtons:  Dialog.Open | Dialog.Cancel

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
                                text: "Folder"
                                font: G.Style.fonts.formLabel

                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignTop

                                wrapMode: Text.Wrap
                                color: G.Style.colors.textColorBase
                            }

                            Rectangle {
                                Layout.fillWidth: true;
                                height: G.Style.mediumLabelHeight

                                color: G.Style.colors.gutterColor;
                                radius: G.Style.panelRadius

                                G.TextField {
                                    id: _folder_path
                                    Layout.fillWidth: true
                                    anchors.right: _folder_button.left
                                    anchors.left: parent.left
                                    anchors.bottom: parent.bottom;
                                    anchors.bottomMargin: G.Style.tinyPadding
                                    placeholderText: qsTr("Enter folder path like: file://...")
                                }
                                G.Button {
                                    id: _folder_button
                                    anchors.bottom: parent.bottom;
                                    anchors.right: parent.right;

                                    text: "Folder";
                                    type: G.Style.ButtonType.Neutral
                                    iconName: "folder"
                                    empty: true

                                    onClicked: {
                                        folderDialog.open()
                                    }
                                }
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

                            Rectangle {
                                Layout.fillWidth: true;
                                height: G.Style.largeLabelHeight

                                color: G.Style.colors.gutterColor;
                                radius: G.Style.panelRadius

                                G.TextField {
                                    id: _project_title

                                    anchors.fill: parent
                                    anchors.margins: G.Style.smallPadding

                                    text: ""
                                    placeholderText: "New project title"
                                    font: G.Style.fonts.header

                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter

                                    wrapMode: Text.Wrap
                                    color: G.Style.colors.textColorNeutral
                                    placeholderTextColor: G.Style.colors.textColorDeEmphasize
                                }
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
                                id: _project_description
                                Layout.fillWidth: true;
                                Layout.fillHeight: true;
                                text: ""
                                placeholderText: "Enter the desciption of the project..."
                                font: G.Style.fonts.value

                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignTop

                                wrapMode: Text.Wrap
                            }

                            Label {
                                Layout.fillWidth: true;
                                text: "Initial Workspace"
                                font: G.Style.fonts.formLabel

                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignTop

                                wrapMode: Text.Wrap
                                color: G.Style.colors.textColorBase
                            }

                            Control {
                                Layout.fillWidth: true;
                                height: G.Style.comboBoxHeight;

                                G.ComboBox {
                                    id: _pipeline_workspace

                                    anchors.left: parent.left
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: parent.width - _remember_workspace.width - G.Style.mediumPadding

                                    model: []
                                    textRole: "title"
                                    valueRole: "source"

                                    delegate: G.ComboBoxDelegate {
                                        text: model["title"]
                                        width: parent.width
                                    }

                                    Component.onCompleted: {
                                        let workspaces = Qt.createQmlObject("import QtQuick 2.15; ListModel { }", _pipeline_workspace, "");
                                        let titles = [];
                                        for (let i=0; i<_workspace_dialog.available_workspaces.count; i++) {
                                            let w = _workspace_dialog.available_workspaces.get(i);
                                            if (w.initial) {
                                                workspaces.append(w);
                                                titles.push(w.title);
                                            }
                                        }
                                        model = workspaces
                                        currentIndex = titles.indexOf(_settings.default_workspace)
                                        if (currentIndex == -1) {
                                            currentIndex = 0;
                                        }
                                    }
                                }

                                G.CheckBox {
                                    id: _remember_workspace

                                    anchors.right: parent.right
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: G.Style.buttonWidth
                                    text: "Make default"
                                    checked: false
                                    tooltip: "Check to make the chosen workspace the default option when you create a new session for the project."
                                }
                            }

                            Settings {
                                id: _settings
                                category: "project"
                                property var default_workspace: "Browsing"
                            }
                        }

                        onAccepted: {
                            if(GP.ProjectManager.isExistingProject(_folder_path.text)) {
                                open_project_dialog(_folder_path.text)
                            } else {
                                create_project()
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

                        Control {
                            id: _no_project_hint

                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter

                            width: G.Style.mediumPanelWidth
                            height: G.Style.mediumDelegateHeight

                            visible: window.recent_projects.count == 0

                            G.IconButton {
                                id: _no_project_new_button

                                anchors.verticalCenter: _no_project_new_label.verticalCenter
                                anchors.left: parent.left
                                anchors.margins: G.Style.smallPadding

                                size: G.Style.iconMedium;
                                iconName: "plus"
                                color: G.Style.colors.bgColor

                                onClicked: {
                                    _new.clicked()
                                }
                            }

                            Label {
                                id: _no_project_new_label
                                anchors.bottom: parent.verticalCenter
                                anchors.left: _no_project_new_button.right
                                anchors.right: parent.right
                                anchors.margins: G.Style.smallPadding

                                text: "Start by creating a **new** project"
                                textFormat: Text.MarkdownText
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignTop

                                wrapMode: Text.Wrap
                                color: G.Style.colors.bgColor
                                font: G.Style.fonts.cardTitle
                            }

                            G.IconButton {
                                id: _no_project_load_button

                                anchors.verticalCenter: _no_project_load_label.verticalCenter
                                anchors.left: parent.left
                                anchors.margins: G.Style.smallPadding

                                size: G.Style.iconMedium;
                                iconName: "folder-open"
                                color: G.Style.colors.bgColor

                                onClicked: {
                                    _load.clicked()
                                }
                            }

                            Label {
                                id: _no_project_load_label
                                anchors.top: parent.verticalCenter
                                anchors.left: _no_project_load_button.right
                                anchors.right: parent.right
                                anchors.margins: G.Style.smallPadding

                                text: "Or simply **open** an existing one"
                                textFormat: Text.MarkdownText
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignTop

                                wrapMode: Text.Wrap
                                color: G.Style.colors.bgColor
                                font: G.Style.fonts.cardTitle
                            }

                        }

                        GridView {
                            id: _project_grid

                            anchors.fill: parent
                            anchors.margins: G.Style.smallPadding;

                            clip: true;
                            focus: true;
                            visible: window.recent_projects.count > 0

                            model: window.recent_projects

                            cellWidth: _projects.width/2 - G.Style.smallPadding;
                            cellHeight: G.Style.largeDelegateHeight;
                            displayMarginBeginning: G.Style.smallPadding;
                            displayMarginEnd: G.Style.smallPadding;

                            ScrollIndicator.vertical: ScrollIndicator {
                                id: _scroll_indicator;

                                width: 8;
                                visible: _project_grid.contentHeight > _project_grid.height;
                            }

                            delegate: G.Card {
                                required property string name
                                required property string source
                                required property string description
                                required property string lastModified

                                height: _project_grid.cellHeight - G.Style.smallPadding
                                width: _project_grid.cellWidth - G.Style.smallPadding

                                type: G.Style.CardType.Background
                                outline: true
                                title: name
                                body: "last modified: " + lastModified + "\n" +description
                                tooltip: source
                                titleTopMargin: G.Style.iconLarge

                                background: Rectangle {
                                    color: _getBgColor()
                                    radius: G.Style.cardRadius

                                    border.width: G.Style.borderWidth
                                    border.color: _getBorderColor()
                                }
                                // there are no thumbnails for now
                                //thumbnail: "image://thumbnails/project_" + index

                                onDoubleClicked: {
                                    history_set_last_used(source)
                                    load_project(source)
                                }

                                G.IconButton {
                                    id: _trash_icon;
                                    iconName: "close-box";
                                    size: G.Style.iconLarge;
                                    color: G.Style.colors.fgColor;
                                    hoverColor: G.Style.colors.hoveredDangerColor;
                                    tooltip: "Remove from the recent projects"

                                    anchors.top: parent.top
                                    anchors.topMargin: G.Style.smallPadding / 2
                                    anchors.left: parent.left
                                    anchors.leftMargin: G.Style.smallPadding
                                    anchors.bottomMargin: G.Style.smallPadding / 2

                                    onClicked: {
                                        remove_from_history(source)
                                    }
                                }

                                G.IconButton {
                                    id: _load_icon;
                                    iconName:"play-box";
                                    size: G.Style.iconLarge;
                                    color: G.Style.colors.fgColor;
                                    hoverColor: G.Style.colors.hoveredBaseColor;
                                    tooltip: "Load and restore last session"

                                    anchors.top: parent.top
                                    anchors.topMargin: G.Style.smallPadding
                                    anchors.right: _more_icon.left
                                    anchors.rightMargin: G.Style.smallPadding/2

                                    onClicked: {
                                        history_set_last_used(source)
                                        load_project(source)
                                    }
                                }

                                G.IconButton {
                                    id: _more_icon;
                                    property bool active: false;
                                    iconName: "dots-horizontal";
                                    size: G.Style.iconLarge;
                                    color: G.Style.colors.fgColor;
                                    hoverColor: G.Style.colors.hoveredBaseColor;
                                    tooltip: "Remove from the recent projects"

                                    anchors.top: parent.top
                                    anchors.topMargin: G.Style.smallPadding
                                    anchors.right: parent.right
                                    anchors.rightMargin: G.Style.smallPadding

                                    onClicked: {
                                        active = !active
                                    }
                                }

                                G.IconButton {
                                    id: _restart_icon;
                                    iconName: "plus-box";
                                    size: G.Style.iconMedium;
                                    color: G.Style.colors.fgColor;
                                    hoverColor: G.Style.colors.hoveredOkColor;
                                    tooltip: "Start a new blank session"
                                    visible: _more_icon.active

                                    anchors.top: _more_icon.bottom
                                    anchors.topMargin: G.Style.smallPadding / 2
                                    anchors.right: parent.right
                                    anchors.rightMargin: G.Style.smallPadding

                                    onClicked: {
                                        open_project_dialog(source, true)
                                    }
                                }
                               
                                G.IconButton {
                                    id: _pipeline_icon;
                                    iconName: "pipe";
                                    size: G.Style.iconMedium;
                                    color: G.Style.colors.fgColor;
                                    hoverColor: G.Style.colors.hoveredOkColor;
                                    tooltip: "Create session from pipeline"
                                    visible: _more_icon.active

                                    anchors.top: _restart_icon.bottom
                                    anchors.topMargin: G.Style.smallPadding / 2
                                    anchors.right: parent.right
                                    anchors.rightMargin: G.Style.smallPadding

                                    onClicked: {
                                        loadFileDialog.folder =  source
                                        loadFileDialog.open()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    G.Toast {
        id: launching_toast
        parent: Overlay.overlay
        header: "Launching Workspace " + _pipeline_workspace.currentText
        message: "Please wait while the corresponding plugins are being loaded. This may take a few seconds."

        type: G.Style.ButtonType.Base

        onOpened: {
            //switch_from_launcher(_pipeline_workspace.currentValue);
        }
    }

    G.Dialog {
        id: not_project_dialog

        simple_dialog : true

        parent: Overlay.overlay
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.smallDialogWidth
        height: G.Style.largeDelegateHeight
        header.height: 0

        modal: true


        Label {
            text: "Not an existing project, create a new one ?"
            font: G.Style.fonts.cardText
        }

        standardButtons:  Dialog.Yes | Dialog.No

        onAccepted : {
            new_project_dialog.open()
        }
    }

    G.Dialog {
        id: existing_project_dialog

        simple_dialog : true

        parent: Overlay.overlay
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.smallDialogWidth
        height: G.Style.largeDelegateHeight + G.Style.mediumLabelHeight
        header.height: 0

        modal: true

        Label {
            id: _message
            text: _workspace._blank_session ? "Do you really want to start a new session ?" : "Do you really want to reset project settings ?"
            font: G.Style.fonts.cardLabel
        }

        Text {
            anchors.top: _message.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: G.Style.smallPadding

            text: _workspace._blank_session ? "Any previously existing session will be deleted, and the generated data will be lost" :
                                              "Any previously existing session will be deleted, and all parameters will be set to their default values"
            font: G.Style.fonts.value
            color: G.Style.colors.textColorBase
            wrapMode: Text.Wrap
        }

        standardButtons:  Dialog.Yes | Dialog.No

        onAccepted : {
            if(_workspace._blank_session) {
                history_set_last_used(_workspace._dialog_source)
                open_blank_project(_workspace._dialog_source)
            } else if(GP.ProjectManager.cleanProject(_workspace._dialog_source)) {
                create_project()
            }
        }

        onRejected : {
            GP.ProjectManager.openProject(_workspace._dialog_source)
            add_to_history(_workspace._dialog_source)
        }
    }

    function create_project() {
        GP.ProjectManager.createProject(_folder_path.text,
                                        _project_title.text,
                                        _project_description.text,
                                        _pipeline_workspace.currentValue)
        GP.PipelineManager.pipeline.name = _project_title.text
        GP.PipelineManager.pipeline.description = _project_description.text
        if (_remember_workspace.checked) {
            _settings.default_workspace = _pipeline_workspace.currentText
        }
        add_to_history(_folder_path.text)
        launching_toast.open()
    }

    function open_project_dialog(source, blank_project = false) {
        _workspace._dialog_source = source
        _workspace._blank_session = blank_project
        existing_project_dialog.open()
    }

    Component.onCompleted:  window.drawelr_closed = true;
}
