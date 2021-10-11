import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15
import QtQuick3D        1.15

import xQuick.Controls  1.0 as X
import xQuick.Fonts     1.0 as X
import xQuick.Style     1.0 as X
import xQuick.Vis       1.0 as XVis

Rectangle {

    id: _control;

    color: "#000000";

    XVis.Viewer {

        id: _viewer;

        anchors.fill: parent;

        mouseEnabled: true;
    }
}
