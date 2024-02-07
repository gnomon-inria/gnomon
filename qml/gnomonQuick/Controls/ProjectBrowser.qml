import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.platform
import Qt.labs.folderlistmodel
import QtCore

import gnomonQuick.Controls as G
import gnomonQuick.Style as G


Control
{
    id: _self;

    property string rootDir: "/";
    property var dataPath: ["/"];

    property alias projectTree: _project_tree_view

    signal fileDoubleClicked(string fileUrl)
    signal fileRightClicked(string fileUrl)

    background: Rectangle {
        color: G.Style.colors.gutterColor;
    }

    ScrollView {
        anchors.fill: parent
        anchors.margins: G.Style.smallPadding

        clip: true
        contentWidth: availableWidth
        contentHeight: 2*G.Style.mediumButtonHeight + 4*G.Style.smallPadding + _project_tree_view.height + _data_tree_view.height

        Label {
            id: _project_label

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: G.Style.smallPadding

            height: G.Style.mediumButtonHeight

            text: "Project : ";

            font: G.Style.fonts.value
            horizontalAlignment: Text.AlignLeft
            color: G.Style.colors.textColorNeutral
        }

        Label {
            id: _directory_label;

            anchors.top: parent.top
            anchors.left: _project_label.right
            anchors.right: parent.right
            anchors.margins: G.Style.smallPadding

            height: G.Style.mediumButtonHeight

            text: _self.rootDir;

            font: G.Style.fonts.subHeader
            horizontalAlignment: Text.AlignLeft
            color: G.Style.colors.textColorNeutral
        }

        G.TreeView {
            id: _project_tree_view

            data_paths : [_self.rootDir]
            is_project_dir : true

            anchors.top : _project_label.bottom;
            width: parent.width
            anchors.bottomMargin: G.Style.smallPadding

            onFileDoubleClicked : (fileUrl) => {
                _self.fileDoubleClicked(fileUrl)
            }
            onFileRightClicked : (fileUrl) => {
                _self.fileRightClicked(fileUrl)
            }
        }

        Label {
            id: _data_label

            anchors.top: _project_tree_view.bottom
            anchors.left: parent.left
            anchors.margins: G.Style.smallPadding

            height: G.Style.mediumButtonHeight
            visible: _self.dataPath.length > 0

            text: "External Data : ";

            font: G.Style.fonts.value
            horizontalAlignment: Text.AlignLeft
            color: G.Style.colors.textColorNeutral
        }

        G.TreeView {
            id: _data_tree_view

            data_paths : _self.dataPath

            anchors.top : _data_label.bottom;
            width: parent.width
            anchors.bottomMargin: G.Style.smallPadding

            visible: _self.dataPath.length > 0

            onFileDoubleClicked : (fileUrl) => {
                _self.fileDoubleClicked(fileUrl)
            }
            onFileRightClicked : (fileUrl) => {
                _self.fileRightClicked(fileUrl)
            }
        }

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical: ScrollBar {
            id: _scrollbar

            policy: ScrollBar.AsNeeded
            anchors.right: parent.right;
            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
        }
    }
}