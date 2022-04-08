import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Window     2.15
import QtQuick.Layouts    1.15

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

import gnomon.Visualization 1.0 as GV

Rectangle {

    id: self;

    color: Qt.darker(X.Style.alternateBaseColor);

    focus: true;

    property alias view: _view;
    property var viewLogic;

    signal droppedFromFile(string path)
    signal droppedFromManager(int index)

    signal transmit();

    Rectangle {
        id: _view;
        anchors.fill: parent
        color: X.Style.baseColor;
    }

    DropArea {

        id: _drop;

        anchors.fill: parent;

        X.Icon {
            icon: X.Icons.icons.arrow_circle_down;
            size: 56;
            color: "#44999999";
            visible: _drop.containsDrag;
            anchors.centerIn: parent;
        }

        onDropped: {
            if (drop.hasUrls) {
                console.log("DROP", drop.mimeData)
                drop.urls.forEach(function (item, index) {
                    console.log("DROP FILE", item)
                    self.droppedFromFile(item);
                });
            } else {
                self.droppedFromManager(drag.source.ref);
            }

            drop.accept();
        }
    }

    X.Icon { id: _export_icon;
        icon: X.Icons.icons.arrow_circle_up;
        size: 32;
        color: X.Style.foregroundColor;

        anchors.top: _view.top
        anchors.topMargin: 10
        anchors.right: _view.right
        anchors.rightMargin: 10

        MouseArea { id: _export_mouse_area;
            anchors.fill: parent;
            hoverEnabled: true;

            onClicked: {

                viewLogic.transmit();

            }
        }

        ToolTip.visible: _export_mouse_area.containsMouse;
        ToolTip.text: "Export";
    }

    Rectangle {

        id: _focus_indicator;

         width: self.width - 1
        height: self.height - 1
        radius: 4;

        color: "#00000000";

        border.width: 2;
        border.color: X.Style.accentColor;

        visible: window.currentView == self;
    }

    Component.onCompleted: window.currentView = self;
}
