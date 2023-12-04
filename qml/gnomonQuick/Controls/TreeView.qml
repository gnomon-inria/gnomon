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
    property bool is_project_dir : false

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
            id: _delegate

            indentation: G.Style.smallPadding
            hoverEnabled: true;
            highlighted: (_item_selection_model.selectedIndexes.includes(_tree_view.index(index, 0)) &&
                          !_model.isDir(_tree_view.index(index, 0)))

            implicitWidth: _list_view.width
            implicitHeight: model.filePath.includes(_model.rootDir) ? G.Style.smallButtonHeight : 0.01

            enabled: model.filePath.includes(_model.rootDir)
            visible: model.filePath.includes(_model.rootDir)
            background.opacity: 0.3

            contentItem : Label {
                id: _label
                anchors.left:  _indicator.right;
                text: model.display
                font: hasChildren? G.Style.fonts.subHeader : G.Style.fonts.value

                Drag.active: _drag_handler.active
                Drag.dragType: Drag.Automatic
            }

            DragHandler {
                id: _drag_handler

                xAxis.minimum: _delegate.x
                xAxis.maximum: _delegate.x
                yAxis.minimum: _delegate.y
                yAxis.maximum: _delegate.y

                onActiveChanged : {
                    if(active) {
                        let paths = "";
                        for (let i=0; i<_item_selection_model.selectedIndexes.length; i++) {
                            let _model_index = _item_selection_model.selectedIndexes[i];
                            if (i>0) paths += ","
                            paths += _model.filePath(_model_index)
                        }
                        _label.Drag.mimeData = {"text/uri-list" : paths};
                        parent.grabToImage(function(result) {
                            _label.Drag.imageSource = result.url;
                            console.log(_label.Drag.mimeData["text/uri-list"])
                        })
                    }
                }
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

            TapHandler {
                acceptedButtons: Qt.LeftButton
                acceptedModifiers: Qt.ControlModifier
                onTapped: _item_selection_model.select(_tree_view.index(index, 0), ItemSelectionModel.Select)
            }

            TapHandler {
                acceptedButtons: Qt.LeftButton
                acceptedModifiers: Qt.NoModifier
                onTapped: _item_selection_model.select(_tree_view.index(index, 0), ItemSelectionModel.ClearAndSelect)
                onDoubleTapped: _list_view.fileDoubleClicked(model.filePath)
            }
        }

        ItemSelectionModel {
             id: _item_selection_model

             model: _model

             onSelectionChanged : {
                 console.log("selected items:", _item_selection_model.selectedIndexes)
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
            if(!_list_view.is_project_dir)
                _tree_view.collapseRecursively(_tree_view.rootRow)
            _tree_view._index = _delegate_heights.length

            _delegate_heights.push(_tree_view.height)
        }
    }
}