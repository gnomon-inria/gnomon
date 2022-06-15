import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import xQuick.Controls 1.0 as X
import xQuick.Style    1.0 as X

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

import dtkCore

Control {
	id: _control

	required property var param;

    implicitHeight: _label.implicitHeight + _cluts.implicitHeight
        + _controls_row.implicitHeight + _controls_row.anchors.topMargin + _value.implicitHeight
    width: G.Style.smallPanelWidth

    X.ToolTip {
        visible: _cluts.hovered && !_cluts.pressed
        text: param.doc
    }

	Label {

        id: _label

        anchors.left: parent.left
        anchors.top: parent.top

        text: param.label.toUpperCase()
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
    }


    G.ColorMapSelector {
        id: _cluts

        anchors.top: _label.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        lut: true
        param: _control.param

        onCurrentValueChanged: _control.param.colorMapName = _cluts.currentValue

    }

    RowLayout {

        id: _controls_row

        anchors.top: _cluts.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: G.Style.smallPadding

        G.RangeSlider {
            id: _range

            Layout.fillWidth: true;

            first.value: param.valueMin
            second.value: param.valueMax
            from: param.from
            to: param.to

            first.onMoved: {
                param.valueMin = first.value
            }
            second.onMoved: {
                param.valueMax = second.value
            }

            Component.onCompleted: {
                console.log(param.valueMin)
                console.log(param.valueMax)
                // from = param.valueRangeMin;
                // to = param.valueRangeMax;
            }
        }

        G.CheckBox {
            id: _visible
            text: "Hide";
            //Layout.fillWidth: true;
            width: G.Style.sizes.s6

            checked: !param.visibility

            onClicked: {
                param.visibility = !_visible.checked
            }
        }

    }

    Label {
        id: _value

        anchors.topMargin: G.Style.sizes.s1
        anchors.top: _controls_row.bottom
        anchors.left: parent.left

        text: _cluts.currentValue
        font: G.Style.fonts.value
        color: G.Style.colors.hoveredBaseColor
    }

	background: Rectangle{
        color: "transparent"
    }

}
