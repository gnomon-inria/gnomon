import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X
import xQuick.Fonts     1.0 as X
import xQuick.Style     1.0 as X

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

        G.Button {
            text: 'Close';
            flat: true
            type: G.Style.ButtonType.Neutral
            onClicked: self.reject();
        }

        G.Button {
            text: 'New';
            flat: true
            type: G.Style.ButtonType.Base

            onClicked: {
                self.reject();
                _workspace_dialog.update_highlight();
                _workspace_dialog.open();
            }
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
            width: listView.width
            text: model.title
            font: G.Style.fonts.cardLabel
            highlighted: ListView.isCurrentItem
            onClicked: {
                listView.currentIndex = index
                window.switch_workspace(index)
                self.close();
            }

            Item {
                id: _icon_container;

                height: parent.height;
                width: parent.height;
                anchors.right: parent.right;
                anchors.rightMargin: G.Style.smallPadding;

                Image {
                    id: _icon
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: "image://workspaces/" + index;
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
