import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Style as G

Control {
    id: _control

    required property int to
    required property int value

    Rectangle {
        id: rect_bg

        anchors {
            left: _control.left
            right: _control.right
            verticalCenter: _control.verticalCenter
            leftMargin: G.Style.smallPadding
            rightMargin: G.Style.smallPadding
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
            margins: rect_bg.height * 0.1
        }
        width: _control.value * rect_bg.width / _control.to
        height: _control.height
        radius: rect_bg.radius * 0.5

        color: G.Style.colors.neutralColor
    }

    Text {
         id: id_curPercent
         anchors {
             centerIn: rect_bg
             margins: id_rec2.height * 0.08
         }
        text: _control.value + " %"
        color: G.Style.colors.neutralColor
        font: G.Style.fonts.value
        //font.pixelSize: rect_bg.height * 0.5
    }

}
