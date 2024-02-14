import QtQuick
import QtQuick.Controls

import gnomonQuick.Style as G

Item {
    id: _self
    
    property var target: parent
    
    property alias left_x: _top_left_handle.x
    property alias top_y: _top_left_handle.y
    property alias right_x: _bottom_right_handle.x
    property alias bottom_y: _bottom_right_handle.y
    
    anchors.fill: target

    Rectangle {
        id: _selection_rectangle

        anchors.left: _top_left_handle.horizontalCenter
        anchors.right: _bottom_right_handle.horizontalCenter
        anchors.top: _top_left_handle.verticalCenter
        anchors.bottom: _bottom_right_handle.verticalCenter

        visible: project_thumbnail_dialog.thumbnail != "" & !project_thumbnail_dialog.thumbnail.startsWith("qrc")

        width: G.Style.mediumPanelWidth
        height: G.Style.mediumPanelHeight

        border.width: G.Style.borderWidth
        border.color: G.Style.colors.baseColor
        color: G.Style.colors.transparent
    }

    Rectangle {
        id: _top_left_handle

        x: -width/2
        y: -width/2

        width: 2*G.Style.smallPadding
        height: 2*G.Style.smallPadding
        radius: G.Style.smallPadding
        color: G.Style.colors.G.Style.colors.baseColor

        visible: project_thumbnail_dialog.thumbnail != "" & !project_thumbnail_dialog.thumbnail.startsWith("qrc")

        Drag.active: _top_left_drag_area.drag.active

        onXChanged: _bottom_left_handle.x = _top_left_handle.x
        onYChanged: _top_right_handle.y = _top_left_handle.y

        MouseArea {
            id: _top_left_drag_area
            anchors.fill: parent
            drag.target: parent
            drag.minimumX: -width/2
            drag.maximumX: _top_right_handle.x-width/2
            drag.minimumY: -width/2
            drag.maximumY: _bottom_left_handle.y-width/2
            hoverEnabled: true
        }
    }

    Rectangle {
        id: _top_right_handle
        x: parent.width+width/2
        y: -width/2

        width: 2*G.Style.smallPadding
        height: 2*G.Style.smallPadding
        radius: G.Style.smallPadding
        color: G.Style.colors.G.Style.colors.baseColor

        visible: project_thumbnail_dialog.thumbnail != "" & !project_thumbnail_dialog.thumbnail.startsWith("qrc")

        Drag.active: _top_right_drag_area.drag.active

        onXChanged: _bottom_right_handle.x = _top_right_handle.x
        onYChanged: _top_left_handle.y = _top_right_handle.y

        MouseArea {
            id: _top_right_drag_area
            anchors.fill: parent
            drag.target: parent
            drag.minimumX: _top_left_handle.x+width/2
            drag.maximumX: target.width+width/2
            drag.minimumY: -width/2
            drag.maximumY: _bottom_right_handle.y-width/2
            hoverEnabled: true
        }
    }

    Rectangle {
        id: _bottom_left_handle
        x: -width/2
        y: parent.height+width/2

        width: 2*G.Style.smallPadding
        height: 2*G.Style.smallPadding
        radius: G.Style.smallPadding
        color: G.Style.colors.G.Style.colors.baseColor

        visible: project_thumbnail_dialog.thumbnail != "" & !project_thumbnail_dialog.thumbnail.startsWith("qrc")

        Drag.active: _bottom_left_drag_area.drag.active

        onXChanged: _top_left_handle.x = _bottom_left_handle.x
        onYChanged: _bottom_right_handle.y = _bottom_left_handle.y

        MouseArea {
            id: _bottom_left_drag_area
            anchors.fill: parent
            drag.target: parent
            drag.minimumX: -width/2
            drag.maximumX: _bottom_right_handle.x-width/2
            drag.minimumY: _top_left_handle.y+width/2
            drag.maximumY: target.height+width/2
            hoverEnabled: true
        }
    }

    Rectangle {
        id: _bottom_right_handle
        x: parent.width+width/2
        y: parent.height+width/2

        width: 2*G.Style.smallPadding
        height: 2*G.Style.smallPadding
        radius: G.Style.smallPadding
        color: G.Style.colors.G.Style.colors.baseColor

        visible: project_thumbnail_dialog.thumbnail != "" & !project_thumbnail_dialog.thumbnail.startsWith("qrc")

        Drag.active: _bottom_right_drag_area.drag.active

        onXChanged: _top_right_handle.x = _bottom_right_handle.x
        onYChanged: _bottom_left_handle.y = _bottom_right_handle.y

        MouseArea {
            id: _bottom_right_drag_area
            anchors.fill: parent
            drag.target: parent
            drag.minimumX: _bottom_left_handle.x+width/2
            drag.maximumX: target.width+width/2
            drag.minimumY: _top_right_handle.y+width/2
            drag.maximumY: target.height+width/2
            hoverEnabled: true
        }
    }

    function resetSelection() {
        _top_left_handle.x = 0
        _top_left_handle.y = 0
        _top_right_handle.x = target.width
        _top_right_handle.y = 0
        _bottom_left_handle.x = 0
        _bottom_left_handle.y = target.height
        _bottom_right_handle.x = target.width
        _bottom_right_handle.y = target.height
    }
}