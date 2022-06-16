import QtQuick
import QtQuick.Controls

import gnomonQuick.Style as G
import gnomonQuick.Controls as G
import gnomonQuick.Icons as G


Slider {
    id: _control

    readonly property alias gaugeWidth: _gauge.width

    implicitWidth: G.Style.controlWidth
    implicitHeight: G.Style.gutterHeight
    padding: 6

    /*rightPadding: padding + (_self.mirrored ? (down.indicator ? down.indicator.width : 0) : (up.indicator ? up.indicator.width : 0))

    gaugeControl: X.ScientificSpinBoxReal.ControlLinear
    contentItem: Control {
        id: _content

        Rectangle {
            id: _gauge

            height: parent.height
            radius: G.Style.panelRadius
            width: _content.width * _self.scaleValue();

            color: G.Style.colors.textColorBase;
        }


        MouseArea {
            id: _slider
            anchors.fill: parent
        }

    }

    up.indicator: G.IconButton {
        autoRepeat: true
        x: _self.mirrored ? 0 : parent.width - width
        y: 0;
        size: parent.height / 2

        iconName: G.Icons.icons["chevron-up"]
        onClicked: {
            _self.increase();
        }
    }


    down.indicator: G.IconButton {
        autoRepeat: true
        x: _self.mirrored ? 0 : parent.width - width
        y: height;
        size: parent.height / 2
        iconName: G.Icons.icons["chevron-down"]
        onClicked: {
            _self.decrease();
        }
    }*/

    background: Rectangle {
        id: _gutter

        color: G.Style.colors.gutterColor
    }

    contentItem: Rectangle {
        id: _gauge

        x: _control.leftPadding
        width: (_control.visualPosition) * _control.availableWidth
        height: _control.availableHeight

        radius: G.Style.panelRadius
        color: G.Style.colors.textColorBase
    }

    handle: Rectangle {
        x: _control.leftPadding + _control.visualPosition * (_control.availableWidth - width)
        y: _control.topPadding + _control.availableHeight / 2 - height / 2
        implicitHeight: _gauge.height
        implicitWidth: _gauge.height
        radius: _gauge.height / 2
        color: _control.pressed || _control.hovered ? G.Style.colors.hoveredBaseColor : G.Style.colors.textColorBase
    }

}
