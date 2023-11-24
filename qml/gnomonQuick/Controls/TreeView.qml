import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomon.Project as GP

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

ListView {
    id: _list_view

    property var data_paths : []
    property var _delegate_heights: []

    signal fileDoubleClicked(string fileUrl)
    signal fileRightClicked(string fileUrl)

    implicitHeight: G.Style.smallPanelHeight
    interactive: false

    model: data_paths

    delegate: TreeView {
        id: _tree_view
        
        required property string modelData
        property int rootRow: 0
        property int _index: -1

        width: _list_view.width
        implicitHeight: (_tree_view.rows - _tree_view.rootRow) * G.Style.smallButtonHeight

        interactive: false
        clip: true
        
        model : GP.FileSystemModel {
            id : _model
            rootDir: modelData
        }

        delegate: TreeViewDelegate {
            indentation: G.Style.smallPadding
            hoverEnabled: true;

            implicitWidth: _list_view.width
            implicitHeight: model.filePath.includes(_model.rootDir) ? G.Style.smallButtonHeight : 0.01

            enabled: model.filePath.includes(_model.rootDir)
            visible: model.filePath.includes(_model.rootDir)

            contentItem : Label {
                anchors.left:  _indicator.right;
                text: model.display
                font: hasChildren? G.Style.fonts.subHeader : G.Style.fonts.value
            }

            G.ToolTip {
                text: model.fileName;
                visible: _mouse_area.containsMouse
                delay: 500
            }

            MouseArea {
                id: _mouse_area

                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.NoButton
            }

            indicator: G.IconButton {
               id: _indicator;

               x: ((depth - _tree_view.rootRow) * indentation)

               anchors.verticalCenter: parent.verticalCenter;
               anchors.rightMargin: G.Style.smallPadding;

               size: G.Style.iconSmall;
               iconName: expanded? "chevron-down" : "chevron-right"

               onClicked: {
                   _tree_view.toggleExpanded(row)
               }
            }

            TapHandler {
                acceptedButtons: Qt.RightButton
                onTapped : _list_view.fileRightClicked(model.filePath)
            }

            onDoubleClicked : {
                _list_view.fileDoubleClicked(model.filePath)
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