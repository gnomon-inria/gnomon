import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import crossQuick        1.0 as C
import crossParameters   1.0 as C

import xQuick.Controls  1.0 as X
import xQuick.Fonts     1.0 as X
import xQuick.Style     1.0 as X

import gnomonQuick.Controls 1.0 as G
import gnomonQuick.Style 1.0 as G

G.Page {

    id: _self;

    required property string workspace_title;
    required property var fill;

    // default property alias contents: _contents.children;
    property alias parameters: _params.params_model;
    property var d: undefined;

    G.Parameters {
        id: _params;
        parameters: d ? d.parameters : null;
    }

    Rectangle {

        id: _banner;

        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        anchors.margins: G.Style.mediumPadding;

        radius: G.Style.panelRadius;
        color: G.Style.colors.gutterColor;
        border.color: G.Style.colors.fgColor;
        border.width: 1;

        width: G.Style.mediumPanelWidth;
        height: G.Style.largeLabelHeight;

        visible: false;

        ProgressBar {
            anchors.fill: parent
            value: sessionLoader.progress
            visible: window.load_in_progress
            opacity: 0.5
        }

        BusyIndicator {
            id: _banner_indicator;

            anchors.left: parent.left;
            anchors.verticalCenter: parent.verticalCenter;
            anchors.margins: G.Style.smallPadding

            height: G.Style.thumbnailLarge;
        }

        Label {
            anchors.left: _banner_indicator.right;
            anchors.right: parent.right;
            anchors.verticalCenter: parent.verticalCenter;
            anchors.margins: G.Style.smallPadding

            text: window.load_in_progress ? "Session loading in progress please dont launch other computations" : "Computation in progress" ;

            font: G.Style.fonts.value
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
