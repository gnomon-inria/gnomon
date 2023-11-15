import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.platform
import Qt.labs.folderlistmodel
import QtCore

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

ListView {

    id: _self
    required property var foldermodel;

    clip: true;

    Item {
        id: _internal;
        property int selected: -1;
    }

    signal folderClicked(string folderUrl)
    signal fileClicked(string fileUrl)
    signal folderDoubleClicked(string folderUrl)
    signal fileDoubleClicked(string fileUrl)
    signal fileRightClicked(string fileUrl)

    /* header: RowLayout { */
    /*     id: _header; */

    /*     anchors.left: _self.left; */
    /*     anchors.right: _self.right; */

    /* } */

    model: foldermodel
    delegate: G.FinderListDelegate {
        width: _self.width

        flickable: _self
        isSelected: index === _internal.selected;

        onDoubleClicked: () => {
            if(fileIsDir) {
                _self.folderDoubleClicked(fileUrl)
            }
            else {
                _self.fileDoubleClicked(fileUrl)
            }
        }
        onClicked: (mouse) => {
            _internal.selected = index;
            if(fileIsDir) {
                _self.folderClicked(fileUrl)
            }
            else {
                if(mouse.button == Qt.RightButton) {
                  _self.fileRightClicked(fileUrl)
                } else {
                  _self.fileClicked(fileUrl)
                }
            }
        }
    }

    ScrollBar.vertical: ScrollBar { visible: _self.contentHeight > _self.height; }
}
