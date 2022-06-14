import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtQuick.Layouts  1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X
import xQuick.Style     1.0 as X
import xQuick.Fonts     1.0 as X

import gnomonQuick.Controls 1.0 as G
import gnomonQuick.Style    1.0 as G

G.ListItemDelegate {
    id: _self;

    property var name: ""
    property var label: ""
    property alias form_name: _label_edit.text

    text: ""
    hoverEnabled: false

    RowLayout {
        anchors.fill: parent
        anchors.margins: G.Style.smallPadding

        Label {
            Layout.fillHeight: true
            Layout.minimumWidth: parent.width/4

            text: _self.name
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft

            font: G.Style.fonts.formLabel
        }

        TextField {
            id: _label_edit

            Layout.fillWidth: true
            Layout.fillHeight: true

            font: G.Style.fonts.value

            text: _self.label
        }
    }
}
