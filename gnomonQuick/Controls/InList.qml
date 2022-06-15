import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {
	id: _control

	required property var param;
    //for crossParameters
    readonly property var paramType: _control.param.type

    implicitHeight: _label.implicitHeight + _combobox.implicitHeight

    width: G.Style.smallPanelWidth

    background: Rectangle{
        color: "transparent"
    }

    hoverEnabled: true

    G.ToolTip {
        visible: _control.hovered && !_combobox.down
        text: param.doc
    }

	Label {

        id: _label

        anchors.left: parent.left
        anchors.top: parent.top

        text: _control.param.label.toUpperCase()
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
    }

    G.ComboBox {

        id: _combobox
        anchors.top: _label.bottom
        anchors.left: _control.left
        anchors.right: _control.right

        textColor: G.Style.colors.hoveredBaseColor

        model: _control.param.list
        currentIndex: param.index

        onCurrentIndexChanged: {
			param.index = _combobox.currentIndex
        }
    }

}
