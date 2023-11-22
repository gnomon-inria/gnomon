import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomon.Project as GP

ListView {


    id: _list_view

    model: ["/Users/ksamassa/Desktop/buffer/yy", "/Users/ksamassa/Desktop/buffer/y"]

    delegate: TreeView {
        id: _tree_view
        width: _list_view.width
        height: _tree_view.rows * 20

        interactive: false
        clip: true
        
        required property string modelData

        model : GP.FileSystemModel {
            id : _m
            rootDir: modelData
        }
        delegate: TreeViewDelegate {
            indentation: 1.5
            implicitHeight: 20
            contentItem : Label {
                text: model.display
            }
        }
    }

}