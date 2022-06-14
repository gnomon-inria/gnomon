import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X
import xQuick.Fonts     1.0 as X
import xQuick.Models    1.0 as X
import xQuick.Style     1.0 as X

import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G
import gnomonQuick.Icons      1.0 as G
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

    }
    property alias plugin_workspace: _internal.plugin_workspace
    property alias available_workspaces: _available_workspaces

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: G.Style.largeDialogWidth
    height: G.Style.largeDialogHeight

    padding: 0;

    parent: Overlay.overlay
    modal: true
    title: "Open a new Workspace"
    standardButtons:  Dialog.Open | Dialog.Cancel

    onAccepted: {
        if (_internal.selected_workspace) {
            window.add_workspace(_internal.selected_workspace.source);
            _self.close();
        }
    }

    onOpened: {
        _list_view.currentIndex = 0;
        _workspace_search_bar.text = "";
        _workspace_search_bar.forceActiveFocus();
    }

    G.Gutter {
        id: _workspace_selection_panel;

        width: _self.width / 3;

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.margins: G.Style.smallPadding;

        TextField {

            id: _workspace_search_bar;

            height: G.Style.formDelegateHeight

            anchors.right: _workspace_selection_panel.right;
            anchors.left: _workspace_selection_panel.left;
            anchors.top: _workspace_selection_panel.top;

            onTextChanged: {
                _model.refilter();
                _list_view.currentIndex = _model.items.count > 0 ? _model.items.get(0).model.index : -1
            }

            onAccepted: {
                _self.onAccepted()
            }

            Keys.onPressed: {
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
                icon: G.Icons.icons["text-search"];
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

            model: X.FilterProxyModel {
                id: _model;

                model: _available_workspaces;

                delegate: G.ListItemDelegate {
                    width: _list_view.width
                    highlighted: _list_view.currentIndex == index

                    text: model.title;
                    font: G.Style.fonts.cardLabel
                    enabled: model.available

                    onClicked: {
                        _list_view.currentIndex = index
                    }

                    onDoubleClicked: {
                        _list_view.currentIndex = index
                        window.add_workspace(model.source);
                        _self.close();
                    }

                    G.Icon {
                        icon: G.Icons.icons["cancel"];
                        color: G.Style.colors.dangerColor; // "red"
                        visible: !model.available

                        anchors.right: parent.right;
                        anchors.rightMargin: G.Style.smallPadding;
                        anchors.verticalCenter: parent.verticalCenter;
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
        plugins : _internal.selected_workspace ? _internal.workspace_plugins[_internal.selected_workspace.type] : []

    }

    ListModel {
        id: _available_workspaces

        ListElement {
            type: "gnomonWorkspaceBinarization"
            title: "Binarization"
            source: "qrc:/qml/gnomonQuick/Workspaces/WorkspaceBinarization.qml"
            available: true
        }
        ListElement {
            type: "gnomonWorkspaceBrowser"
            title: "Browsing"
            source: "qrc:/qml/gnomonQuick/Workspaces/WorkspaceBrowsing.qml"
            available: true
        }
        ListElement {
            type: "gnomonWorkspaceCellImageQuantification"
            title: "Cell Image Quantification"
            source: "qrc:/qml/gnomonQuick/Workspaces/WorkspaceCellImageQuantification.qml"
            available: true
        }
        ListElement {
            type: "gnomonWorkspaceCellImageTracking"
            title: "Cell Image Tracking"
            source: "qrc:/qml/gnomonQuick/Workspaces/WorkspaceCellImageTracking.qml"
            available: true
        }
        ListElement {
            type: "gnomonWorkspaceMorphonet"
            title: "MorphoNet"
            source: "qrc:/qml/gnomonQuick/Workspaces/WorkspaceMorphonet.qml"
            available: true
        }
        ListElement {
            type: "gnomonWorkspacePreprocess"
            title: "Preprocessing"
            source: "qrc:/qml/gnomonQuick/Workspaces/WorkspacePreprocess.qml"
            available: true
        }
        ListElement {
            type: "gnomonWorkspacePythonAlgorithm"
            title: "Python Algorithm"
            source: "qrc:/qml/gnomonQuick/Workspaces/WorkspacePythonAlgorithm.qml"
            available: true
        }
        ListElement {
            type: "gnomonWorkspaceRegistration"
            title: "Registration"
            source: "qrc:/qml/gnomonQuick/Workspaces/WorkspaceRegistration.qml"
            available: true
        }
        ListElement {
            type: "gnomonWorkspaceSegmentation"
            title: "Segmentation"
            source: "qrc:/qml/gnomonQuick/Workspaces/WorkspaceSegmentation.qml"
            available: true
        }
    }

    function has_loaded_plugins(index) {
        const workspace = _available_workspaces.get(index)
        const plugins = _internal.workspace_plugins[workspace.type]
        return plugins.length > 0;
    }

    Component.onCompleted: {
        _internal.workspace_groups["gnomonWorkspaceBinarization"] = "binaryImageFromImage"
        _internal.workspace_groups["gnomonWorkspaceBrowser"] = ["binaryImageReader", "cellImageReader", "cellComplexReader", "imageReader", "meshReader", "pointCloudReader"]
        _internal.workspace_groups["gnomonWorkspaceCellImageQuantification"] = "cellImageQuantification"
        _internal.workspace_groups["gnomonWorkspaceMorphonet"] = ""
        _internal.workspace_groups["gnomonWorkspaceCellImageTracking"] = "cellImageTracking"
        _internal.workspace_groups["gnomonWorkspacePreprocess"] = "imageFilter"
        _internal.workspace_groups["gnomonWorkspacePythonAlgorithm"] = "formAlgorithm"
        _internal.workspace_groups["gnomonWorkspaceRegistration"] = "imageRegistration"
        _internal.workspace_groups["gnomonWorkspaceSegmentation"] = "cellImageFromImage"

        for (var i=0; i<_available_workspaces.count; i++) {
            let w = _available_workspaces.get(i)
            _available_workspaces.setProperty(i, "section", w.title[0])
            let desc = GM.MetaData.workspaceMetaData(w.type, "description")
            _available_workspaces.setProperty(i, "description", desc)
            let plugins = []
            if (w.type in _internal.workspace_groups) {
                plugins = GM.MetaData.pluginGroupMetaData(_internal.workspace_groups[w.type])
                for(var p=0; p<plugins.length; p++) {
                    _internal.plugin_workspace[plugins[p]] = w.type
                }
            }
            _internal.workspace_plugins[w.type] = plugins
            _available_workspaces.setProperty(i, "available", (plugins.length > 0 || w.type === "gnomonWorkspacePythonAlgorithm" || w.type === "gnomonWorkspaceMorphonet"))
        }
    }
}
