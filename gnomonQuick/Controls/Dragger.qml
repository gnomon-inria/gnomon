import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Rectangle {
    id: self;

    property int orientation: Qt.Horizontal;
    property string tooltip: "";

    property int l_size: 100;
    property int s_size: 13;

    width: orientation == Qt.Horizontal ? l_size : s_size;
    height: orientation == Qt.Horizontal ? s_size : l_size;
    radius: s_size/2;

    color: G.Style.colors.gutterColor;

    border.width: 1;
    border.color: G.Style.colors.embossColorNeutral;

    signal clicked;

    Rectangle {
        id: _center_rectangle
        anchors.centerIn: parent;

         width: orientation == Qt.Horizontal
            ? parent.width - parent.height
            : s_size/4;
        height: orientation == Qt.Horizontal
            ? s_size/4
            : parent.height - parent.width;

        radius: 1;
        color: G.Style.colors.embossColorNeutral;
    }

    MouseArea {
        id: _mouse_area
        anchors.fill: parent;
        hoverEnabled: true;
        onClicked: self.clicked();

        onEntered: {
            //color = G.Style.colors.hoveredBaseColor;
            _center_rectangle.color = G.Style.colors.hoveredBaseColor;
        }

        onExited: {
            //color = G.Style.colors.gutterColor;
            _center_rectangle.color = G.Style.colors.embossColorNeutral;
        }
    }

    G.ToolTip {
        text: self.tooltip;
        visible: self.tooltip && _mouse_area.containsMouse
    }
}
