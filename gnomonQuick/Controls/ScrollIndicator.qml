import QtQuick
import QtQuick.Controls

import gnomonQuick.Style as G
import gnomonQuick.Controls as G

ScrollIndicator {
    id: _control

    implicitWidth: G.Style.gutterWidth
    padding: 2

    background: Rectangle {
        id: _gutter

        color: G.Style.colors.gutterColor
    }

    contentItem: Rectangle {
        id: _handle

        implicitWidth: _control.availableWidth
        implicitHeight: _control.availableHeight

        radius: width
        color: G.Style.colors.embossColor
    }

}
