import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

import xQuick.Vis as XVis

Rectangle {

    id: self;

    color: G.Style.colors.bgColor;

    focus: true;

    XVis.Viewer {

        id: _view;

        anchors.fill: parent;

        mouseEnabled: true;

        onActiveFocusChanged: {
            if (_view.activeFocus) {
                window.currentView = self;
            }
        }
    }
}