import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Window     2.15
import QtQuick.Layouts    1.15

import Qt5Compat.GraphicalEffects

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

import gnomon.Visualization 1.0 as GV
import gnomon.Mpl           1.0 as GV

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G

Rectangle {

    id: self;

    color: G.Style.colors.bgColor;

    focus: true;

    property alias view: _view;
    property var viewLogic;
    property var visualizations;
    property int number: _view.number;

    signal droppedFromFile(string path)
    signal droppedFromManager(int index)

    signal transmit();

    GV.FigureCanvas {
        id: _view;

        dpi_ratio: Screen.devicePixelRatio

        anchors.fill: parent

        onActiveFocusChanged: {
            if (_view.activeFocus)
                window.currentView = self;
        }

        onWidthChanged: _view.setGeom(_view.width, _view.height);
        onHeightChanged: _view.setGeom(_view.width, _view.height);
    }

    DropArea {

        id: _drop;

        anchors.fill: parent;

        G.Icon {
            icon: G.Icons.icons["arrow-down-circle"];
            size: G.Style.smallDelegateHeight;
            color: G.Style.colors.bgColor
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

    G.Icon { id: _export_icon;
        icon: viewLogic.inputView ? G.Icons.icons["arrow-down-circle"] : G.Icons.icons["arrow-up-circle"];
        enabled: !viewLogic.inputView
        size: G.Style.iconLarge;
        color: viewLogic.inputView ? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        clickable: true
        tooltip: viewLogic.inputView? "" : "Export"

        anchors.top: _view.top
        anchors.topMargin: 10
        anchors.right: _view.right
        anchors.rightMargin: 10

        onClicked: {
            viewLogic.transmit();

            // _view.requestCapture();
            // _view.update();
        }
    }

    layer.enabled: true
    layer.effect: OpacityMask
    {
        maskSource: Rectangle
        {
             width: self.width
            height: self.height
            radius: G.Style.panelRadius;
        }
    }

    Rectangle {

        id: _focus_indicator;

         width: self.width - 1
        height: self.height - 1
        radius: G.Style.panelRadius;

        color: G.Style.colors.transparent;

        border.width: G.Style.borderWidth;
        border.color: G.Style.colors.baseColor;

        visible: window.currentView == self;
    }

    Component.onCompleted: window.currentView = self;
}
