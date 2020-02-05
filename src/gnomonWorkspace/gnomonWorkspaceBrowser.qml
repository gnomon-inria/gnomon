// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

import QtQuick 2.9
import QtQuick.Controls 2.0

Rectangle {

    id: root;

    color: theme.value("@bg");

    signal clicked(string name);

    // /////////////////////////////////////////////////////////////////////////////
    // Components
    // /////////////////////////////////////////////////////////////////////////////

    Component {
        id: delegate;

        Rectangle {

            id: delegate_r;

            color: theme.value("@bgalt");

            border.width: 1;
            border.color: theme.value("@bd");

            height: 80;
            width: parent.width;

            Column {

                anchors.fill: parent;

                Text {
                    width: parent.width;

                    height: 40;

                    color: theme.value("@fg");

                    text: title;

                    font.pointSize: 24;

                    padding: 6;
                }

                TextArea {
                    width: parent.width;

                    height: 40;

                    color: theme.value("@fgalt");

                    text: description;

                    font.pointSize: 14;

                    wrapMode: TextEdit.WordWrap;
                }
            }

            MouseArea {
                anchors.fill: parent;

                onClicked: {
                    root.clicked(title);
                }
            }

        }
    }

    // /////////////////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////////////////

    Column {

        anchors.fill: parent;

        Rectangle {

            id: header;

            property alias text: label.text;

            width: parent.width;
            height: 25;

            color: theme.value("@bg");

            Text {

                id: label;

                anchors.verticalCenter: parent.verticalCenter;
                anchors.horizontalCenter: parent.horizontalCenter;

                color: theme.value("@fg");

                text: "Choose reader";

                font.pointSize: 16;
            }
        }

        SwipeView {
            id: view

            width: parent.width;
            height: parent.height - header.height;

            clip: true;

            ListView {
                id: readers

                delegate: delegate;

                model: ListModel { id: model; }

                ScrollBar.vertical: ScrollBar {
                    id: vbar2
                    visible: true;
                    hoverEnabled: true
                    active: true; // hovered || pressed
                    orientation: Qt.Vertical
                    size: view.height / readers.height
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }

                Component.onCompleted: {

                    for (var prop in reader_descs) {
                        model.append({
                                  "title": prop.toString(),
                            "description": reader_descs[prop].toString()
                        });
                    }
                }
            }
        }
    }
}

//
// dtkWidgetsWorkspaceBar.qml ends here
