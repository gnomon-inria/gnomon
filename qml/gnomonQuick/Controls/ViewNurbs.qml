import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

import xQuick.Vis as XVis

Control {

    id: self;

    background: Rectangle {
        color: G.Style.colors.bgColor;
    }

    focus: true;
    hoverEnabled: true;

    XVis.Viewer {

        id: _view;

        anchors.fill: parent;

        mouseEnabled: true;

    }
}