import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls  1.0 as G
import gnomonQuick.Style     1.0 as G

G.Dialog {
    id: self;

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: G.Style.mediumDialogWidth
    height: G.Style.largeDialogHeight

    padding: 0;

    parent: Overlay.overlay

    modal: true
    title: "Workspaces"
    // standardButtons: Dialog.Close

    footer: DialogButtonBox
    {
        visible: true

        alignment: Qt.AlignRight
        background: Rectangle {
            anchors.fill: parent
            color: G.Style.colors.gutterColor
        }

        G.Button {
            text: 'Close';
            flat: true
            type: G.Style.ButtonType.Neutral
            width: G.Style.buttonWidth
            onClicked: self.reject();
        }

        G.Button {
            text: 'New';
            flat: true
            type: G.Style.ButtonType.Base
            width: G.Style.buttonWidth
            onClicked: {
                self.reject();
                _workspace_dialog.update_highlight();
                _workspace_dialog.open();
            }
        }
    }
    onOpened: {
        // this is insurance
        listView.currentIndex = window.current_workspace_index()
    }

    Connections {
        target: window
        function onCurrentWorkspaceIndexChanged(index: int) {
            listView.currentIndex = index
        }
    }

    ListView {
        id: listView

        clip: true;
        focus: true
        currentIndex: -1

        anchors.fill: parent
        anchors.margins: 0

        delegate: G.ListItemDelegate {
            required property int index
            required property string title

            width: listView.width
            text: title
            font: G.Style.fonts.cardLabel
            highlighted: ListView.isCurrentItem
            onClicked: {
                listView.currentIndex = index
                let workspace = window.workspace_at(index)
                if (workspace.d.hibernating) {
                    _hibernating_toast.workspace_title = workspace.workspace_title
                    _hibernating_toast.open()
                } else {
                    window.switch_workspace(index)
                    self.close();
                }
            }

            Connections {
                target: _hibernating_toast
                function onOpened() {
                    window.switch_workspace(listView.currentIndex)
                     _hibernating_toast.close()
                    self.close();
                }
            }

            Item {
                id: _icon_container;

                height: parent.height;
                width: parent.height;
                anchors.right: parent.right;
                anchors.rightMargin: G.Style.smallPadding;

                G.IconButton {
                    id: _close_workspace

                    anchors.right: parent.right;
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom

                    size: G.Style.iconSmall;
                    iconName: "close"
                    color: G.Style.colors.textColorBase;

                    visible: window.workspace_at(index) ? window.workspace_at(index).canBeDestroyed : false
                    onClicked: {
                        console.log("close wp... ", index );
                        closeWorkspace(index);
                    }
                }

                Image {
                    id: _icon
                    anchors.right: _close_workspace.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: G.Style.smallPadding

                    fillMode: Image.PreserveAspectFit
                    source: "image://workspaces/" + index;
                }

                G.Icon {
                    id: _hibernating_icon
                    anchors.right: _icon.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: G.Style.smallPadding

                    icon: "database"
                    size: G.Style.iconSmall;
                    color: G.Style.colors.textColorBase;
                    visible: window.workspace_at(index).d.hibernating
                    G.ToolTip {
                        text: "This workspace is hibernating to save memory."
                        visible: _hibernating_icon.hovered
                    }
                }

                Connections {
                    target: window
                    function onWorkspaceThumbnailUpdated(id) {
                        //this is the only way I found to actually refresh the image...
                        if(id === index) {
                            _icon.source = ""
                            _icon.source = "image://workspaces/" + index
                        }

                    }
                }
            }


        }

        section.property: "section"
        section.criteria: ViewSection.FullString
        section.delegate: ItemDelegate {

            id: _section_delegate;

            required property string section

            width: listView.width
            text: section

            hoverEnabled: false;

            contentItem: Text {
                rightPadding: _section_delegate.spacing;
                text: _section_delegate.text;
                font: _section_delegate.font;
                color: G.Style.colors.textColorNeutral;
                elide: Text.ElideRight;
                verticalAlignment: Text.AlignVCenter;
            }

            background: Rectangle {
                implicitWidth: G.Style.buttonWidth
                implicitHeight: G.Style.mediumLabelHeight
                opacity: enabled ? 0.8 : 0.3
                color: G.style.color.gutterColor

                Rectangle {
                    width: parent.width
                    height: 1
                    color: G.Style.colors.fgColor;
                    anchors.bottom: parent.bottom
                }
            }
        }

        model: window.workspace_list;
        ScrollIndicator.vertical: ScrollIndicator { }
    }

    G.Toast {
        id: _hibernating_toast

        property string workspace_title;

        parent: Overlay.overlay
        header: "Switching to workspace " + _hibernating_toast.workspace_title
        message: "The workspace was hibernating, please wait while its data is reloaded. This may take a few seconds. (You may change the hibernation threshold in the ⚙ Settings)"

        type: G.Style.ButtonType.Base
    }

    /* ListModel { */
    /*     id: _workspaces_model */
    /* } */

    /* onVisibleChanged : { */
    /*     if(visible) { */
    /*         var workspaces = window.workspaces_container(); */
    /*         _workspaces_model.clear() */
    /*         for (var i = 0; i < workspaces.count; i++) { */
    /*             var workspace = workspaces.children[i]; */
    /*             _workspaces_model.append({"title": workspace.workspace_title, "index": i}); */
    /*         } */
    /*     } */
    /* } */
    //onABoutToShow
}
