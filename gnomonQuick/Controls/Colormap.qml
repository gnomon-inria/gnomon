import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {
	id: _control

	required property var param;

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

        param: _control.param

        onCurrentValueChanged: _control.param.identifier = _cluts.currentValue

    }

    Label {
        id: _value

        anchors.topMargin: G.Style.sizes.s1
        anchors.top: _cluts.bottom
        anchors.left: parent.left

        text: _cluts.currentValue
        font: G.Style.fonts.value
        color: G.Style.colors.hoveredBaseColor
    }

	background: Rectangle{
        color: "transparent"
    }

    Component.onCompleted: {
        console.log("CLUTS: " + _control.param.availableCluts)
    }

}
