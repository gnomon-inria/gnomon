import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

import gnomon.MetaData 1.0 as GM


G.Dialog {
    id: _self

    property var parameters

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

    G.Gutter {
        id: _group_panel;

        width: _self.width / 4;

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.margins: 0;

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
            anchors.bottom: _new_group_delegate.top
            anchors.left: parent.left
            anchors.right: parent.right

            model: _self.parameters

            clip: true
            focus: true;
            currentIndex: -1

            onCurrentIndexChanged: {
                _parameter_list.model = currentIndex != -1 ? model.get(currentIndex).parameters : undefined
            }

            delegate: G.ListItemDelegate {
                id: _group_delegate

                height: G.Style.largeButtonHeight
                width: _group_list.width

                required property var parameters;
                required property string group;
                required property int index

                highlighted: _group_list.currentIndex == index

                text: getTitleString(group);
                font: G.Style.fonts.cardLabel

                onClicked: {
                    _group_list.currentIndex = index
                }
            }

            ScrollBar.vertical: ScrollBar { visible: _group_list.contentHeight > _group_list.height; }
        }

        G.ListItemDelegate {
            id: _new_group_delegate

            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            implicitHeight: G.Style.largeButtonHeight

            text: "New Group";
            font: G.Style.fonts.cardLabel

            G.IconButton {
                id: _new_group_button;

                iconName: "plus";
                size: G.Style.iconMedium;
                tooltip: "Add a new parameter group";

                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: G.Style.smallPadding

                onClicked: {
                    console.log("New parameter group")
                }
            }
        }
    }

    G.Gutter {
        id: _parameter_panel;

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: _group_panel.right;
        anchors.margins: G.Style.smallPadding;

        width: _self.width / 4;

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
            id: _parameter_list

            anchors.top: _parameter_title.bottom
            anchors.bottom: _new_parameter_delegate.top
            anchors.left: parent.left
            anchors.right: parent.right

            clip: true;
            focus: true;
            currentIndex: -1

            delegate: G.ListItemDelegate {
                id: _parameter_delegate

                height: G.Style.largeButtonHeight
                width: _parameter_list.width

                highlighted: _parameter_list.currentIndex == index

                required property var param;
                required property int index;

                Label {
                    id: _parameter_name_label

                    anchors.top: _parameter_delegate.top
                    anchors.left: _parameter_delegate.left
                    anchors.margins: G.Style.smallPadding

                    text: param.label
                    horizontalAlignment: Text.AlignLeft
                    font: G.Style.fonts.cardLabel
                }

                Label {
                    id: _parameter_type_label

                    anchors.bottom: _parameter_delegate.bottom
                    anchors.right: _parameter_delegate.right
                    anchors.margins: G.Style.smallPadding

                    text: param.type
                    horizontalAlignment: Text.AlignRight;
                    font: G.Style.fonts.value
                }

                onClicked: {
                    _parameter_list.currentIndex = index
                }
            }

            ScrollBar.vertical: ScrollBar { visible: _group_list.contentHeight > _group_list.height; }
        }

        G.ListItemDelegate {
            id: _new_parameter_delegate

            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            implicitHeight: G.Style.largeButtonHeight
            visible: _group_list.currentIndex != -1

            text: "New Parameter";
            font: G.Style.fonts.cardLabel

            G.IconButton {
                id: _new_parameter_button;

                iconName: "plus";
                size: G.Style.iconMedium;
                tooltip: "Add a new parameter";

                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: G.Style.smallPadding

                onClicked: {
                    console.log("New parameter")
                }
            }
        }
    }

    function getTitleString(group : string) : string {
        return (group.charAt(0).toUpperCase() + group.slice(1)).replace('_', ' ')
    }

}
