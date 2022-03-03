import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X
import xQuick.Style     1.0 as X

Dialog {

    id: self

    required property var workspace_logic;

    x: Math.round((window.width  - width)/2)
    y: Math.round((window.height - height)/2)

    width: window.width / 3 * 2;
    height: 300;

    modal: true
    focus: true
    title: "Computing ..."

    standardButtons: Dialog.Ok | Dialog.Cancel

    leftPadding: 0;
    rightPadding: 0;
    bottomPadding: 0;

    leftMargin: 0;
    rightMargin: 0;

    spacing: 0;

    ColumnLayout {

        width: parent.width;
        height: parent.height;

        TextArea {
               topInset: 10;
              leftInset: 10;
             rightInset: 10;
            bottomInset: 10;

            Layout.fillWidth: true;
            Layout.preferredHeight: 100;
        }

        ProgressBar {

            Layout.fillWidth: true;
            Layout.preferredHeight: 22;

            indeterminate: true
        }
    }

    Timer {
        id: _timer;
        interval: 2000;
        onTriggered: self.destroy();
    }

    function start() {
        workspace_logic.run();
        _timer.start();
    }
}
