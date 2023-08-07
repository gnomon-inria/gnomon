import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

G.Dialog {

    id: self

    x: Math.round((parent.width - width) / 2)
    y: Math.round(parent.height / 6)

    width: G.Style.largeDialogWidth;
    height: G.Style.largeDialogHeight;

    modal: true
    focus: true
    title: "Journal"

    standardButtons: Dialog.Ok

    leftPadding: 0;
    rightPadding: 0;
    bottomPadding: 0;
    topPadding: 0;

    leftMargin: 0;
    rightMargin: 0;

    spacing: 0;

    G.Console {

        id: _console;

        width: parent.width;
        height: parent.height;

        onItemsChanged: {
            ; // Just in case ...
        }

        G.Separator {
            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.right: parent.right;
        }

        G.Separator {
            anchors.left: parent.left;
            anchors.right: parent.right;
            anchors.bottom: parent.botttom;
        }
    }
}
