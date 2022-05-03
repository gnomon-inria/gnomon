import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import crossQuick        1.0 as C
import crossParameters   1.0 as C

import xQuick.Controls  1.0 as X
import xQuick.Fonts     1.0 as X
import xQuick.Style     1.0 as X

import gnomonQuick.Controls 1.0 as G

Page {

    id: _self;

    required property string workspace_title;
    required property var fill;

    // default property alias contents: _contents.children;

    property alias parameters: _params.params_model;

    G.Parameters {
        id: _params;
        parameters: d.parameters;
    }

    // Item {
    //     id: _contents;
    //     anchors.fill: parent;
    // }

    Rectangle {

        id: _banner;

        anchors.left: parent.left;
        anchors.leftMargin: 20;
        anchors.right: parent.right;
        anchors.rightMargin: 20;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 20;

        radius:  X.Style.controls.radius;
        color: "#bb000000";
        border.color: "#333333";
        border.width: 1;

        width: 400;
        height: 48;

        visible: false;

        RowLayout {

            anchors.fill: parent;

            BusyIndicator {
                id: _banner_indicator;

                        height: 38;
                implicitHeight: 38;
            }

            X.Label {
                text: "Computation in progress";
                Layout.fillWidth: true;
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft;
            }
        }
    }

    function updateParametersModel() {
        _params.parameters = d.parameters;
        _params.updateParametersModel();
    }

    function idleStart() {
        _banner.z = Infinity;
        _banner.visible = true;
        _banner_indicator.running = true;
    }

    function idleStop() {
        _banner.visible = false;
    }
}
