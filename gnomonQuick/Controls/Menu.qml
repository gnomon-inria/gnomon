import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G

Menu {
    id: _control

    width: G.Style.smallPanelWidth

    closePolicy: Popup.CloseOnPressOutsideParent

    background: Rectangle {
        anchors.fill: parent
        radius: G.Style.panelRadius
        color: G.Style.colors.bgColor
    }

    delegate: G.MenuItemDelegate {}

}
