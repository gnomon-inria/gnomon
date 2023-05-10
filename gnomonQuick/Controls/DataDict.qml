import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Window     2.15
import QtQuick.Layouts    1.15

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

import gnomonQuick.Controls  1.0 as G
import gnomonQuick.Style     1.0 as G
import gnomonQuick.Icons     1.0 as G

import gnomon.Visualization 1.0 as GV

Rectangle {

    id: self;

    color: G.Style.colors.gutterColor;

    focus: true;

    property alias view: _view;
    property var viewLogic;

    signal droppedFromFile(string path)
    signal droppedFromManager(int index)

    signal transmit();

    Control {
        id: _view;
        anchors.fill: parent
        clip: true

        TextEdit {
            id: _text

            anchors.fill: parent
            anchors.margins: G.Style.smallPadding

            text: viewLogic? viewLogic.displayText : ""
            readOnly: true
            wrapMode: Text.WrapAnywhere
            selectByMouse: true

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font: G.Style.fonts.formLabel
            color: G.Style.colors.textColorNeutral
        }

        onActiveFocusChanged: {
            if (_view.activeFocus)
                window.currentView = self;
        }
    }

    DropArea {

        id: _drop;

        anchors.fill: parent;

        G.Icon {
            icon: G.Icons.icons["arrow-down-drop-circle"];
            size: G.Style.smallDelegateHeight;
            color: G.Style.colors.fgColor
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


    G.IconButton {
        id: _export_icon;

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.right: _view.right
        anchors.rightMargin: G.Style.smallPadding

        iconName: viewLogic.inputView ? G.Icons.icons["arrow-down-drop-circle"] : G.Icons.icons["arrow-up-drop-circle"];
        enabled: !viewLogic.inputView
        size: G.Style.iconLarge;
        color: viewLogic.inputView ? G.Style.colors.fgColor : G.Style.colors.textColorNeutral;
        tooltip: viewLogic.inputView? "" : "Export"


        onClicked: {
            viewLogic.transmit()
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
