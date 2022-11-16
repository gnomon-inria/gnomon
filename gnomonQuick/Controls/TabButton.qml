import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

TabButton {
    id: _self
    text: ""
    contentItem: Label {
        anchors.fill: parent

        font: G.Style.fonts.header;
        color: G.Style.colors.textColorBase;
        text: _self.text;
        verticalAlignment: Text.AlignVCenter;
        horizontalAlignment: Text.AlignHCenter;
    }
    background: Rectangle {
        anchors.fill: parent
        color: G.Style.colors.fgColor
    }
}
