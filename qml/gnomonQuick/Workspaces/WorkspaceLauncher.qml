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
import gnomon.Utils


G.Workspace {

    id: _workspace;

    uuid: "launcher"

    workspace_title: "Launcher";
    fill: () => {}

    property string current_file: "";
    property string _dialog_source : "";

    P.FolderDialog {
        id: _open_project_folder_dialog;

        folder: P.StandardPaths.writableLocation(P.StandardPaths.HomeLocation);
        modality: Qt.NonModal;

        onAccepted: {
            if(GP.ProjectManager.isExistingProject(_open_project_folder_dialog.folder)) {
                console.log('Loading an existing project');
                splash_screen.project_source = _open_project_folder_dialog.folder
                splash_screen.start()
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

    G.Panel {
        id : panel
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
                            titleTopMargin: G.Style.smallPadding

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
                                _project_title.text = ""
                                _project_description.text = ""
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
                                _workspace._dialog_source = _folder_path.text
                                existing_project_dialog.open()
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
                                id: project_delegate

                                required property string name
                                required property string source
                                required property string description
                                required property string lastModified

                                property bool has_thumbnail: false

                                height: _project_grid.cellHeight - G.Style.smallPadding
                                width: _project_grid.cellWidth - G.Style.smallPadding

                                type: G.Style.CardType.Background
                                outline: true
                                title: name
                                body: "last modified: " + lastModified + "\n" +description
                                tooltip: decodeURIComponent(source).slice(7)
                                titleTopMargin: G.Style.smallPadding

                                background: Rectangle {
                                    color: _getBgColor()
                                    radius: G.Style.cardRadius

                                    border.width: G.Style.borderWidth
                                    border.color: _getBorderColor()
                                }

                                onDoubleClicked: {
                                    splash_screen.project_source = source
                                    splash_screen.start()
                                }

                                G.IconButton {
                                    iconName: "pencil";
                                    size: G.Style.iconSmall;
                                    color: G.Style.colors.textColorFaded;
                                    hoverColor: G.Style.colors.hoveredNeutralColor

                                    outline: true
                                    outlineColor:  G.Style.colors.bgColor;

                                    tooltip: "Change project thumbnail"

                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    anchors.leftMargin: parent.height - parent.titleTopMargin - 2*G.Style.smallPadding - G.Style.tinyPadding
                                    anchors.topMargin: parent.titleTopMargin + G.Style.tinyPadding

                                    onClicked: {
                                        project_thumbnail_dialog.source = source
                                        project_thumbnail_dialog.thumbnail = thumbnail
                                        project_thumbnail_dialog.delegate = project_delegate
                                        project_thumbnail_dialog.resetSelection()
                                        project_thumbnail_dialog.open()
                                    }
                                }

                                G.IconButton {
                                    iconName: "close-thick";
                                    size: G.Style.iconSmall;
                                    color: G.Style.colors.textColorFaded;
                                    hoverColor: G.Style.colors.hoveredDangerColor;

                                    outline: true
                                    outlineColor:  G.Style.colors.bgColor;
                                    tooltip: "Remove from the recent projects"

                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    anchors.topMargin: parent.titleTopMargin + G.Style.tinyPadding
                                    anchors.leftMargin: G.Style.smallPadding + G.Style.tinyPadding

                                    onClicked: {
                                        _close_dialog.open()
                                    }
                                }

                                G.SimpleDialog {
                                    id: _close_dialog

                                    message: "Do you really want to remove the project ?"
                                    caption: "It will disappear from the list of recent projects, but no information will be lost and you will be able to reload it using the Open button."

                                    onAccepted : {
                                        remove_from_history(source)
                                    }
                                }

                                G.IconButton {
                                    id: _load_icon;
                                    iconName:"play-box";
                                    size: G.Style.iconLarge;
                                    color: G.Style.colors.textColorFaded;
                                    hoverColor: G.Style.colors.hoveredBaseColor;
                                    tooltip: "Load and restore last session"

                                    anchors.top: parent.top
                                    anchors.topMargin: G.Style.smallPadding
                                    anchors.right: _more_icon.left
                                    anchors.rightMargin: -G.Style.smallPadding

                                    onClicked: {
                                        splash_screen.project_source = source
                                        splash_screen.start()
                                    }
                                }

                                G.IconButton {
                                    id: _more_icon;
                                    property bool active: false;
                                    iconName: "dots-vertical";
                                    size: G.Style.iconLarge;
                                    color: G.Style.colors.textColorFaded;
                                    hoverColor: G.Style.colors.hoveredBaseColor;
                                    tooltip: "More reloading options..."

                                    anchors.top: parent.top
                                    anchors.topMargin: G.Style.smallPadding
                                    anchors.leftMargin: -G.Style.smallPadding
                                    anchors.right: parent.right

                                    onClicked: {
                                        active = !active
                                        _more_timer.start()
                                    }
                                }

                                Timer {
                                    id: _more_timer;
                                    interval: 2000;
                                    onTriggered: {
                                        if (!(_restart_icon.containsMouse | _pipeline_icon.containsMouse)) {
                                            _more_icon.active = false;
                                        }
                                    }
                                }

                                G.IconButton {
                                    id: _restart_icon;
                                    iconName: "file-plus";
                                    size: G.Style.iconMedium;
                                    color: G.Style.colors.textColorFaded;
                                    hoverColor: G.Style.colors.hoveredOkColor;
                                    tooltip: "Start new blank session"
                                    visible: _more_icon.active

                                    anchors.horizontalCenter: _more_icon.horizontalCenter
                                    anchors.top: _more_icon.bottom
                                    anchors.topMargin: G.Style.smallPadding / 2

                                    onClicked: {
                                        _blank_session_dialog.open()
                                    }

                                    onContainsMouseChanged: {
                                        _more_timer.restart()
                                    }
                                }

                                G.SimpleDialog {
                                    id: _blank_session_dialog

                                    message: "Do you really want to start a new session ?"
                                    caption: "Any previously existing session will be deleted, and the generated data will be lost"

                                    onAccepted : {
                                        open_blank_project(source)
                                    }
                                }
                               
                                G.IconButton {
                                    id: _pipeline_icon;
                                    iconName: "play-network";
                                    size: G.Style.iconMedium;
                                    color: G.Style.colors.textColorFaded;
                                    hoverColor: G.Style.colors.hoveredOkColor;
                                    tooltip: "Replay session from pipeline"
                                    visible: _more_icon.active

                                    anchors.top: _restart_icon.bottom
                                    anchors.topMargin: G.Style.smallPadding
                                    anchors.horizontalCenter:  _more_icon.horizontalCenter

                                    onClicked: {
                                        _pipeline_file_dialog.folder = source
                                        _pipeline_file_dialog.open()
                                    }

                                    onContainsMouseChanged: {
                                        _more_timer.restart()
                                    }
                                }
                                
                                P.FileDialog {
                                    id: _pipeline_file_dialog
                            
                                    nameFilters: ["Json files (*.json)"]
                            
                                    onAccepted: {
                                        open_blank_project(source, true)
                                        load_session(_pipeline_file_dialog.file)
                                    }
                                }

                                Component.onCompleted: {
                                    refreshThumbnail()
                                }

                                function refreshThumbnail() {
                                    project_delegate.thumbnail = ""
                                    project_delegate.has_thumbnail = GUtils.fileExists(source.slice(7) + "/.gnomon/thumbnail.png")
                                    project_delegate.thumbnail = has_thumbnail? source + "/.gnomon/thumbnail.png": "qrc:/qt/qml/gnomon/assets/thumbnail.png"
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

    G.SimpleDialog {
        id: not_project_dialog

        message: "Not an existing project, create a new one ?"

        onAccepted : {
            _project_title.text = ""
            _project_description.text = ""
            new_project_dialog.open()
        }
    }

    G.SimpleDialog {
        id: existing_project_dialog

        message: "This is an existing project, do you want to reset it ?"
        caption: "It will overwrite all the project settings and any previously existing session will be deleted."

        onAccepted : {
             if(GP.ProjectManager.cleanProject(_workspace._dialog_source)) {
                create_project()
            }
        }

        onRejected : {
            GP.ProjectManager.openProject(_workspace._dialog_source)
            add_to_history(_workspace._dialog_source)
        }
    }

    G.Dialog {
        id: project_thumbnail_dialog

        property string source: "";
        property var delegate;
        property string thumbnail: ""

        parent: Overlay.overlay
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.mediumDialogWidth
        height: G.Style.largeDialogHeight + G.Style.smallDelegateHeight

        modal: true
        title: "Select project thumbnail"

        G.IconButton {
            id: _open_image_button

            anchors.top: parent.top
            anchors.left: thumbnail_image.left;
            anchors.margins: G.Style.smallPadding
            anchors.topMargin: -G.Style.smallPadding
            size: G.Style.iconSmall;

            color: G.Style.colors.textColorFaded;
            hoverColor: G.Style.colors.hoveredNeutralColor

            iconName: "folder-open"

            onClicked: {
                _image_file_dialog.folder = project_thumbnail_dialog.source
                _image_file_dialog.open();
            }
        }

        Label {
            anchors.verticalCenter: _open_image_button.verticalCenter
            anchors.left: _open_image_button.right;
            anchors.right: parent.right;
            anchors.margins: G.Style.smallPadding

            text: "Load thumbnail image file..."

            color: G.Style.colors.textColorBase;
            font: G.Style.fonts.value;
        }

        P.FileDialog {
            id: _image_file_dialog

            nameFilters: [ "Image files (*.jpg, *.png)" ]
            title: "Open thumbnail image"
            folder: project_thumbnail_dialog.source
            modality: Qt.WindowModal;
            fileMode: P.FileDialog.OpenFile

            onAccepted: {
                project_thumbnail_dialog.thumbnail = _image_file_dialog.file
                project_thumbnail_dialog.resetSelection()
            }
        }

        Image {
            id: thumbnail_image

            anchors.top: _open_image_button.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: G.Style.smallPadding
            width: G.Style.mediumPanelWidth
            height: G.Style.mediumPanelHeight

            cache: false
            fillMode: Image.PreserveAspectFit
            source: project_thumbnail_dialog.thumbnail

            DropArea {
                id: _drop;

                anchors.fill: parent;

                onDropped: (drop) => {
                    if (drop.hasUrls) {
                        let image_file = decodeURIComponent(drop.urls[0])
                        if (image_file.endsWith(".png") || image_file.endsWith(".jpg")) {
                            project_thumbnail_dialog.thumbnail = image_file
                        }
                    }
                    drop.accept();
                }
            }

            G.SelectionRectangle {
                id: _selection

                target: thumbnail_image
                visible: project_thumbnail_dialog.thumbnail != "" & !project_thumbnail_dialog.thumbnail.startsWith("qrc")
            }
        }

        G.IconButton {
            id: _clear_image_button

            anchors.top: thumbnail_image.bottom
            anchors.left: thumbnail_image.left;
            anchors.margins: G.Style.smallPadding
            size: G.Style.iconSmall;

            color: G.Style.colors.textColorFaded;
            hoverColor: G.Style.colors.hoveredNeutralColor

            iconName: "image-remove"

            onClicked: {
                project_thumbnail_dialog.thumbnail = "qrc:/qt/qml/gnomon/assets/thumbnail.png"
            }
        }

        Label {
            anchors.verticalCenter: _clear_image_button.verticalCenter
            anchors.left: _clear_image_button.right;
            anchors.right: parent.right;
            anchors.margins: G.Style.smallPadding

            text: "Clear thumbnail image"

            color: G.Style.colors.textColorBase;
            font: G.Style.fonts.value;
        }

        function resetSelection() {
            _selection.left_x = -_selection.handleRadius
            _selection.top_y = -_selection.handleRadius
            _selection.right_x = thumbnail_image.width-_selection.handleRadius
            _selection.bottom_y = thumbnail_image.height-_selection.handleRadius
        }

        onClosed: {
            resetSelection()
        }

        standardButtons:  Dialog.Ok | Dialog.Cancel

        onAccepted: {
            let project_path = decodeURIComponent(project_thumbnail_dialog.source).slice(7)

            if (project_thumbnail_dialog.thumbnail.startsWith("qrc")) {
                GUtils.removeProjectThumbnail(project_path)
            } else {
                let thumbnail_path = decodeURIComponent(project_thumbnail_dialog.thumbnail).slice(7)
                let top_left = Qt.point((_selection.left_x+_selection.handleRadius)/thumbnail_image.width, (_selection.top_y+_selection.handleRadius)/thumbnail_image.height)
                let bottom_right = Qt.point((_selection.right_x+_selection.handleRadius)/thumbnail_image.width, (_selection.bottom_y+_selection.handleRadius)/thumbnail_image.height)
                GUtils.makeProjectThumbnail(project_path, thumbnail_path, top_left, bottom_right)
            }
            project_thumbnail_dialog.delegate.refreshThumbnail();
        }
    }

    Loader {
        id: splash_screen

        property string project_source: ""
        asynchronous: true

        function start()  {
            panel.visible = false
            _overlay.visible = true
            splash_screen.source = "qrc:/qt/qml/gnomonQuick/Controls/SplashScreen.qml"
        }

        onLoaded: {
            load_project(splash_screen.project_source)
            history_set_last_used(splash_screen.project_source)
            splash_screen.source = "";
            splash_screen.project_source = "";
            _overlay.visible = false
            panel.visible = true
        }
    }

    Rectangle {
        id: _overlay

        parent:  Overlay.overlay
        anchors.fill: parent
        visible: false

        color: G.Style.colors.overlayColor
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

    Component.onCompleted:  window.drawelr_closed = true;
}
