import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X

Slider {
    id: _ts_slider
    value: times.length > 0 ? times[0] : 0
    from: 0 
    to: times.length-1
    stepSize: 1
    snapMode: Slider.SnapAlways

    property var times
    readonly property int tickWidth: 2 
    
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.bottom: parent.bottom 
    anchors.margins: 6

    background: Rectangle {
        color: "#00000000";
        radius: _ts_slider.handle.width/2
        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
            height: 2
            color: X.Style.backgroundColor

        }

        Repeater {
            model: _ts_slider.times.length
            delegate: Rectangle {
                x:  _ts_slider.handle.width/2 + index*(_ts_slider.background.width - _ts_slider.handle.width)/(_ts_slider.to - _ts_slider.from) - _ts_slider.tickWidth/2
                implicitWidth: _ts_slider.tickWidth
                implicitHeight: parent.height
                color:X.Style.accentColor;
            }
        }
    }

    ToolTip {
        parent: _ts_slider.handle
        visible: _ts_slider.pressed
        text: _ts_slider.times[_ts_slider.value]
    }

    handle: Rectangle {
        x: _ts_slider.leftPadding + _ts_slider.visualPosition * (_ts_slider.availableWidth - width)
        implicitWidth: 14
        implicitHeight: 14
        radius: 7
    }


}
