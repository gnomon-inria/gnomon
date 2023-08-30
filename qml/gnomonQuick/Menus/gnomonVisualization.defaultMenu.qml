import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {

    id: _control

    property alias model: _list_view.model

    clip: true

    background: Rectangle {
        color: G.Style.colors.gutterColor
        radius: G.Style.panelRadius

        Rectangle {
            color: G.Style.colors.bgColor
            height: parent.height - 4
            width: parent.width - 4
            anchors.centerIn: parent

            Rectangle {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: G.Style.sizes.s1

                color: G.Style.colors.baseColor
            }

        }
    }

    signal valueChanged()

    ListView {

        id: _list_view

        anchors.fill: parent
        anchors.topMargin: 2
        anchors.rightMargin: 0
        anchors.bottomMargin: 2
        anchors.leftMargin: G.Style.sizes.s1 + G.Style.smallPadding

        height: _control.availableHeight
        width: _control.availableWidth
        spacing: G.Style.smallColumnSpacing
        clip: true;
        interactive: _list_view.contentHeight > _control.height

        delegate: G.ParameterGroup {
            required property var parameters;
            required property string group;

            title: _control.getTitleString(group)
            model: parameters
            width: _list_view.width - scroll_indicator.width
            collapsed: title != "General";

            onValueChanged: {
                _control.valueChanged()
            }
        }

        ScrollIndicator.vertical: G.ScrollIndicator {
            id: scroll_indicator

            anchors.right: parent.right
            visible: _list_view.contentHeight > _control.height
        }

    }

    function getTitleString(group : string) : string {
        return (group.charAt(0).toUpperCase() + group.slice(1)).replace('_', ' ')
    }
}
