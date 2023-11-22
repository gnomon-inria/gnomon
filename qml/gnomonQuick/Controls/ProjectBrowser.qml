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

    hoverEnabled: true;

    background: Rectangle {
        color: G.Style.colors.gutterColor;
    }

    Label {
        id: _project_label

        anchors.top: _self.top
        anchors.left: _self.left
        anchors.margins: G.Style.smallPadding

        height: G.Style.mediumButtonHeight

        text: "Project : ";

        font: G.Style.fonts.value
        horizontalAlignment: Text.AlignLeft
        color: G.Style.colors.textColorNeutral
    }

    Label {
        id: _directory_label;

        anchors.top: _self.top
        anchors.left: _project_label.right
        anchors.right: _self.right
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

        anchors.top : _project_label.bottom;
        anchors.left: _self.left
        anchors.right: _self.right
        anchors.margins: G.Style.smallPadding
    }

    Label {
        id: _data_label

        anchors.top: _project_tree_view.bottom
        anchors.left: _self.left
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
        anchors.left: _self.left
        anchors.right: _self.right
        anchors.margins: G.Style.smallPadding

        visible: _self.dataPath.length > 0
    }
}