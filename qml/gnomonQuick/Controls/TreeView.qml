import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomon.Project as GP

import gnomonQuick.Style as G

ListView {
    id: _list_view

    property var data_paths : []

    // implicitHeight: contentHeight
    implicitHeight: G.Style.smallPanelHeight
    implicitWidth: G.Style.smallPanelWidth

    model: data_paths

    delegate: TreeView {
        id: _tree_view
        
        required property string modelData
        property int rootRow: 0

        width: _list_view.width
        height: (_tree_view.rows - _tree_view.rootRow) * G.Style.smallLabelHeight

        interactive: false
        clip: true
        
        model : GP.FileSystemModel {
            id : _model
            rootDir: modelData
        }

        delegate: TreeViewDelegate {
            indentation: 1.5
            implicitHeight: model.filePath.includes(_model.rootDir) ? 20 : 0.01
            enabled: model.filePath.includes(_model.rootDir)
            opacity: model.filePath.includes(_model.rootDir) ? 1 : 0
            contentItem : Label {
                text: model.display
            }
        }

        Component.onCompleted : {
            _tree_view.rootRow = _model.rootDir.split('/').length - 1
            _tree_view.expandRecursively()
            _tree_view.collapseRecursively(_tree_view.rootRow)
        }
    }

}