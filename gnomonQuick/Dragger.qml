import QtQuick            2.15
import QtQuick.Controls   2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts    1.15

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

Rectangle {
    id: self;

    width: 100;
    height: 12;
    radius: height/2;

    color: Qt.darker(X.Style.alternateBaseColor, 1.1);

    border.width: 1;
    border.color: X.Style.borderColor;

    signal clicked;

    Rectangle {
        anchors.centerIn: parent;
        width: parent.width - parent.height;
        height: 4;
        radius: 2;
        color: X.Style.borderColor;
    }

    MouseArea {
        anchors.fill: parent;
        onClicked: self.clicked();
    }
}
