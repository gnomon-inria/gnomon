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
        property var param_groups: [];
    }

    spacing: 10;

    clip: true;

    Component {
        id: _section_heading;

        Rectangle {
            id: _section_rectangle;

            required property string section;

            width: _self.width;
            height: section ? childrenRect.height : 0 //_self.computeSectionHeight(section);
            z: -1;

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
                anchors.verticalCenter: parent.verticalCenter;
                anchors.leftMargin: 5;

                text: sectionTitle(parent.section);
                font.pixelSize: 12;
                font.bold: true;
            }
        }
    }

    section.property: "group"
    section.criteria: ViewSection.FullString
    section.delegate: _section_heading;
    //section.labelPositioning: ViewSection.CurrentLabelAtStart

    delegate: Loader {
        property var lparam: param;

        anchors.topMargin: 10;

        height: _self.isSectionExpanded(group) ? 70 : 0;
        width: _self.width;

        clip: true;
        z: 1;

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
        _internal.expanded = _internal.expanded.filter((item) => item !== group)
    }

    function expandSection(group) {
        _internal.expanded = _internal.expanded.concat([group])
    }

    function sectionTitle(s) {
        let title = s.replace('_', ' ');
        return title.charAt(0).toUpperCase() + title.slice(1);
    }

    function computeSectionHeight(group) {
        if(group) {
            if(_self.isSectionExpanded(group)) {
                const num_params = _internal.param_groups.filter(g => g === group).length
                return num_params * 80;
            }
            return 40;
        }
        return 0;
    }

    Component.onCompleted: {
        const groups = []

        for(let i = 0; i < _self.model.count; i++)
            if(_self.model.get(i).group)
                groups.push(_self.model.get(i).group)

        _internal.expanded = []
        _internal.param_groups = groups
    }
}
