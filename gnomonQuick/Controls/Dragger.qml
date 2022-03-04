import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

Rectangle {
    id: self;

    property int orientation: Qt.Horizontal;

    property int l_size: 100;
    property int s_size: 12;

    width: orientation == Qt.Horizontal ? l_size : s_size;
    height: orientation == Qt.Horizontal ? s_size : l_size;
    radius: s_size/2;

    color: Qt.darker(X.Style.alternateBaseColor, 1.1);

    border.width: 1;
    border.color: X.Style.borderColor;

    signal clicked;

    Rectangle {
        anchors.centerIn: parent;

         width: orientation == Qt.Horizontal
            ? parent.width - parent.height
            : s_size/4;
        height: orientation == Qt.Horizontal
            ? s_size/4
            : parent.height - parent.width;

        radius: 2;
        color: X.Style.borderColor;
    }

    MouseArea {
        anchors.fill: parent;
        onClicked: self.clicked();
    }
}
