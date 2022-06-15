import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G

ComboBox {

    id: _control

    required property var param

    //HAD TO BRING THIS IN CAUSE THE BACKEND IS NOT CONSISTENT
    //AND DEPENDING ON WHETHER IT IS A LUT OR COLORMAP THE ACCESSORS
    //ARE NOT THE SAME
    property bool lut: false

    implicitWidth: G.Style.smallPanelWidth
    implicitHeight: G.Style.comboBoxHeight

    model: _control.param.availableCluts
    currentIndex: _control.lut ? _control.model.indexOf(param.colorMapName) : _control.model.indexOf(param.identifier)

    background: Rectangle {
        color: "transparent" //G.Style.colors.bgColor
    }

    delegate: G.ComboBoxDelegate {
        width: _control.width
        text: modelData
        highlighted: _control.highlightedIndex === index
    }

    indicator: G.Icon {

        id: _indicator

        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: G.Style.sizes.s2
        // anchors.leftMargin: G.Style.sizes.s6

        size: G.Style.iconSmall
        icon: G.Icons.icons["apple-keyboard-control"]

        rotation: _control.down ? 0 : 180

        Behavior on rotation {
            NumberAnimation { duration: 200 }
        }
    }

    contentItem: Rectangle {

        anchors.left: _control.left
        //anchors.right: _indicator.left

      
        height: _control.height
        radius: G.Style.panelRadius

        gradient: G.ClutGradient {

            id: _gradient

            cmap: _control.lut ? param.colorMap : param.value
        }
    }

    //TODO: MAKE A BETTER LOOKING POPUP
    popup: Popup {
        y: _control.height - 1
        width: _control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {

            clip: true
            spacing: 2;
            implicitHeight: contentHeight
            model: _control.popup.visible ? _control.delegateModel : null
            currentIndex: _control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            color: G.Style.colors.gutterColor
            radius: G.Style.panelRadius
        }
    }

    //HAD TO ADD ANOTHER CONNECTION IN CASE ITS A LUT
    Connections {
        target: _control.param
        function onValueChanged(cmap) {
            // TODO: Find a better way to ensure gradient refresh
            _gradient.orientation = Gradient.Vertical
            _gradient.orientation = Gradient.Horizontal
        }
        function onColorMapChanged(cmap) {
            // TODO: Find a better way to ensure gradient refresh
            _gradient.orientation = Gradient.Vertical
            _gradient.orientation = Gradient.Horizontal
        }
    }
}
