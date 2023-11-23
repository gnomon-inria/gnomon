import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomon.Project as GP

import gnomonQuick.Style as G

ListView {
    id: _list_view

    property var data_paths : []
    property var _delegate_heights: []

    implicitHeight: G.Style.smallPanelHeight

    model: data_paths

    delegate: TreeView {
        id: _tree_view
        
        required property string modelData
        property int rootRow: 0
        property int _index: -1

        width: _list_view.width
        implicitHeight: (_tree_view.rows - _tree_view.rootRow) * G.Style.smallLabelHeight

        interactive: false
        clip: true
        
        model : GP.FileSystemModel {
            id : _model
            rootDir: modelData
        }

        delegate: TreeViewDelegate {
            indentation: G.Style.tinyPadding
            implicitWidth: _list_view.width
            implicitHeight: model.filePath.includes(_model.rootDir) ? G.Style.smallLabelHeight : 0.01
            enabled: model.filePath.includes(_model.rootDir)
            visible: model.filePath.includes(_model.rootDir)
            contentItem : Label {
                text: model.display
            }
        }

        onHeightChanged : {
            if (_index >= 0) {
                _delegate_heights[_index] = height
                _list_view.implicitHeight = _delegate_heights.reduce((s_v, v) => s_v + v, 0);
            }
        }

        Component.onCompleted : {
            _tree_view.rootRow = _model.rootDir.split('/').length - 1
            _tree_view.expandRecursively()
            _tree_view.collapseRecursively(_tree_view.rootRow)
            _tree_view._index = _delegate_heights.length

            _delegate_heights.push(_tree_view.height)
        }
    }
}