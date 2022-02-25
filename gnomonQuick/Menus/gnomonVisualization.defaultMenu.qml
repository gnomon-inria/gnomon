import QtQuick              2.15
import QtQuick.Controls     2.15
import QtGraphicalEffects   1.15
import QtQuick.Layouts      1.15

import xQuick.Controls      1.0 as X
import xQuick.Fonts         1.0 as X
import xQuick.Style         1.0 as X
import xQuick.Vis           1.0 as XVis

import crossQuick        1.0 as C
import crossParameters   1.0 as C

import gnomonQuick.Controls 1.0 as G

ListView {
    id: _self;

    signal valueChanged();

    QtObject {
        id: _internal;

        property var expanded: [];

    }

    spacing: 10;

    clip: true;

    Component {
        id: _section_heading;

        Rectangle {

            id: _section_rectangle;

            required property string section;

            width: _self.width;
            height: section ? childrenRect.height : 0;

            color: X.Style.backgroundColor;
            radius: 3;

            clip: true;

            Behavior on height {
                NumberAnimation { duration: 200 }
            }

            MouseArea {
                anchors.fill: parent;

                onClicked: _self.toggleCollapse(parent.section);
            }

            X.Icon {
                id: _icon;

                anchors.right: parent.right;
                anchors.top: parent.top;
                anchors.topMargin: 5;
                anchors.rightMargin: 5;
                size: 30;

                icon: X.Icons.icons.keyboard_arrow_up;

                rotation: _self.isSectionExpanded(parent.section) ? 0 : 180;

                Behavior on rotation {
                    NumberAnimation { duration: 200 }
                }

            }

            X.Label {

                anchors.left: parent.left;
                anchors.top: parent.top;
                anchors.topMargin: 5;
                anchors.leftMargin: 5;

                text: parent.section;
                font.pixelSize: 20;
            }
        }
    }

    section.property: "group"
    section.criteria: ViewSection.FullString
    section.delegate: _section_heading;

    delegate: Loader {
        property var lparam: param;

        anchors.topMargin: 10;

        height: _self.isSectionExpanded(group) ? 70 : 0;
        width: _self.width;

        clip: true;

        sourceComponent: component;

        Connections {
            target: param
            function onValueChanged() {
                _self.valueChanged();
            }
        }

        Behavior on height {
            NumberAnimation { duration: 200 }
        }

    }

    ScrollIndicator.vertical: ScrollIndicator {
        visible: _self.contentHeight > _self.height;
    }

    function isSectionExpanded(group) {

        if(group) {
            return _internal.expanded.includes(group)
        }

        return true;
    }

    function toggleCollapse(group) {
        if(_internal.expanded.includes(group)) _self.collapseSection(group)
        else _self.expandSection(group)
    }

    function collapseSection(group) {
        console.log("collapse", group)
        _internal.expanded = _internal.expanded.filter((item) => item !== group)
    }

    function expandSection(group) {
        console.log("expand", group)
        _internal.expanded = _internal.expanded.concat([group])
    }

    Component.onCompleted: {

        const expanded = []
        for(let i = 0; i < _self.model.count; i++)
            if(_self.model.get(i).group) {
                expanded.push(_self.model.get(i).group)
            }
        _internal.expanded = expanded
    }

}
