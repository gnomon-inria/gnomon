import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X

Slider {
    id: _ts_slider
    from: 0
    to: asViewLogic.timeMax
    value: 0
    stepSize: 1
    snapMode: Slider.SnapAlways

    visible: asViewLogic.timeMax > 0

    property int tickWidth 
    property var asViewLogic
    property var asView

    anchors.right: asView.right
    anchors.left: asView.left
    anchors.bottom: asView.bottom 
    anchors.margins: 6

    background: Rectangle {
        color: X.Style.backgroundColor;
        radius: _ts_slider.handle.width/2

        Repeater {
            model: Math.round(_ts_slider.to - _ts_slider.from + 1)
            delegate: Rectangle {
                x:  _ts_slider.handle.width/2 + index*(_ts_slider.background.width - _ts_slider.handle.width)/(_ts_slider.to - _ts_slider.from) - _ts_slider.tickWidth/2
                implicitWidth: _ts_slider.tickWidth
                implicitHeight: parent.height
                color:X.Style.accentColor;
            }
        }
    }

    onValueChanged: {
        asViewLogic.currentTime = value;
    }
}
