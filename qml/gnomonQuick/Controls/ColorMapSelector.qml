import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

ComboBox {

    id: _control

    required property var param

    //HAD TO BRING THIS IN CAUSE THE BACKEND IS NOT CONSISTENT
    //AND DEPENDING ON WHETHER IT IS A LUT OR COLORMAP THE ACCESSORS
    //ARE NOT THE SAME
    property bool lut: false

    implicitWidth: G.Style.smallPanelWidth
    implicitHeight: G.Style.comboBoxHeight

    model: param ? _control.param.availableCluts: [""]
    currentIndex: param ? _control.lut ? _control.model.indexOf(param.colorMapName) : _control.model.indexOf(param.identifier): -1

    background: Rectangle {
        color: "transparent" //G.Style.colors.bgColor
    }

    delegate: G.ComboBoxDelegate {
        width: _box.width
        text: modelData
        textStyle: Text.Outline
        highlighted: _control.highlightedIndex === index

        background: Rectangle {
            anchors.fill: parent
            //color: G.Style.colors.exampleColor

            gradient: G.ClutGradient {
                cmap: _control.param.getColorMap(modelData)
            }
        }
    }

    indicator: G.Icon {

        id: _indicator

        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: G.Style.sizes.s2
        // anchors.leftMargin: G.Style.sizes.s6

        size: G.Style.iconSmall
        icon: "apple-keyboard-control"

        rotation: _control.down ? 0 : 180

        Behavior on rotation {
            NumberAnimation { duration: 200 }
        }
    }

    contentItem: Rectangle {
        id: _box

        anchors.left: _control.left
        anchors.right: _indicator.left


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
        width: _box.width
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
