import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

import gnomon.MetaData 1.0 as GM


G.Dialog {
    id: _self

    property var parameters
    property var d

    parent: Overlay.overlay

    x: Math.round((window.width - width) / 2)
    y: Math.round((window.height - height) / 2)

    width: G.Style.hugeDialogWidth
    height: G.Style.hugeDialogHeight

    focus: true
    modal: true

    // header.height: G.Style.largeLabelHeight
    // footer.visible: false

    standardButtons:  Dialog.Ok | Dialog.Cancel

    title: "Edit Parameters"
    padding: 0;

    onOpened : {
        if (_group_list.model.count > 0) {
            _group_list.currentIndex = 0
        }
    }

    G.Gutter {
        id: _group_panel;

        width: _self.width / 5;

        anchors.top: parent.top;
        anchors.bottom: _new_group_delegate.top;
        anchors.left: parent.left;
        anchors.margins: 0;
        anchors.leftMargin: G.Style.smallPadding

        Label {
            id: _group_title;

            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.right: parent.right;
            height: G.Style.mediumLabelHeight
            anchors.margins: G.Style.smallPadding

            horizontalAlignment: Text.AlignLeft;

            text: "Groups";
            color: G.Style.colors.textColorNeutral
            font: G.Style.fonts.header;
        }

        ListView {
            id: _group_list

            anchors.top: _group_title.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            model: _self.parameters

            clip: true
            focus: true;
            currentIndex: -1

            function indexOfGroup(group) {
                for(let i=0; i<count; i++) {
                    if(itemAtIndex(i).text === group) {
                        return i
                    }
                }
            }

            delegate: G.ListItemDelegate {
                id: _group_delegate

                height: G.Style.largeButtonHeight
                width: _group_list.width

                required property var parameters;
                required property string group;
                required property int index

                highlighted: _group_list.currentIndex == index

                text: group
                font: G.Style.fonts.cardLabel

                onClicked: {
                    _group_list.currentIndex = index
                }

                DropArea {
                    id: _group_drop;

                    anchors.fill: parent;

                    G.IconButton {
                        iconName: "arrow-down-drop-circle";
                        size: G.Style.largeButtonHeight
                        color: _group_delegate.highlighted? G.Style.colors.baseColor : G.Style.colors.fgColor;
                        visible: _group_drop.containsDrag;
                        anchors.centerIn: parent;
                    }

                    onDropped: (drop) => {
                        let source_index = _group_list.currentIndex
                        let source_group = _group_list.model.get(source_index).group

                        if (source_group != group) {
                            let param = drag.source.param

                            console.log("Move parameter", param.label, "from group", source_group, "to group", group)
                            if(_all_parameter_list.itemAtIndex(_group_list.indexOfGroup(source_group)).count === 1) {
                                d.setGroup(param.label, group)
                                d.removeGroup(source_group)
                            } else {
                                d.setGroup(param.label, group)
                            }

                            drop.accept()
                        }
                    }
                }
            }

            ScrollBar.vertical: ScrollBar { visible: _group_list._delegate_contentHeight > _group_list.height; }
        }
    }

    G.ListItemDelegate {
        id: _new_group_delegate

        anchors.bottom: parent.bottom
        anchors.left: _group_panel.left
        anchors.right: _group_panel.right

        implicitHeight: G.Style.largeButtonHeight
        hoverColor: G.Style.colors.fgColor

        highlighted: _new_group_drop.containsDrag;

        text: "New Group";
        font: G.Style.fonts.cardLabel

        DropArea {
            id: _new_group_drop;

            anchors.fill: parent;

            G.IconButton {
                anchors.centerIn: parent;

                iconName: "arrow-down-drop-circle";
                size: G.Style.largeButtonHeight
                color: G.Style.colors.baseColor;
                visible: _new_group_drop.containsDrag;
            }

            onDropped: (drop) => {
                _new_group_dialog.name = ""
                _new_group_dialog.param = drag.source.param
                _new_group_dialog.open()
                drop.accept()
            }
        }
    }

    G.Gutter {
        id: _parameter_panel;

        anchors.top: parent.top;
        anchors.bottom: _new_parameter_delegate.top
        anchors.left: _group_panel.right;
        anchors.leftMargin: 2;
        z: 1

        width: _self.width / 5;

        Label {
            id: _parameter_title;

            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.right: parent.right;

            height: G.Style.mediumLabelHeight
            anchors.margins: G.Style.smallPadding

            horizontalAlignment: Text.AlignLeft;

            text: "Parameters";
            color: G.Style.colors.textColorNeutral
            font: G.Style.fonts.header;
        }

        ListView {
            id: _all_parameter_list

            anchors.top: _parameter_title.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            clip: true;
            focus: true;
            interactive: false

            model: _self.parameters

            delegate: ListView {
                id: _parameter_list

                height: visible? _all_parameter_list.height : 0
                width: _all_parameter_list.width

                required property var parameters;
                required property string group;
                required property int index

                visible: _group_list.currentIndex === index
                interactive: false
                currentIndex: -1

                model: parameters

                onCurrentIndexChanged: {
                    if (currentIndex !== -1) {
                        _parameter_config_panel.component = undefined
                        _parameter_config_panel.param = model.get(currentIndex).param
                        _parameter_config_panel.component = model.get(currentIndex).component
                    }
                }

                onVisibleChanged: {
                    currentIndex = visible? 0 : -1
                }

                delegate: G.ListItemDelegate {
                    id: _parameter_delegate

                    height: G.Style.largeButtonHeight
                    width: _parameter_list.width

                    highlighted: _parameter_list.currentIndex === index

                    required property var param;
                    required property int index;

                    text: param.label;

                    Drag.active: _parameter_drag.active
                    Drag.hotSpot.x: width / 2
                    Drag.hotSpot.y: height / 2
                    Drag.dragType: Drag.Automatic

                    DragHandler {
                        id: _parameter_drag

                        xAxis.minimum: _parameter_delegate.x
                        xAxis.maximum: _parameter_delegate.x
                        yAxis.minimum: _parameter_delegate.y
                        yAxis.maximum: _parameter_delegate.y

                        onActiveChanged : {
                            if(active) {
                                _parameter_delegate.Drag.mimeData = {"text/plain" : param.label};
                                parent.grabToImage(function(result) {
                                    _parameter_delegate.Drag.imageSource = result.url;
                                })
                            }
                        }
                    }

                    onClicked: {
                        _parameter_list.currentIndex = index
                    }

                    G.IconButton {
                        id: _remove_parameter_button;

                        iconName: "close";
                        size: G.Style.iconSmall
                        tooltip: "Remove parameter";

                        hoverColor : G.Style.colors.dangerColor;

                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.margins: G.Style.smallPadding

                        onClicked: {
                            _remove_parameter_dialog.param = param;
                            _remove_parameter_dialog.open()
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar { visible: _parameter_list.contentHeight > _parameter_list.height; }
            }
        }
    }

    G.ListItemDelegate {
        id: _new_parameter_delegate

        anchors.bottom: parent.bottom
        anchors.left: _parameter_panel.left
        anchors.right: _parameter_panel.right

        implicitHeight: G.Style.largeButtonHeight

        text: "New Parameter";
        font: G.Style.fonts.cardLabel

        onClicked: {
            _new_parameter_dialog.name = ""
            _new_parameter_dialog.type_index = 0
            _new_parameter_dialog.new_group = _group_list.count === 0
            _new_parameter_dialog.group_name = ""
            _new_parameter_dialog.open()
        }

        G.IconButton {
            id: _new_parameter_button;

            iconName: "plus";
            size: G.Style.iconMedium;
            tooltip: "Add a new parameter";

            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: G.Style.smallPadding

            onClicked: {
                _new_parameter_dialog.name = ""
                _new_parameter_dialog.type_index = 0
                _new_parameter_dialog.open()
            }
        }
    }

    G.ParameterCreationDialog {
        id: _new_parameter_dialog
        force_new_group: _group_list.count === 0
        onAccepted: {
            if(new_group) {
                d.addParameter(name, type, group_name)
            } else {
                d.addParameter(name, type, _group_list.currentItem.group)
            }
        }
    }

    G.ParameterCreationDialog {
        id: _new_group_dialog

        property var param

        isGroup: true

        onAccepted: {
            let group = _new_group_dialog.name
            let source_group = param.group
            console.log("Move parameter", param.label, "from group", source_group, "to NEW group", group)
            if(_all_parameter_list.itemAtIndex(_group_list.indexOfGroup(source_group)).count === 1) {
                d.setGroup(param.label, group)
                d.removeGroup(source_group)
            } else {
                d.setGroup(param.label, group)
            }
        }
    }

    G.SimpleDialog {
        id: _remove_parameter_dialog

        property var param;

        message: "Do you really want to remove the parameter \"" + param.label + "\"?"
        caption: "Removing a parameter does not remove its occurrences in the code. It may end up generating a lot of errors."

        onAccepted: {
            let source_group = param.group
            if(_all_parameter_list.itemAtIndex(_group_list.indexOfGroup(source_group)).count === 1) {
                d.removeParameter(param.label)
                d.removeGroup(source_group)
            } else {
                d.removeParameter(param.label)
            }
        }
    }

    G.ParameterConfigurationPanel {
        id: _parameter_config_panel;

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: _parameter_panel.right;
        anchors.right: parent.right;
        anchors.margins: G.Style.smallPadding;

        param: undefined
        component: undefined

        visible: param !== undefined
    }
}
