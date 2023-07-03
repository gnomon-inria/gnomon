import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import xQuick.Controls 1.0 as X
import xQuick.Style    1.0 as X

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {
    id: _control

    required property var param

    implicitHeight: _label.implicitHeight + 2*G.Style.smallPadding
    width: G.Style.smallPanelWidth

    Label {

        id: _label

        anchors.left: parent.left
        anchors.top: parent.top

        text: param.label.toUpperCase()
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
    }

    background: Rectangle{
        color: G.Style.colors.fgColor
    }

}
