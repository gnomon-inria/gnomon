import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtQuick.Layouts  1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X
import xQuick.Style     1.0 as X
import xQuick.Fonts     1.0 as X

Control {
    id: _self;

    property var name: ""
    property var label: ""
    property var text: _label_edit.text

    RowLayout {
        anchors.fill: parent

        X.Label {
            Layout.preferredWidth: parent.width/4
            Layout.minimumWidth: parent.width/4
            Layout.fillHeight: true

            text: _self.name

            font {
                pointSize: 14
                weight: Font.Bold
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        X.TextField {
            id: _label_edit

            Layout.preferredWidth: 2*parent.width/3
            Layout.minimumWidth: 2*parent.width/3
            Layout.fillHeight: true

            text: _self.label
        }
    }
}
