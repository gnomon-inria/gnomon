import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X
import xQuick.Fonts     1.0 as X
import xQuick.Style     1.0 as X

import gnomonQuick.Controls  1.0 as G

Dialog {
    id: self;

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: Math.min(window.width, window.height) / 3 * 2
    height: window.height / 3 * 2

    leftPadding: 0;
    rightPadding: 0;

    parent: Overlay.overlay

    modal: true
    title: "Workspaces"
    // standardButtons: Dialog.Close

    footer: X.DialogButtonBox
    {
        visible: true

        Button {
            text: 'New';

            onClicked: {
                self.accept();
                _workspace_dialog.open();
            }
        }

        Button {
            text: 'Close';
            onClicked: self.accept();
        }
    }


    ListView {
        id: listView

        clip: true;
        focus: true
        currentIndex: -1

        anchors.fill: parent

        delegate: ItemDelegate {
            width: listView.width
            text: model.title
            highlighted: ListView.isCurrentItem
            onClicked: {
                listView.currentIndex = index
                window.switch_workspace(index)
                self.close();
            }

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 80
                opacity: enabled ? 0.8 : 0.3
                // color: Qt.darker(X.Style.backgroundColor, 1.2)
                color: "#22000000";

                Rectangle {
                    width: parent.width
                    height: 1
                    color: X.Style.borderColor;
                    anchors.bottom: parent.bottom
                }
            }

            Item {
                id: _icon_container;

                height: parent.height;
                width: parent.height;
                anchors.right: parent.right;
                anchors.rightMargin: 10;

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
                color: X.Style.textColor;
                elide: Text.ElideRight;
                verticalAlignment: Text.AlignVCenter;
            }

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                opacity: enabled ? 0.8 : 0.3
                // color: Qt.darker(X.Style.backgroundColor, 1.2)
                color: "#11000000";

                Rectangle {
                    width: parent.width
                    height: 1
                    color: X.Style.borderColor;
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
