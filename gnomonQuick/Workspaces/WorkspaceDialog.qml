import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G
import gnomonQuick.Workspaces 1.0 as G

import gnomon.MetaData    1.0 as GM

G.Dialog {
    id: _self;

    QtObject {
        id: _internal;

        property var selected_workspace: _list_view.currentIndex > -1 ? _available_workspaces.get(_list_view.currentIndex) : undefined;
        property var workspace_groups: []
        property var workspace_plugins: []
        property var plugin_workspace: []
        property var workspace_forms: []
        property string algoName: ""

    }
    property alias plugin_workspace: _internal.plugin_workspace
    property alias available_workspaces: _available_workspaces

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: G.Style.largeDialogWidth
    height: 1.5*G.Style.largeDialogHeight

    padding: 0;

    parent: Overlay.overlay
    modal: true
    title: "Open a new Workspace"
    standardButtons:  Dialog.Open | Dialog.Cancel

    onAccepted: {
        if (_internal.selected_workspace) {
            _internal.algoName = ""
            let show_dialog = !window.check_if_forms_in_world();
            if (window.current_workspace().workspace_title == "Browsing") {
                show_dialog = show_dialog & !window.current_workspace().d.view.empty
            }
            if(show_dialog) {
                no_form_exported_dialog.workspace_source = _internal.selected_workspace.source
                no_form_exported_dialog.open()
                no_form_exported_dialog.forceActiveFocus()
                _self.close()
                return
            }
            openWorkspaceAndClose(_internal.selected_workspace.source)
        }
    }

    onOpened: {
        _list_view.currentIndex = 0;
        _workspace_search_bar.text = ""
        _workspace_search_bar.forceActiveFocus()
        _internal.algoName = ""
    }

    function openWorkspaceAndClose(source) {
        window.add_workspace(source)
        if(_internal.algoName != "") {
            window.current_workspace().d.algoName = _internal.algoName
        }
        _self.close()
    }

    G.Gutter {
        id: _workspace_selection_panel;

        width: _self.width / 3;

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.margins: G.Style.smallPadding;

        G.TextField {

            id: _workspace_search_bar;

            height: G.Style.mediumLabelHeight

            anchors.right: _workspace_selection_panel.right;
            anchors.left: _workspace_selection_panel.left;
            anchors.top: _workspace_selection_panel.top;

            placeholderText: "search a plugin or workspace"
            color: G.Style.colors.textColorNeutral
            placeholderTextColor: G.Style.colors.textColorDeEmphasize

            background: Rectangle {
                anchors.fill: parent
                color: G.Style.colors.gutterColor
            }

            onTextChanged: {
                _model.refilter();
                _list_view.currentIndex = _model.items.count > 0 ? _model.items.get(0).model.index : -1
            }

            onAccepted: {
                _self.accept()
            }

            Keys.onPressed: (event) => {
                // TODO: broken when filter is active because of proxy interaction
                if (event.key == Qt.Key_Down) {
                    event.accepted = true;
                    _list_view.incrementCurrentIndex();
                } else if (event.key == Qt.Key_Up) {
                    event.accepted = true;
                    _list_view.decrementCurrentIndex();
                }
            }

            G.Icon {
                icon: "text-search";
                color: G.Style.colors.neutralColor;

                anchors.right: _workspace_search_bar.right;
                anchors.rightMargin: G.Style.smallPadding;
                anchors.verticalCenter: _workspace_search_bar.verticalCenter;
            }
        }

        ListView {
            id: _list_view

            anchors.top: _workspace_search_bar.bottom;
            anchors.bottom: _workspace_selection_panel.bottom;
            anchors.right: _workspace_selection_panel.right;
            anchors.left: _workspace_selection_panel.left;
            clip: true;
            focus: true;
            currentIndex: -1

            model: G.FilterProxyModel {
                id: _model;

                model: _available_workspaces;

                delegate: G.ListItemDelegate {
                    id: _delegate
                    implicitHeight: G.Style.largeButtonHeight

                    required property string type
                    required property string title
                    required property string source
                    required property bool available
                    required property bool initial
                    required property bool highlightElement
                    required property int index

                    width: _list_view.width
                    highlighted: _list_view.currentIndex == index

                    text: title;
                    font: G.Style.fonts.cardLabel
                    enabled: available

                    onClicked: {
                        _list_view.currentIndex = index
                    }

                    onDoubleClicked: {
                        _list_view.currentIndex = index
                        _self.accept()
                    }

                    G.Icon {
                        icon: "cancel";
                        color: G.Style.colors.dangerColor; // "red"
                        visible: !available

                        anchors.right: parent.right;
                        anchors.rightMargin: G.Style.smallPadding;
                        anchors.verticalCenter: parent.verticalCenter;
                    }

                    Rectangle {
                        anchors.fill: parent
                        anchors.topMargin: -1;
                        anchors.bottomMargin: -1;

                        visible: highlightElement
                        z: -1
                        color: G.Style.colors.hoveredOkColor;
                    }

                    contentItem: Text {
                        text: _delegate.text
                        font: _delegate.font
                        color: highlightElement ? G.Style.colors.lightGreen : G.Style.colors.textColorNeutral
                    }
                }

                filterAccepts: function(item) {
                    let w = model.get(item.index)
                    let content = w.title + w.description
                    let plugins = _internal.workspace_plugins[w.type]
                    for (var i in plugins) {
                        content += plugins[i]
                    }
                    return content.toUpperCase().includes(_workspace_search_bar.text.toUpperCase())
                }
            }

            ScrollIndicator.vertical: ScrollIndicator { }

            Component.onCompleted: {

                for(let i = 0; i < _available_workspaces.count; i++) {
                    if(_available_workspaces.get(i).available) {
                        currentIndex = i;
                        break;
                    }
                }
                /* currentIndex = 0 */
            }
        }
    }


    G.WorkspaceDescriptionDelegate {
        id: _workspace_description_panel;

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        anchors.left: _workspace_selection_panel.right;
        anchors.margins: G.Style.smallPadding;

        show_description: true

        title: _internal.selected_workspace ? _internal.selected_workspace.title : ""
        description: _internal.selected_workspace ? _internal.selected_workspace.description : ""
        preview: _internal.selected_workspace ? _internal.selected_workspace.preview : ""
        plugins : _internal.selected_workspace ? _internal.workspace_plugins[_internal.selected_workspace.type] : []

        onOpenWithAlgo : (algo_name) => {
            _internal.algoName = algo_name
            if(!window.check_if_forms_in_world()) {
                no_form_exported_dialog.workspace_source = _internal.selected_workspace.source
                no_form_exported_dialog.open()
                no_form_exported_dialog.forceActiveFocus()
                _self.close()
                return
            }
            openWorkspaceAndClose(_internal.selected_workspace.source)
        }
    }

    G.Dialog {
        id: no_form_exported_dialog
        width: G.Style.mediumDialogWidth
        height: G.Style.mediumDialogHeight
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        property string workspace_source: ""

        modal: true
        title: "No form exported yet."

        onAccepted: {
            openWorkspaceAndClose(workspace_source)
            no_form_exported_dialog.close()
        }



        ColumnLayout {
            anchors.fill: parent
            Label {
                Layout.fillWidth: true
                Layout.fillHeight: false

                text: "There are currently no forms in the world (top bar). You probably want to export a form to it with the export button : "
                wrapMode: TextInput.Wrap
                font: G.Style.fonts.value
                color: G.Style.colors.textColorBase
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignJustify
            }
            G.Icon {
                Layout.fillWidth: true
                Layout.fillHeight: true
                icon: "arrow-up-drop-circle"
                color: G.Style.colors.textColorNeutral
                font: G.Style.fonts.header
            }
        }

        footer: DialogButtonBox
        {
            visible: true
            // anchors.left: parent.left
            // anchors.right: parent.right
            // anchors.bottom: parent.bottom
            G.Button {
                id: _cancel_button
                anchors.right: _ignore_button.left
                anchors.margins: G.Style.smallRowSpacing
                text: 'Cancel';
                flat: true
                type: G.Style.ButtonType.Neutral
                onClicked: no_form_exported_dialog.reject();
            }

            G.Button {
                anchors.right: _export_button.left
                anchors.margins: G.Style.smallRowSpacing
                id: _ignore_button
                text: 'Ignore';
                flat: true
                type: G.Style.ButtonType.Base

                onClicked: {
                    no_form_exported_dialog.accept();
                }
            }

            G.Button {
                id: _export_button
                anchors.right: parent.right
                anchors.margins: G.Style.smallRowSpacing
                text: 'Export and Continue';
                iconName: "arrow-up-drop-circle"
                flat: false
                type: G.Style.ButtonType.OK
                width: G.Style.buttonWidth * 2

                onClicked: {
                    // export
                    let workspace = window.current_workspace()
                    workspace.d.export_outputs()
                    no_form_exported_dialog.accept();
                }
            }
        }

    }

    ListModel {
        id: _available_workspaces

        ListElement {
            type: "gnomonWorkspaceBinarization"
            title: "Binarization"
            source: "qrc:/gnomonQuick/Workspaces/WorkspaceBinarization.qml"
            available: true
            initial: false
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspaceBrowser"
            title: "Browsing"
            source: "qrc:/gnomonQuick/Workspaces/WorkspaceBrowsing.qml"
            available: true
            initial: true
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspaceCellImageQuantification"
            title: "Cell Image Quantification"
            source: "qrc:/gnomonQuick/Workspaces/WorkspaceCellImageQuantification.qml"
            available: true
            initial: false
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspaceCellImageTracking"
            title: "Cell Image Tracking"
            source: "qrc:/gnomonQuick/Workspaces/WorkspaceCellImageTracking.qml"
            available: true
            initial: false
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspaceImageMeshing"
            title: "Image Meshing"
            source: "qrc:/gnomonQuick/Workspaces/WorkspaceImageMeshing.qml"
            available: true
            initial: false
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspaceLSystemModel"
            title: "L-System Model"
            source: "qrc:/gnomonQuick/Workspaces/WorkspaceLSystemModel.qml"
            available: true
            initial: true
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspaceMeshFilter"
            title: "Mesh Processing"
            source: "qrc:/gnomonQuick/Workspaces/WorkspaceMeshFilter.qml"
            available: true
            initial: false
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspaceMorphonet"
            title: "MorphoNet"
            source: "qrc:/gnomonQuick/Workspaces/WorkspaceMorphonet.qml"
            available: true
            initial: true
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspacePointCloudQuantification"
            title: "Point Cloud Quantification"
            source: "qrc:/gnomonQuick/Workspaces/WorkspacePointCloudQuantification.qml"
            available: true
            initial: false
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspacePointDetection"
            title: "Point Detection"
            source: "qrc:/gnomonQuick/Workspaces/WorkspacePointDetection.qml"
            available: true
            initial: false
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspacePreprocess"
            title: "Preprocessing"
            source: "qrc:/gnomonQuick/Workspaces/WorkspacePreprocess.qml"
            available: true
            initial: false
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspacePythonAlgorithm"
            title: "Python Algorithm"
            source: "qrc:/gnomonQuick/Workspaces/WorkspacePythonAlgorithm.qml"
            available: true
            initial: true
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspaceRegistration"
            title: "Registration"
            source: "qrc:/gnomonQuick/Workspaces/WorkspaceRegistration.qml"
            available: true
            initial: false
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspaceSegmentation"
            title: "Segmentation"
            source: "qrc:/gnomonQuick/Workspaces/WorkspaceSegmentation.qml"
            available: true
            initial: false
            highlightElement: false
        }
        ListElement {
            type: "gnomonWorkspaceSimulation"
            title: "Simulation"
            source: "qrc:/gnomonQuick/Workspaces/WorkspaceSimulation.qml"
            available: true
            highlightElement: false
        }
    }

    function has_loaded_plugins(index) {
        const workspace = _available_workspaces.get(index)
        const plugins = _internal.workspace_plugins[workspace.type]
        return plugins.length > 0;
    }

    function update_highlight() {
        if(window.world.count > 0) {
            for (var i=0; i<_available_workspaces.count; i++) {
                _available_workspaces.setProperty(i, "highlightElement", false)
                let w = _available_workspaces.get(i)
                if(w.type in _internal.workspace_forms && window.world.count > 0) {
                    let temp_forms = _internal.workspace_forms[w.type]
                    for(let f in temp_forms) {
                        if(temp_forms[f] == window.world.getFormName(window.world.currentIndex)) {
                            _available_workspaces.setProperty(i, "highlightElement", true)
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        _internal.workspace_groups["gnomonWorkspaceBinarization"] = "binaryImageFromImage"
        _internal.workspace_groups["gnomonWorkspaceBrowser"] = ["binaryImageReader", "cellImageReader", "cellComplexReader", "imageReader", "meshReader", "pointCloudReader"]
        _internal.workspace_groups["gnomonWorkspaceCellImageQuantification"] = "cellImageQuantification"
        _internal.workspace_groups["gnomonWorkspaceImageMeshing"] = "meshFromImage"
        _internal.workspace_groups["gnomonWorkspaceLSystemModel"] = "lStringEvolutionModel"
        _internal.workspace_groups["gnomonWorkspaceMorphonet"] = ""
        _internal.workspace_groups["gnomonWorkspaceMeshFilter"] = "meshFilter"
        _internal.workspace_groups["gnomonWorkspaceCellImageTracking"] = "cellImageTracking"
        _internal.workspace_groups["gnomonWorkspacePointCloudQuantification"] = "pointCloudQuantification"
        _internal.workspace_groups["gnomonWorkspacePointDetection"] = "pointCloudFromImage"
        _internal.workspace_groups["gnomonWorkspacePreprocess"] = "imageFilter"
        _internal.workspace_groups["gnomonWorkspacePythonAlgorithm"] = "formAlgorithm"
        _internal.workspace_groups["gnomonWorkspaceRegistration"] = "imageRegistration"
        _internal.workspace_groups["gnomonWorkspaceSegmentation"] = "cellImageFromImage"
        _internal.workspace_groups["gnomonWorkspaceSimulation"] = "femSolver"

        _internal.workspace_forms["gnomonWorkspaceBinarization"] = ["gnomonImage", "gnomonBinaryImage"]
        _internal.workspace_forms["gnomonWorkspaceCellImageQuantification"] = ["gnomonCellImage", "gnomonImage"]
        _internal.workspace_forms["gnomonWorkspaceImageMeshing"] = ["gnomonImage", "gnomonCellImage", "gnomonBinaryImage"]
        _internal.workspace_forms["gnomonWorkspaceLSystemModel"] = ["gnomonLString", "gnomonTree"]
        _internal.workspace_forms["gnomonWorkspaceMorphonet"] = ["gnomonCellImage"]
        _internal.workspace_forms["gnomonWorkspaceMeshFilter"] = ["gnomonMesh"]
        _internal.workspace_forms["gnomonWorkspaceCellImageTracking"] = ["gnomonCellImage", "gnomonDataDict", "gnomonImage"]
        _internal.workspace_forms["gnomonWorkspacePointCloudQuantification"] = ["gnomonPointCloud", "gnomonImage"]
        _internal.workspace_forms["gnomonWorkspacePointDetection"] = ["gnomonImage"]
        _internal.workspace_forms["gnomonWorkspacePreprocess"] = ["gnomonImage", "gnomonBinaryImage"]
        _internal.workspace_forms["gnomonWorkspacePythonAlgorithm"] = ["gnomonBinaryImage", "gnomonCellImage", "gnomonImage", "gnomonMesh", "gnomonPointCloud"]
        _internal.workspace_forms["gnomonWorkspaceRegistration"] = ["gnomonImage"]
        _internal.workspace_forms["gnomonWorkspaceSegmentation"] = ["gnomonImage", "gnomonBinaryImage", "gnomonPointCloud"]
        _internal.workspace_forms["gnomonWorkspaceSimulation"] = ["gnomonMesh", "gnomonDataDict"]

        for (var i=0; i<_available_workspaces.count; i++) {
            let w = _available_workspaces.get(i)
            _available_workspaces.setProperty(i, "section", w.title[0])
            let desc = GM.MetaData.workspaceMetaData(w.type, "description")
            _available_workspaces.setProperty(i, "description", desc)
            let preview = w.type + ".png"
            _available_workspaces.setProperty(i, "preview", preview)
            let plugins = []
            if (w.type in _internal.workspace_groups) {
                plugins = GM.MetaData.pluginGroupMetaData(_internal.workspace_groups[w.type])
                for(var p=0; p<plugins.length; p++) {
                    _internal.plugin_workspace[plugins[p]] = w.type
                }
            }
            _internal.workspace_plugins[w.type] = plugins
            _available_workspaces.setProperty(i, "available", (plugins.length > 0 || w.type === "gnomonWorkspacePythonAlgorithm" || w.type === "gnomonWorkspaceMorphonet" || w.type === "gnomonWorkspaceLSystemModel"))
        }
    }
}
