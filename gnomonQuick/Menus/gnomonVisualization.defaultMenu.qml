

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {

    id: _control

    property alias model: _list_view.model

    clip: true
    topPadding: 2
    bottomPadding: 2
    rightPadding: G.Style.mediumPadding
    leftPadding: 2

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

        //anchors.horizontalCenter: parent.horizontalCenter
        anchors.centerIn: parent

        height: _control.availableHeight
        width: _control.availableWidth
        spacing: G.Style.smallColumnSpacing
        clip: true;
        interactive: _list_view.contentHeight > _control.height

        delegate: G.ParameterGroup {
            title: _control.getTitleString(group)
            model: parameters
            width: _list_view.width - 10

            onValueChanged: {
                _control.valueChanged()
            }
        }

        ScrollIndicator.vertical: G.ScrollIndicator {
            anchors.right: parent.right
            visible: _list_view.contentHeight > _control.height
        }

    }

    function getTitleString(group : string) : string {
        return (group.charAt(0).toUpperCase() + group.slice(1)).replace('_', ' ')
    }
}
