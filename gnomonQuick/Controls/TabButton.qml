import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

TabButton {
    id: _self

    font: G.Style.fonts.header
    property alias color: _label.color

    contentItem: Label {
        id: _label

        anchors.fill: parent

        font: _self.font;
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
