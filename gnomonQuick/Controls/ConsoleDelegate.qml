import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Style as G

Control {

    height: 28;
    width: parent ? parent.width : 0;

    background: Rectangle {
        color: index % 2 ? "transparent" : "#11000000";
    }

    RowLayout {

        anchors.fill: parent;

        Label {
            text: modelData.label;

            font.bold: modelData.type > 1;

            color:
              modelData.type == 0 ? G.Style.colors.neutralColor
            : modelData.type == 2 ? G.Style.colors.warningColor
            : modelData.type == 3 ? G.Style.colors.dangerColor
            :                       G.Style.colors.exampleColor;

            Layout.minimumWidth: 64;
            Layout.maximumWidth: 64;

            leftPadding: 4;
        }

        Label {
            text: modelData.description;
            clip: true;

            leftPadding: 6;

            Layout.fillWidth: true;
        }

        Label {
            text: modelData.time;

            Layout.minimumWidth: 64;
            Layout.maximumWidth: 64;
        }
    }
}
