import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Layouts    1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Slider {
    id: _ts_slider
    value: times && times.length > 0 ? times[0] : 0
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
            color: G.Style.colors.bgColor

        }

        Repeater {
            model: _ts_slider.times ? _ts_slider.times.length : 0
            delegate: Rectangle {
                x:  _ts_slider.handle.width/2 + index*(_ts_slider.background.width - _ts_slider.handle.width)/(_ts_slider.to - _ts_slider.from) - _ts_slider.tickWidth/2
                implicitWidth: _ts_slider.tickWidth
                implicitHeight: parent.height
                color:G.Style.colors.dangerColor;
            }
        }
    }

    handle: Rectangle {
        x: _ts_slider.visualPosition*(_ts_slider.width - width)
        implicitWidth: 14
        implicitHeight: 14
        radius: 7
    }

    Label {
        id: _current_time

        anchors.bottom: handle.top
        anchors.horizontalCenter: handle.horizontalCenter
        anchors.margins: G.Style.smallPadding

        width: contentWidth + G.Style.mediumPadding
        verticalAlignment: Text.AlignVCenter;
        horizontalAlignment: Text.AlignHCenter;

        text: (_ts_slider.times &&  _ts_slider.times[_ts_slider.value] !== undefined) ? _ts_slider.times[_ts_slider.value] : ""
        font: G.Style.fonts.value

        background: Rectangle {
            color: G.Style.colors.fgColor
            radius: G.Style.panelRadius

            opacity: 0.9
        }
    }

    Label {
        id: _initial_time

        anchors.bottom: handle.top
        anchors.left: _ts_slider.left
        anchors.bottomMargin: G.Style.smallPadding
        anchors.leftMargin: _ts_slider.tickWidth
        horizontalAlignment: Text.AlignHCenter;

        visible: _ts_slider.value != _ts_slider.from
        text: (_ts_slider.times &&  _ts_slider.times[_ts_slider.from] !== undefined) ? _ts_slider.times[_ts_slider.from] : ""
        font: G.Style.fonts.value
    }

    Label {
        id: _final_time

        anchors.bottom: handle.top
        anchors.right: _ts_slider.right
        anchors.bottomMargin: G.Style.smallPadding
        anchors.rightMargin: _ts_slider.tickWidth
        horizontalAlignment: Text.AlignHCenter;

        visible: _ts_slider.value != _ts_slider.to
        text: (_ts_slider.times &&  _ts_slider.times[_ts_slider.to] !== undefined) ? _ts_slider.times[_ts_slider.to] : ""
        font: G.Style.fonts.value
    }
}
