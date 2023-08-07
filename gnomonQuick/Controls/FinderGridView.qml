import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import Qt.labs.platform 1.1
import Qt.labs.folderlistmodel 2.15
import Qt.labs.settings 1.0

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

GridView {

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

    model: foldermodel
    delegate: G.FinderGridDelegate {
        flickable: _self;
        isSelected: foldermodel.index === _internal.selected;
        onDoubleClicked: () => {
            if(foldermodel.fileIsDir) {
                folderDoubleClicked(foldermodel.fileUrl)
            }
            else {
                fileDoubleClicked(foldermodel.fileUrl)
            }
        }
        onClicked: (mouse) => {
            _internal.selected = foldermodel.index
            if(foldermodel.fileIsDir) {
                folderClicked(foldermodel.fileUrl)
            }
            else {
                if(mouse.button == Qt.RightButton) {
                    fileRightClicked(foldermodel.fileUrl)
                } else {
                    fileClicked(foldermodel.fileUrl)
                }
            }
        }
    }

    ScrollBar.vertical: ScrollBar { visible: _self.contentHeight > _self.height; }
}
