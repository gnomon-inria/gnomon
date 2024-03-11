import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Style as G

Control {
    id: _control

    required property int to
    required property int value
    property bool progressValueCentered: true

    Rectangle {
        id: rect_bg

        anchors {
            left: _control.left
            right: _control.right
            verticalCenter: _control.verticalCenter
        }
        height: _control.height
        radius: rect_bg.height * 0.5

        color: G.Style.colors.gutterColor
    }

    Rectangle {
        id: id_rec2

        anchors {
            left: rect_bg.left
            top: rect_bg.top
            bottom: rect_bg.bottom
            margins: G.Style.smallPadding
        }
        width: _control.value * (rect_bg.width - 2*rect_bg.height*0.1) / _control.to
        radius: rect_bg.radius * 0.5

        color: G.Style.colors.neutralColor
    }

    Text {
         id: id_curPercentCenter
         anchors {
             centerIn: rect_bg
             margins: id_rec2.height * 0.08
         }
        text: _control.value + " %"
        color: G.Style.colors.textColorBase
        font: G.Style.fonts.value
        //font.pixelSize: rect_bg.height * 0.5
        visible: progressValueCentered
    }

    Text {
        id: id_curPercentRight
        anchors {
            right: rect_bg.right
            verticalCenter: rect_bg.verticalCenter
            margins: G.Style.mediumPadding
        }
        text: _control.value + " %"
        color: G.Style.colors.textColorBase
        font: G.Style.fonts.value
        //font.pixelSize: rect_bg.height * 0.5
        visible: !progressValueCentered
    }

}
