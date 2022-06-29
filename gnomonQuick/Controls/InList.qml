import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {
	id: _control

	required property var param;
    //for crossParameters
    readonly property var paramType: _control.param ? _control.param.type : undefined

    implicitHeight: _combobox.implicitHeight

    width: G.Style.smallPanelWidth

    background: Rectangle{
        color: "transparent"
    }

    hoverEnabled: true

    G.ToolTip {
        visible: _control.hovered && !_combobox.down
        text: param ? param.doc : ""
    }

    G.ComboBoxWithLabel {

        id: _combobox
        anchors.top: _control.top
        anchors.left: _control.left
        anchors.right: _control.right

		label: _control.param ? _control.param.label.toUpperCase() : ""

        model: _control.param ? _control.param.list : []
        currentIndex: _control.param ? param.index : -1

        onCurrentIndexChanged: {
			if(param)
				param.index = _combobox.currentIndex
        }
    }

}
